#!/bin/bash

GO_PREFIX="$HOME/usr"

function uninstall() {
	echo "unisntall $GO_PREFIX/go ..."
	rm -rf "$GO_PREFIX/go"
}

function install() {
	echo "install $1 to $GO_PREFIX ..."
	tar -C $GO_PREFIX -xzf $1
}

function print_usage() {
	echo "Usage: ./insall.sh <cmd> [option]"
	echo "  cmd:"
	echo "    - install <packge>"
	echo "    - uninstall"
}

if [[ $1 == "install" ]]; then
	if [[ $# != "2" ]]; then
		print_usage
		exit 1
	fi
	install $2
elif [[ $1 == "uninstall" ]]; then
	uninstall
else
	print_usage
fi
