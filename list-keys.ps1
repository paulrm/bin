Write-Host "`n===== POW E R T O Y S ====="

$path = "$env:LOCALAPPDATA\Microsoft\PowerToys\Keyboard Manager"

if (Test-Path $path) {
    Get-ChildItem $path -Filter *.json | ForEach-Object {
        Write-Host "`n--- $($_.Name) ---"
        Get-Content $_.FullName -Raw
    }
}

Write-Host "`n===== WINDOWS KEY REMAPPING ====="

Get-ItemProperty `
    -Path "HKLM:\SYSTEM\CurrentControlSet\Control\Keyboard Layout" `
    -Name "Scancode Map" `
    -ErrorAction SilentlyContinue

Write-Host "`n===== GLOBAL HOTKEY SCAN ====="

if (-not ("HotkeyProbe" -as [type])) {
    Add-Type @"
using System;
using System.Runtime.InteropServices;

public class HotkeyProbe {
    [DllImport("user32.dll", SetLastError=true)]
    public static extern bool RegisterHotKey(
        IntPtr hWnd, int id, uint modifiers, uint vk);

    [DllImport("user32.dll", SetLastError=true)]
    public static extern bool UnregisterHotKey(
        IntPtr hWnd, int id);

    public static int Test(uint modifiers, uint vk) {
        if (RegisterHotKey(IntPtr.Zero, 1, modifiers, vk)) {
            UnregisterHotKey(IntPtr.Zero, 1);
            return 0;
        }
        return Marshal.GetLastWin32Error();
    }
}
"@
}

$modifiers = [ordered]@{
    "Alt"            = 1
    "Ctrl"           = 2
    "Ctrl+Alt"       = 3
    "Alt+Shift"      = 5
    "Ctrl+Shift"     = 6
    "Ctrl+Alt+Shift" = 7
    "Win"            = 8
    "Win+Shift"      = 12
}

$keys = [ordered]@{}

65..90 | ForEach-Object {
    $keys["$([char]$_)"] = $_
}

1..12 | ForEach-Object {
    $keys["F$_"] = 111 + $_
}

$results = foreach ($mod in $modifiers.GetEnumerator()) {
    foreach ($key in $keys.GetEnumerator()) {

        $code = [HotkeyProbe]::Test(
            $mod.Value, $key.Value
        )

        [PSCustomObject]@{
            Shortcut = "$($mod.Key)+$($key.Key)"
            Status = switch ($code) {
                0    { "AVAILABLE" }
                1409 { "OCCUPIED" }
                default { "ERROR/RESERVED" }
            }
            ErrorCode = $code
        }
    }
}

$results |
    Where-Object Status -ne "AVAILABLE" |
    Format-Table -AutoSize

$results | Group-Object Status |
    Select-Object Name, Count |
    Format-Table -AutoSize