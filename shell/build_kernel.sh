#!/bin/bash

################################# config #################################
CODEGEN_PREFIX=arm-hisiv400-linux-
OUTPUT_DIR=`pwd`/../build
TFTP_HOME=/home/zhs/workspace/tftphome
MOD_PATH=${OUTPUT_DIR}/mod
BOARD_DEFCONF=gamebox_defconfig
THREAD_NUM=5
##########################################################################

ARCH=arm
CROSS_COMPILE=${CODEGEN_PREFIX}

############ color output #############
red='\E[31;1m'
green='\E[32;1m'
white='\E[37;1m'
function info {
	echo -e "$white$1\E[0;m"
}

function good {
	echo -e "$green$1\E[0;m"
}

function err {
	echo -e "$red$1\E[0;m"
}
########################################

function save_config() {
	TARGET=savedefconfig
    kernel_build_target ${TARGET}
	cp -v ${OUTPUT_DIR}/defconfig `pwd`/arch/arm/configs/${BOARD_DEFCONF}
}

function kernel_build_target() {
    make -j${THREAD_NUM} O=${OUTPUT_DIR} CROSS_COMPILE=${CODEGEN_PREFIX} ARCH=arm ${1}\
        INSTALL_MOD_PATH=${MOD_PATH}
}

function build_mod {
	TARGET=modules
	make -j${THREAD_NUM} O=${OUTPUT_DIR} CROSS_COMPILE=${CODEGEN_PREFIX} ARCH=arm ${TARGET} INSTALL_MOD_PATH=${MOD_PATH}
	TARGET=modules_install
	make -j${THREAD_NUM} O=${OUTPUT_DIR} CROSS_COMPILE=${CODEGEN_PREFIX} ARCH=arm ${TARGET} INSTALL_MOD_PATH=${MOD_PATH}

	cd ${MOD_PATH}
	tar cf mods.tar lib
	mv -v mods.tar ${TFTP_HOME}
}

function check_version {
    kver=$(kernel_build_target "kernelversion")
    krel=$(kernel_build_target "kernelrelease")
    good "kernel version = ${kver}"
    good "kernel release = ${krel}"
}

function print_usage {
    info "===================================================================="
	info "build tree directory is: '$OUTPUT_DIR'"
    info "===================================================================="
	info "options:"
	info "  defconfig :  use default config '$BOARD_DEFCONF'"
	info "  savecfg   :  save current .config as '$BOARD_DEFCONF'"
	info "  config    :  make 'menuconfig'"
	info "  make      :  make target 'uImage'"
	info "  mod       :  build modules and install to '$MOD_PATH'"
	info "  clean     :  make clean"
	info "  distclean :  make distclean"
	info "  install   :  copy uImage to '${TFTP_HOME}'"
	info "  version   :  print kernel's version"
	info "  help      :  print this info"
    info "===================================================================="
}

if [[ ! -e ${OUTPUT_DIR} ]];then
	mkdir -p ${OUTPUT_DIR}
fi

good "run command '$1'"

if [[ $1 == "defconfig" ]]; then
	TARGET=${BOARD_DEFCONF}
    kernel_build_target ${TARGET}
elif [[ $1 == "saveconfig" ]]; then
	save_config
elif [[ $1 == "config" ]]; then
	TARGET=menuconfig
    kernel_build_target ${TARGET}
elif [[ $1 == "make" ]]; then
	TARGET=uImage
    kernel_build_target ${TARGET}
elif [[ $1 == "mod" ]]; then
	build_mod
elif [[ $1 == "clean" ]]; then
	TARGET=clean
    kernel_build_target ${TARGET}
elif [[ $1 == "distclean" ]]; then
	TARGET=distclean
    kernel_build_target ${TARGET}
elif [[ $1 == "install" ]]; then
	cp -v ${OUTPUT_DIR}/arch/arm/boot/uImage ${TFTP_HOME}/uImage
elif [[ $1 == "version" ]]; then
    check_version
elif [[ $1 == "help" ]]; then
	print_usage
else
	err "invalid target"
	print_usage
	exit 1
fi
