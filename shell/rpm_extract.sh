#!/bin/sh

[ -z $1 ] && echo "Need RPM pkg name" && exit 1

rpm2cpio $1 | cpio -div
