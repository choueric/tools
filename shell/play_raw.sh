#!/bin/bash

# play raw video files with gstreamer.
# play other format, e.g. h264, by mplayer like `mplayer -fps 30 video.h264`

FILE=$1
FMT=$2
FORMAT=2

if [[ $FMT == "nv12" ]]; then
	FORMAT=23
elif [[ $FMT == "i420" ]]; then
	FORMAT=2
elif [[ $FMT == "RGBA" ]]; then
	FROMAT=11
else
	echo "invalid format"
	exit 1
fi

#WIDTH=800
#HEIGHT=600

WIDTH=1920
HEIGHT=1080

gst-launch-1.0 filesrc location=$FILE ! videoparse width=$WIDTH height=$HEIGHT framerate=30/1 format=$FORMAT ! autovideoconvert ! autovideosink
