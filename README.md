# rgrp
Multiline Log Grep tool

to Build, first make sure you have libpcre3-dev installed, e.g.
sudo apt-get install libpcre3-dev

then compile with:

make


if that isn't working for you for whatever reason, you can just do the compile without make with:
gcc -o rgrp rgrp.c -lpcre
