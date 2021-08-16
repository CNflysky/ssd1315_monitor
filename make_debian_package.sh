#!/bin/bash
if [ ! -n "$1" ]
then
    echo "Usage:$0 [conf file]"
    echo "e.g. $0 rpi4b.conf"
    exit
fi
make 
mkdir -p package/lib/systemd/system/
mkdir -p package/usr/local/etc/ssd1315/
mkdir -p package/usr/share/locale/zh_CN/LC_MESSAGES/ssd1315.mo
mkdir -p package/usr/local/bin
mv ssd1315 package/usr/local/bin
msgfmt locales/zh_CN.po -o package/usr/share/locale/zh_CN/LC_MESSAGES/ssd1315.mo
cp ssd1315.service package/lib/systemd/system/ssd1315.service
cp config/$1 package/usr/local/etc/ssd1315/config.conf
dpkg -b package ssd1315.deb
make clean
rm -rf package/usr/local/bin/*
rm -rf package/usr/local/etc/ssd1315/*
rm -rf package/lib/systemd/system/*
rm -rf package/usr/share/locale/zh_CN/LC_MESSAGES/*
