#!/bin/sh

grep Codec . -Rl --exclude=test.sh |
while IFS= read -r line
do
    grep input $line -l
done |
while IFS= read -r line
do
	echo "--> $line"
done
