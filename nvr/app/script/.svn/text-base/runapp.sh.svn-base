#! /bin/bash

if [ $1 = "stop" ]
then
	killall jpTds.sh
	./jpTds.sh stop &
else
	nohup ./jpTds.sh &
fi
