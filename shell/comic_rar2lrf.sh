#!/usr/bin/env bash

# 遍历目录下的所有特定类型的文件（通过后缀名），
# 然后执行定义的操作
# 这里进行漫画转化，使用的命令是comic2lrf

# 使用正则表达式和rename进行批量重命名
# rename -v 's/\[(comic)\]\[海賦王\]\[(One).(Piece)\]\[小良HMM版\]\[尾田榮一郎\].Vol.(\d+).rar/$1.$2_$3.$4.rar/' *.rar
# 可能需要针对不同的文件名格式进行相应的修改规则


SUFFIX=rar            # 所要寻找的文件类型的后缀名
NEWSUFFIX=cbz         # 操作后的新文件类型后缀名
REG='s/\[(comic)\]\[海賦王\]\[(One).(Piece)\]\[小良HMM版\]\[尾田榮一郎\].Vol.(\d+).rar/$1.$2_$3.$4.rar/'
#REG='s/.*(\d+).rar/comic.one_piece.$1.rar/'
CONVERT=./new_comic2lrf.py


# 检查必要的程序、文件是否存在
if test -e $CONVERT
then
	echo "Using $CONVERT to convert"
else
	echo "!!! $CONVERT not found !!!"
	exit 1
fi

# 脚本参数之一：选择处理的目录
# 可以为空，则默认为当前目录
if [ -n "$1" ]
then
	dir=$1   # 如果目录名作为第一个参数给出
	if test -e $dir
	then
		echo "Working dir is $dir"
	else
		echo "!!! Input dir: \"$dir\" dosn't existed !!!"
		exit 1
	fi
else
	dir=$PWD # 否则使用当前的工作目录
fi


# 重命名
cd $dir
rename -v $REG *.$SUFFIX
cd -

# 进行操作
for file in $dir/*.$SUFFIX
do
	if test -e $file
	then
		# 提取文件名
		filename=`basename $file`
		name=`basename $file .$SUFFIX`
		echo "Dealing \"$filename\" \"$name\"......"
		echo "--------------------------------------------------------------"
		# 解压
		echo "$filename ---> unrar ......."
		unrar e -inul $file $dir/$name/
		# 删除原始压缩包
		rm -rf $file
		# 压缩
		echo "$filename ---> zip ......"
		zip $dir/$name.zip $dir/$name/ -r > /dev/null
		mv $dir/$name.zip $dir/$name.cbz
		# 删除临时目录
		rm -rf $dir/$name
		# 进行转换
		echo "$filename ---> comic2lrf ......"
		$CONVERT $dir/$name.cbz -o $dir$name.lrf
		# 删除临时zip文件
		rm -rf $dir/$name.cbz
	fi
done

echo "Done"
exit 0
