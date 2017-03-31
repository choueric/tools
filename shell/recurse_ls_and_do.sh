#!/bin/sh

set -e
#set -x

CMD_UNZIP="unzip"
CMD_CONVERT="convert"
CMD_RESUFFIX="resuffix"

printUsage() {
	echo "Usage: $0 <command>\nAvailable commands are:"
	echo "  - $CMD_UNZIP\t: loop unzip files in current directory."
	echo "  - $CMD_CONVERT\t: <format>, convert files with suffix <format> into mp3."
	echo "  - $CMD_RESUFFIX\t: <old> <new>, rename suffix from <old> to <new>."
	exit 1
}

convertAudio() {
	if [ "$1" = "" ]; then
		echo "$CMD_CONVERT need format suffix, like 'flac', 'wav'"
		exit 1
	fi
	SUFFIX=$1
	for f in *.${SUFFIX}
	do
		b=`basename -s .${SUFFIX} "$f"`
		avconv -i "$f" -b 320k "$b".mp3
	done
}

resuffix() {
	if [ "$1" = "" ] || [ "$2" = "" ]; then
		echo "$CMD_RESUFFIX need suffix <old> and <new>"
		exit 1
	fi
	SUFFIX=$1
	NEWSUFFIX=$2

	for f in *.${SUFFIX}
	do
		b=`basename -s .${SUFFIX} "$f"`
		mv -v "$f" "$b".${NEWSUFFIX}
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

case "$1" in
	$CMD_UNZIP) unzipFiles;;
	$CMD_CONVERT) convertAudio $2;;
	$CMD_RESUFFIX) resuffix $2 $3;;
	*) printUsage;;
esac
