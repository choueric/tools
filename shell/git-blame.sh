#!/bin/sh

ABS_PATH=`readlink $0`
CUR_PATH="$( cd "$(dirname "$ABS_PATH")" ; pwd -P )"

PY_BLAME="$CUR_PATH/git-blame-colored-pygments.py"

$PY_BLAME $1 | less -R
