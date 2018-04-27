#!/bin/sh

git filter-branch -f --commit-filter '
        if [ "$GIT_AUTHOR_EMAIL" = "haishan.zhou@magewell.com" ];
        then
                GIT_AUTHOR_NAME="choueric";
                GIT_AUTHOR_EMAIL="zhssmail@gmail.com";
                git commit-tree "$@";
        else
                git commit-tree "$@";
        fi' HEAD
