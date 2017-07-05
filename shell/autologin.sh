#!/usr/bin/expect -f 

spawn ssh ubuntu@test
expect "*password:" 
send "ubuntu\r"
expect "$*" { send "su\r" }
expect "Password:" { send "root\r" }
expect "*#" { send "tmux\r" }
expect "*#" { send "cd setupKernel\r" }
interact
