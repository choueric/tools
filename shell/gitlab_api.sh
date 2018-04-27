#!/bin/bash

# refer: https://docs.gitlab.com/ce/api/projects.html

set -e
#set -x

PRIVATE_TOKEN=HgBLhHJ_fPV3DcRxFu5F
API_URL=http://git.magewell.nj/api/v4
PROJECTS_API=${API_URL}/projects
NAMESPACE_ID=4

IMPORT_BASE="ssh://git@192.168.1.7:tk1-android"

################################################################################

function apiCreateProject() {
	name=$1
	visibility=$2
	namespace_id=$3
	curl --header "PRIVATE-TOKEN: $PRIVATE_TOKEN" -X POST \
		"${PROJECTS_API}?name=${name}&namespace_id=${namespace_id}&visibility=${visibility}" \
		> curl.log
	repo_url=`cat curl.log | jq ".ssh_url_to_repo" | tr -d '"'`
	if [ "$repo_url" = "" ]; then
		cat curl.log
		exit 1
	fi
	echo "$repo_url"
}

function apiGetProjects() {
	curl --header "PRIVATE-TOKEN: $PRIVATE_TOKEN" "${PROJECTS_API}"
}

function apiRemoveProject() {
	id=$1
	curl --header "PRIVATE-TOKEN: $PRIVATE_TOKEN" -X DELETE "${PROJECTS_API}/$id"
}

function apiGetSingleProject() {
	id=$1
	curl --header "PRIVATE-TOKEN: $PRIVATE_TOKEN" "${PROJECTS_API}/$id"
}

################################################################################

function convertRepoName() {
	name=$1
	ret=`echo $name | sed "s/\//_/g"`
	echo $ret
}

function createProject() {
	new_name=$1
	echo "$(apiCreateProject $new_name "private" $NAMESPACE_ID)"
}

function importProject() {
	repo_path=$1
	repo_url=$2
	remote=gitlab
	pushd $repo_path
	echo "    push $repo_path into $repo_url"
	current=`git rev-parse --abbrev-ref HEAD`
	if [ $current = "HEAD" ]; then
		git checkout -b master
	fi
	git remote add $remote "$repo_url"
	git push -u $remote --all
	git push -u $remote --tags
	popd
}

function transferRepo() {
	echo "---> $repo_name"
	repo_name=$1
	repo_path=$2
	new_name=$(convertRepoName $repo_name)
	repo_url=$(createProject $new_name)
	importProject $repo_path $repo_url
	echo "-----------------------------------------------------"
}


git_dir=$HOME/workspace/TK1/git
cd $git_dir
repo_list=$git_dir/repo_list

case "$1" in
	"import")
		while read line
		do
			echo $line >> output.repo_list
			repo_path=`echo $line | cut -d ":" -f 1 | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
			repo_name=`echo $line | cut -d ":" -f 2 | grep -o "[^ ]\+\( \+[^ ]\+\)*"`
			transferRepo $repo_name $repo_path
			sleep 5
		done < $repo_list
		;;
	"gets")
		apiGetProjects
		;;
	"get")
		if [ "$2" = "" ];then
			echo "need project id"
			exit 1
		fi
		apiGetSingleProject $2
		;;
	"del")
		if [ "$2" = "" ];then
			echo "need project id"
			exit 1
		fi
		apiRemoveProject $2
		;;
	*)
		echo "wrong option. import | get | gets | del."
		exit 1
esac
