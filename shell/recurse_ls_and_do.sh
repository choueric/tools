#!/bin/sh

for f in *.flac
do
	b=`basename -s .flac "$f"`
	avconv -i "$f" -b 320k "$b".mp3
done
