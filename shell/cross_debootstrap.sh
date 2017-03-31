#!/bin/bash

# required packages:
# sudo apt-get install binfmt-support qemu qemu-user-static debootstrap

set -e

ARCH=armhf
DIST=xenial
DST_DIR="rootfs_${DIST}_${ARCH}"

PKGS="
ubuntu-minimal,\
xserver-xorg,\
xserver-xorg-core,\
xinit,\
xterm,\
x11-xserver-utils,\
alsa-utils,\
wireless-tools,\
wpasupplicant,\
openssh-client,\
openssh-server,\
bzip2,\
less,\
iputils-ping,\
isc-dhcp-client,\
net-tools,\
sudo,\
bash,\
bash-completion,\
vim,\
iw,\
bluez,\
gdisk,\
wget,\
language-pack-en-base,\
xfonts-base,\
ntp"

############ color output #############
red='\E[31;1m'
green='\E[32;1m'

info() {
	echo -e "$green$1\E[0;m"
}

err() {
	echo -e "$red$1\E[0;m"
}
########################################

print_usage() {
	echo -e "Usage: $0 <command>\nAvailable commands are:"
	echo "  down   : download and setup the rootfs."
	echo "  config : run post-build config commands."
	echo "  build  : build the rootfs (down & config)."
	echo "  bash   : chroot bash."
	echo "  pull   : backup archives."
	echo "  push   : apply archives."
}

if [ $# != 1 ]; then
	print_usage
	exit 1
fi

do_down() {
	info "download packages" 
	debootstrap \
		--foreign \
		--verbose \
		--no-check-gpg \
		--arch=$ARCH \
		--variant=minbase \
		--include="$PKGS"	\
		--components=main,restricted,universe \
		$DIST $DST_DIR \
		http://ports.ubuntu.com/
	cp /usr/bin/qemu-arm-static $DST_DIR/usr/bin

	export LC_ALL=C

	chroot $DST_DIR /debootstrap/debootstrap --second-stage
	cp -v sources.list $DST_DIR/etc/apt
	chroot $DST_DIR apt-get update
	chroot $DST_DIR apt-get install -y python3
	chroot $DST_DIR dpkg --configure -a
}

do_config() {
	info "post-build config" 

	export LC_ALL=C

	mount -t proc proc $DST_DIR/proc

	info ">>> Input password of root:"
	chroot $DST_DIR bash -c "echo root:root | chpasswd"
	info ">>> Create new user 'ubuntu':"
	chroot $DST_DIR useradd ubuntu
	chroot $DST_DIR bash -c "echo ubuntu:ubuntu | chpasswd"
	chroot $DST_DIR usermod -aG sudo,video ubuntu
	chroot $DST_DIR bash -c "echo "tegra-ubuntu" > /etc/hostname"
	chroot $DST_DIR bash -c "echo -e "127.0.0.1\ tegra-ubuntu" >> /etc/hosts"
	chroot $DST_DIR apt-get -y install ubuntu-desktop

	chroot $DST_DIR update-locale LC_ALL=C.UTF-8

	# configure gcc
	chroot $DST_DIR apt-get install -y gcc-4.8 gcc-4.8-multilib g++-4.8 g++-4.8-multilib
	chroot $DST_DIR update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50 
	chroot $DST_DIR update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 40
	chroot $DST_DIR update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 40 
	chroot $DST_DIR update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50

	backup_archives "pull"

	chroot $DST_DIR apt-get clean
	umount $DST_DIR/proc
}

do_bash() {
	info "bash" 

	mount -t proc proc $DST_DIR/proc
	chroot $DST_DIR bash
	umount $DST_DIR/proc
}

setup_archives() {
	if [ -e $DST_DIR ]; then
		info "Remove existing $DST_DIR"
		rm -rf $DST_DIR
	fi
	info "Push archives"
	mkdir -p $DST_DIR/var/cache/apt
	backup_archives "push"
}

backup_archives() {
	if [ $1 = "pull" ]; then
		mv $DST_DIR/var/cache/apt/archives .
	elif [ $1 = "push" ]; then
		rm $DST_DIR/var/cache/apt/archives -rf
		mv archives $DST_DIR/var/cache/apt/
	fi
}

# if the user is not root, there is not point in going forward
THISUSER=`whoami`
if [ "x$THISUSER" != "xroot" ]; then
    echo "This script requires root privilege"
    exit 1
fi

if [ $1 = "down" ]; then
	do_down
elif [ $1 = "config" ]; then
	do_config
elif [ $1 = "bash" ]; then
	do_bash
elif [ $1 = "pull" ]; then
	backup_archives "pull"
elif [ $1 = "push" ]; then
	backup_archives "push"
elif [ $1 = "build" ]; then
	setup_archives
	do_down
	do_config
else
	print_usage
	exit 1
fi

exit 0
