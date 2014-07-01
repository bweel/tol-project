#!/bin/bash

if [ $# -ne 2 ]; then
	echo 'Usage: run-webots <number-of-webots-instance> <world-file>'
	exit 0;
fi

echo 'Running world file ' $2 ' with ' $1 ' webots at a time.'

one_run()
{
	date=`date +"%Y-%m-%d--%H.%M.%S"`
	outputfile=$1"-"$date".log"
	
	/Applications/Webots/webots --minimize --mode=fast --stdout --stderr $1 > $outputfile 2>&1 &
}

for ((i=1;i<=$1;i++))
do
	echo 'Starting webots instance: ' $i
	one_run $2
	sleep 10s;
done
