#!/bin/sh

for x in /dev/yurex*; do
	echo L1 > $x
	echo A0 > $x
done
sleep 3
for x in /dev/yurex*; do
	echo L0 > $x
	echo R  > $x
	echo `cat $x` > $x
done
