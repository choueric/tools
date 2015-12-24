#!/bin/sh

# find path $FIND_PATH
# - exlude path patter *dvr_rdk*
# - type is file
# - perm and name
# - exlude name libpthread*.so* and *.ko

FIND_PATH=/home/zhs/workspace/ti8107/svn/EmbPlatformDM8107/trunk/buildroot-2015.08.1/output/target
EXLUDE_PATH=/home/zhs/workspace/ti8107/svn/EmbPlatformDM8107/trunk/buildroot-2015.08.1/output/target/opt/dvr_rdk*

find $FIND_PATH -not \( -path '*dvr_rdk*' \)  -type f \( -perm /111 -o -name '*.so*' \) -not \( -name 'libpthread*.so*' -o -name '*.ko' \) 
# or this one
find $FIND_PATH  \( -path $EXLUDE_PATH \) -prune -o -type f \( -perm /111 -o -name '*.so*' \) -not \( -name 'libpthread*.so*' -o -name '*.ko' \) -print
