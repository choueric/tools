#!/bin/sh

# install libav-tools for avconv

for file in *.flac
do
	echo "converting $file ..."
	name=`basename "$file" .flac`
	avconv -i "$file" -b 320k "${name}.mp3"
done
