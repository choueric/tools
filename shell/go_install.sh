#!/bin/bash

GO_INSTALL_DIR_DEF="/usr/local/go"
GO_INSTALL_PATH_DEF="/usr/local"

function uninstall() {
	echo "unisntall $GO_INSTALL_DIR_DEF ..."
	sudo rm -rf $GO_INSTALL_DIR_DEF
}

function install() {
	echo "install $1 to $GO_INSTALL_PATH_DEF ..."
	sudo tar -C $GO_INSTALL_PATH_DEF -xzf $1
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
