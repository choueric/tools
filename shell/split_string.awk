#!/usr/bin/awk -f

BEGIN {
	FS=":"
}

{
	i = 1
	while (i <= NF) {
		print $i
		i++
	}
}
