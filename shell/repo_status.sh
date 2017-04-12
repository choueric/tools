#!/bin/bash

IN=$1

if [ "$IN" = "" ]; then
	echo "Need input file"
	exit 1
fi

while read line
do
	path=`echo $line | cut -d ":" -f 1`
	pushd $path > /dev/null
	info=`git br | grep -v "no branch"`
	if [ "$info" != "" ]; then
		echo "------ $path"
		git br
		echo ""
	fi
	popd > /dev/null
done < $IN
