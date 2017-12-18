#!/bin/bash

# $1: file, $2: format, $3: width, $4: height

FILE=$1
FMT=$2
FORMAT=2

if [[ $FMT == "nv12" ]]; then
	FORMAT=23
elif [[ $FMT == "i420" ]]; then
	FORMAT=2
elif [[ $FMT == "rgba" ]]; then
	FROMAT=11
elif [[ $FMT == "h264" ]]; then
	mplayer -fps 30 $FILE
	exit 0
else
	echo "invalid format"
	echo "Usage: play_video <file> <fmt> [width] [height]"
	echo "  fmt: nv12, i420, rgba, h264"
	exit 1
fi

WIDTH=1920
HEIGHT=1080

if [[ $3 != "" ]]; then
	WIDTH=$3
fi
if [[ $4 != "" ]]; then
	HEIGHT=$4
fi


gst-launch-1.0 filesrc location=$FILE ! videoparse width=$WIDTH height=$HEIGHT framerate=30/1 format=$FORMAT ! autovideoconvert ! autovideosink
