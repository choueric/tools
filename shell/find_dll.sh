#!/bin/sh

LIB_PATH=/usr/lib/arm-linux-gnueabihf/tegra

search_lib() {
	LIB_PATH=$1
	FUNC=$2
	find $LIB_PATH -name "*.so" -exec bash -c "nm -D {} | grep 'T $FUNC' &&  echo {}" \;
}

read_output() {
	while read line 
	do
		OUT=`search_lib $LIB_PATH $line | tail -n 1`
		echo "$line\t$OUT"
	done < ./libs.input
}

(printf "Functions Library\n"; read_output) | column -t
