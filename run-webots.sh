#!/bin/bash

if [ $# -ne 2 ]; then
	echo 'Usage: run-webots <number-of-webots-instance> <world-file>'
	exit 0;
fi

echo 'Running world file ' $2 ' with ' $1 ' webots at a time.'

for ((i=1;i<=$1;i++))
do
	echo 'Starting webots instance: ' $i
	/Applications/Webots/webots --minimize --mode=fast $2 &
	sleep 5s;
done