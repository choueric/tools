#!/bin/sh

set -e
#set -x

printUsage() {
	echo "Usage: [unzip] | [convert format]"
	echo "  - unzip  : loop unzip files in current directory."
	echo "  - convert: convert files into mp3 by format."
	exit 1
}

convertAudio() {
	if [ "$#" != "3" ]; then
		printUsage
	fi
	SUFFIX=$1
	for f in *.${SUFFIX}
	do
		b=`basename -s .${SUFFIX} "$f"`
		avconv -i "$f" -b 320k "$b".mp3
	done
}

unzipFiles() {
	SUFFIX=zip
	for f in *.${SUFFIX}
	do
		b=`basename -s .${SUFFIX} "$f"`
		mkdir "$b"
		cd "$b"
		unzip ../"$f"
		cd ..
	done
}

if [ "$1" = "unzip" ]; then
	unzipFiles
elif [ "$1" = "convert" ]; then
	convertAudio $2
else
	printUsage
fi

