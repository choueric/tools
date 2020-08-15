#!/bin/bash

input="./output.txt"

for f in *.mp3; do mediainfo --Inform="General;%Album%:%CompleteName%" "$f"; done > "$input"

while IFS= read -r line
do
  album=`echo "$line" | awk -F ":" '{print $1}'`
  file=`echo "$line" | awk -F ":" '{print $2}'`
  echo "[$album] - [$file]"
  mkdir -p "$album"
  mv "$file" "$album"
done < "$input"
