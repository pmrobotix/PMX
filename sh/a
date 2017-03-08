#!/usr/bin/expect -f 

set force_conservative 1; 
set timeout -1
set cmd [lindex $argv 0]

spawn ./pmx-connect $cmd 

expect {
password: {
send "pmx\r";
#exp_continue
interact
}
}


