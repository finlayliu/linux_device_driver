#########################################################################
# File Name: build.sh
# Author: finlay
# mail: liufy.thu@gmail.com
# Created Time: Mon 01 Sep 2014 12:31:30 AM CST
#########################################################################
#!/bin/bash
sudo rm -rf hello.ko
sudo make -f origin.mk
sudo dmesg -c
echo ################################
echo local prints!!!!
echo ################################
sudo insmod hello.ko
sudo rmmod hello
sudo dmesg -c
echo ################################
echo prints ended!!!!!
sudo make clean
