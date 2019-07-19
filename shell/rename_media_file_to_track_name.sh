#!/bin/sh

F=$1
SUFFIX="${F##*.}"

realname=`mediainfo $F | grep "Track name" | cut -d ':' -f 2 | xargs`
echo "$F -> "$realname.$SUFFIX""
mv $F "$realname.$SUFFIX"
