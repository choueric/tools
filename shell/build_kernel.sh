#! /bin/bash

CONFIG_PATH="./arch/arm/configs/"
UIMAGE_PATH="./arch/arm/boot"
NOW_CONFIG="./.config"
CONFIG_ARCH=
MOD_PATH="./build"
ROOTFS=
WIFI_DRV_PATH="drivers/net/wireless/bcm4329/"
WIFI_FS_PATH=

VERSION_FILE="./Makefile"
KERNEL_VERSION="ver=Linux-"
VERSION_SUFFIX=

function get_kernel_version() {
	if [ ! -e $VERSION_FILE ] || [ ! -f $VERSION_FILE ]; then
		echo "$VERSION_FILE is not found or is not file!"
		return 1
	fi

    i=0
	while read line
	do
		var=`echo $line | cut -d= -f2 | tr -d " "`
		if [ $i == 0 ]; then
			KERNEL_VERSION=$KERNEL_VERSION$var
		elif [ $i == 1 ]; then
			KERNEL_VERSION=$KERNEL_VERSION.$var
		elif [ $i == 2 ]; then
			KERNEL_VERSION=$KERNEL_VERSION.$var
		elif [ $i == 3 ]; then
			KERNEL_VERSION=$KERNEL_VERSION$var-$1
			return 0
		fi
		i=$(($i+1))
	done < $VERSION_FILE
}

if [ "$1" = "miracle" ] ; then
    echo "<------------ select mircale --------------------->"
    ITEM_PATH="./item/miracle"
    ITEM_CONFIG="miracle_defconfig"
    ITEM_LOGO="./drivers/video/logo/logo_linux_clut224_miracle.ppm"
	ROOTFS="./rootfs/rootfs"
	WIFI_FS_PATH="rootfs/rootfs/wifi/"
	VERSION_SUFFIX="MIR"
elif [ "$1" = "astro" ] ; then
    echo "<------------ select astro --------------------->"
    ITEM_PATH="./item/astro"
    ITEM_CONFIG="astro_defconfig"
    ITEM_LOGO=
	ROOTFS="./rootfs/rootfs"
	WIFI_FS_PATH="rootfs/rootfs/wifi/"
	VERSION_SUFFIX="AST"
elif [ "$1" = "aed" ] ; then
    echo "<------------ select aed --------------------->"
    ITEM_PATH="./item/aed"
    ITEM_CONFIG="aed_defconfig"
	ROOTFS="./rootfs/aed_rootfs"
    MOD_PATH="./rootfs/aed_user"	
	MOD_CRAMFS="${ITEM_PATH}/aed_lib.cramfs"
    ITEM_LOGO="./drivers/video/logo/logo_linux_clut224_aed.ppm"
	VERSION_SUFFIX="AED"
elif [ "$1" = "vs900" ] ; then
    echo "<------------ select vs900 --------------------->"
    ITEM_PATH="./item/vs900"
    ITEM_CONFIG="vs900_defconfig"
    ITEM_LOGO="./drivers/video/logo/logo_linux_clut224_miracle.ppm"
	ROOTFS="./rootfs/vs900_rootfs"
    WIFI_FS_PATH="rootfs/vs900_rootfs/wifi/"
	VERSION_SUFFIX="VS9"
elif [ "$1" = "ecg" ] ; then
    echo "<------------ select ecg --------------------->"
    ITEM_PATH="./item/ecg"
    ITEM_CONFIG="ecg_defconfig"
	ROOTFS="./rootfs/ecg_rootfs"
    MOD_PATH="./rootfs/ecg_user"	
	MOD_CRAMFS="${ITEM_PATH}/ecg_lib.cramfs"
    ITEM_LOGO="./drivers/video/logo/logo_linux_clut224_aed.ppm"			
	VERSION_SUFFIX="ECG"
else
    echo "build                   ----> display support command"
    echo "build help              ----> display support command"
    echo "build miracle           ----> make uImage for miracle(iMEC)"
    echo "build miracle config    ----> make menuconfig for miracle(iMEC)"
    echo "build miracle clean     ----> make clean for miracle(iMEC)"
    echo "build astro             ----> make uImage for astro(iPM-T)"
    echo "build astro config      ----> make menuconfig for astro(iPM-T)"
    echo "build astro clean       ----> make clean for astro(iPM-T)"
    echo "build aed               ----> make uImage for aed"
    echo "build aed config        ----> make menuconfig for aed"
    echo "build aed clean         ----> make clean for aed"
    echo "build vs900             ----> make uImage for vs900"
    echo "build vs900 config      ----> make menuconfig for vs900"
    echo "build vs900 clean       ----> make clean for vs900"
    echo "build ecg               ----> make uImage for ecg"
    echo "build ecg config        ----> make menuconfig for ecg"
    echo "build ecg clean         ----> make clean for ecg"				
    exit 0
fi

if [ "$2" = "" ] ; then
    rm -f ${ITEM_PATH}/*
    cat ${CONFIG_PATH}/${ITEM_CONFIG} > ${NOW_CONFIG}
    if [ -f $ITEM_LOGO ] ; then
        cp ${ITEM_LOGO} ./drivers/video/logo/logo_linux_clut224.ppm
    fi
    make vmlinux ${CONFIG_ARCH}
    if [ $? -ne 0 ] ; then 
        exit -1;
    fi
	
    make modules ${CONFIG_ARCH}
    if [ -d $MOD_PATH ] ; then
		rm -rf $MOD_PATH/lib/modules
		make ${CONFIG_ARCH} INSTALL_MOD_PATH=$MOD_PATH modules_install
    else 
        cp ${WIFI_DRV_PATH}/bcm4329.ko ${WIFI_FS_PATH}
	fi

	make uImage ${CONFIG_ARCH}
    if [ $? -ne 0 ] ; then 
        exit -1;
    fi

	if [ -d $ROOTFS ] ; then
		tar cjvf $ROOTFS.tar.bz2 $ROOTFS
	fi

	if [ -d $MOD_PATH ] ; then
		if [ $MOD_CRAMFS ] ; then 
			rm $MOD_PATH/$KERNEL_VERSION*
			get_kernel_version $VERSION_SUFFIX
			echo "fetch kernel version is: $KERNEL_VERSION"
			touch $MOD_PATH/$KERNEL_VERSION
			./tools/cramfs/mkcramfs -v $MOD_PATH $MOD_CRAMFS
			mv $MOD_CRAMFS $MOD_CRAMFS.bin
		fi
		tar cjvf $MOD_PATH.tar.bz2 $MOD_PATH
	fi

    mkdir -p ${ITEM_PATH}
	
    if [ "$1" = "miracle" ] ; then
        cp ${UIMAGE_PATH}/uImage ${ITEM_PATH}/Kernel_IMEC_IPM.bin
    elif [ "$1" = "astro" ] ; then
        cp ${UIMAGE_PATH}/uImage ${ITEM_PATH}/Kernel_T1.bin
    elif [ "$1" = "aed" ] ; then
        cp ${UIMAGE_PATH}/uImage ${ITEM_PATH}/aed_kernel.bin
    elif [ "$1" = "vs900" ] ; then
        cp ${UIMAGE_PATH}/uImage ${ITEM_PATH}/vs900_kernel.bin
    elif [ "$1" = "ecg" ] ; then
        cp ${UIMAGE_PATH}/uImage ${ITEM_PATH}/ecg_kernel.bin
    fi

    cp ${UIMAGE_PATH}/Image ${ITEM_PATH}
    cp ./System.map ${ITEM_PATH}
    cp ./vmlinux ${ITEM_PATH}
elif [ "$2" = "modules" ] ; then
	make modules ${CONFIG_ARCH}	
    if [ -d $MOD_PATH ] ; then
		rm -rf $MOD_PATH/lib/modules
		make ${CONFIG_ARCH} INSTALL_MOD_PATH=$MOD_PATH modules_install
    else 
		cp ${WIFI_DRV_PATH}/bcm4329.ko ${WIFI_FS_PATH}
	fi
elif [ "$2" = "config" ] ; then
    cat ${CONFIG_PATH}/${ITEM_CONFIG} > ${NOW_CONFIG}
    make menuconfig ${CONFIG_ARCH}
    cat ${NOW_CONFIG} > ${CONFIG_PATH}/${ITEM_CONFIG}
elif [ "$2" = "clean" ] ; then
    cat ${CONFIG_PATH}/${ITEM_CONFIG} > ${NOW_CONFIG}
    make clean ${CONFIG_ARCH}
    if [ -d $MOD_PATH ] ; then
       	rm -rf $MOD_PATH/lib/modules
    fi
fi

