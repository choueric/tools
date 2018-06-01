#!/bin/bash

#set -e
#set -x

LOG=/dev/stdout

while read line
do
	path=`echo $line | cut -d ":" -f 1`
	pushd $path > /dev/null
	info=`git log -10 | grep magewell`
	if [ "$info" != "" ]; then
		echo "---------------------------------" >> $LOG
		echo $path >> $LOG
		echo $info >> $LOG
	fi
	popd > /dev/null
done < repo_list
