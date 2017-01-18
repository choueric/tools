#!/bin/sh

# Usage:
# 1. copy this script into the directory of vim source.
# 2. ./vim_build.sh
# 3. make && sudo make install

./configure \
	--enable-pythoninterp=yes \
	--enable-gui=gnome2 \
	--with-x \
	--enable-gnome-check \
	--enable-gtk2-check \
	--with-gnome
