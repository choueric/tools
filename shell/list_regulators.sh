#!/bin/sh

# list all regulators
cd /sys/class/regulator
for d in *
do
	cd $d
	p=`pwd | cut -d '/' -f 5`
	n=`cat name`
	echo "$p: \t$n"
	cd ..
done
