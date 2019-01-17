<?PHP
#tmp/todoist.items.json
$data=file_get_contents("data/activity-todoist/todoist.items.json") ; 
$decoded=json_decode($data, true); 
$outdir="data/activity-todoist";
foreach($decoded as $v) 
    { 
    ksort($v);
    $out=json_encode($v,JSON_PRETTY_PRINT); 
    $fname=$outdir . "/" . $v["id"] . ".json";
    file_put_contents($fname,$out);
    echo "writed id=" . $v["id"] . "\n";
    }
