#!/usr/bin/awk -f

# use -F to specify the filed separator

BEGIN {
	print "========================================"
}

{
	i = 1
	while (i <= NF) {
		print $i
		i++
	}
}
