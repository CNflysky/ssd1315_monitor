#!/bin/bash
msgfmt locales/zh_CN.po -o locales/ssd1315.mo
sudo mv locales/ssd1315.mo /usr/share/locale/zh_CN/LC_MESSAGES/ssd1315.mo