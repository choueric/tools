#!/bin/sh

set -e

if mountpoint -q /; then
   echo "mounted"
else
    echo "unmounted"
fi
