#!/bin/bash

set -e

if [ $# -lt 3 ];then # 如命令行参数输入错误，打印提示信息
    echo "Usage: source $0 <product> <debug|release> <user|userdebug|eng>"
	echo "     product    : livebox, ardbeg, etc."
	echo "     build type : release, debug."
	echo "     variant    : user, userdebug, eng."
    exit 1
fi

# In the root directory of android source tree.
export TOP=`pwd` 
. ./build/envsetup.sh
setpaths

PRODUCT=$1

# build type: 1. release; 2. debug
case "$2" in
	"release") BUILD_TYPE=1;;
	"debug") BUILD_TYPE=2;;
	*)
		echo "Invalid build type"
		exit 1
		;;
esac

# variant: 1. uesr; 2. userdebug; 3. eng
case "$3" in
	"user") VARIANT=1;;
	"userdebug") VARIANT=2;;
	"eng") VARIANT=3;;
	*)
		echo "Invalid Variant"
		exit 1
		;;
esac

choosecombo $BUILD_TYPE $PRODUCT $VARIANT
