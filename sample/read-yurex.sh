#!/bin/sh

for x in /dev/yurex*; do
	echo $x: `cat $x`
done

