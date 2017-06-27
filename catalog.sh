echo ":::::::::::"
echo ": Catalog :"
echo ":::::::::::"

find . -maxdepth 1 -type f -name "*.jpg" -print

echo "#"

list=$( find . -maxdepth 1 -type f -name "*.jpg" -print )

for file in $list
do
    md5=$( md5sum $file)
	#printf "%60s %30s\n", $file, $md5
	echo $md5
done
