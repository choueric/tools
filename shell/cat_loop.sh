#!/bin/sh

set -e

FILES="vout1_clk
vout1_g_y_yc0
vout1_g_y_yc1
vout1_g_y_yc2
vout1_g_y_yc3
vout1_g_y_yc4
vout1_g_y_yc5
vout1_g_y_yc6
vout1_g_y_yc7
vout1_g_y_yc8
vout1_g_y_yc9
vout1_b_cb_c0
vout1_b_cb_c1
vout1_b_cb_c2
vout1_b_cb_c3
vout1_b_cb_c4
vout1_b_cb_c5
vout1_b_cb_c6
vout1_b_cb_c7
vout1_b_cb_c8
vout1_b_cb_c9
vout1_hsync
vout1_vsync
vout1_avid
"

debugfs=`mount | grep debugfs`
if [ "$debugfs" != "" ]; then
    mount -t debugfs none /mnt
    echo "y"
else
    echo "no"
fi

cd /mnt/omap_mux

for file in $FILES
do
    cat $file | grep name | awk '{print $2}'
done
