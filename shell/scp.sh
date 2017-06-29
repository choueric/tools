#!/usr/bin/expect -f 

spawn scp $argv ubuntu@test:~/setupKernel
expect "*password:" 
send "ubuntu\r"
expect eof 
