#!/bin/bash

APC_IP=172.24.200.123

if [[ $# != 2 ]];then
    echo "Usage: $0 @/remote/path /local/path. or verse vise"
    exit 1
fi

src=$1
dst=$2

if [[ ${src:0:1} == "@" ]]; then
    src=`echo $src | cut -c 2-`
    scp -i ~/.ssh/root_id_rsa root@$APC_IP:$src $dst
elif [[ ${dst:0:1} == "@" ]]; then
    dst=`echo $dst | cut -c 2-`
    scp -i ~/.ssh/root_id_rsa $src root@$APC_IP:$dst
else
    echo "Usage: $0 @/remote/path /local/path. or verse vise"
    exit 1
fi

