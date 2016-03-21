#!/bin/bash
# color-echo.sh: 用彩色来显示文本.

# ============================
# | 色彩    | 前景色 | 背景色|
# |  黑 	|  30	 |  40   |
# |  红 	|  31	 |  41   |
# |  绿 	|  32	 |  42   |
# |  黄 	|  33	 |  43   |
# |  蓝 	|  34	 |  44   |
# |  洋红	|  35	 |  45   |
# |  青 	|  36	 |  46   |
# |  白 	|  37	 |  47   |
# ============================

# 依照需要修改这个脚本.
# 这比手写彩色的代码更容易一些.

black='\E[30;1m'
yellow='\E[33;1m'
blue='\E[34;1m'
magenta='\E[35;1m'
cyan='\E[36;47m'

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


# 参数 $1 = 要显示的信息
# 参数 $2 = 颜色
cecho ()
{
	message=${1:-$default_msg}   # 默认的信息.
	color=${2:-$black}           # 如果没有指定,默认使用黑色.

	echo -e "$color$message\E[0;m"

	return
}


# ----------------------------------------------------
cecho "Feeling blue..." $blue
cecho "Magenta looks more like purple." $magenta
cecho "Green with envy." $green
cecho "Seeing red?" $red
cecho "Cyan, more familiarly known as aqua." $cyan
# ----------------------------------------------------

info "this is a info"
good "this is a good"
err "this is a error"
