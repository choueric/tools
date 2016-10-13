#!/bin/bash

# usage:
# $ find . -name "*.ape" ape2mp3_wrapper.sh {} \;
# search all ape files in current directory and invoke ape2mp3.sh on each of 
# them.

if [[ $# < 1 ]];then
	echo "please input ape filename"
	exit 1
fi

DIRNAME=`dirname "$1"`

cd "$DIRNAME"
ape2mp3.sh "$1"

echo -e "\E[32;47m" "=========================================================="
tput sgr0
