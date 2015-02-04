#########################################################################
# File Name: build.sh
# Author: finlay
# mail: liufy.thu@gmail.com
# Created Time: Mon 01 Sep 2014 12:31:30 AM CST
#########################################################################
#!/bin/bash
sudo dmesg -c
sudo insmod hello.ko
sudo rmmod hello
sudo dmesg -c
