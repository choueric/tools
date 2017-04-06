#!/bin/sh

$@

ret=$?
cmd="$@"

if [ "$ret" != "0" ]; then
	notify-send ""${cmd}" failed!"
else
	notify-send "${cmd} success."
fi

return $ret
