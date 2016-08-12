#! /bin/bash

CURRENT_DIR=`pwd`
echo "======================================================="
echo "@@@@@@@@@@@@@@@@@@@@@@ start tds @@@@@@@@@@@@@@@@@@@@@@"
echo ""

DABIGATE_DIR="${CURRENT_DIR}/../dabigate"
FWDSERVER_DIR="${CURRENT_DIR}/../forwardserver"
TRAPAPP_DIR="${CURRENT_DIR}/../trapapp"

function CheckApp()
{
	APP_NAME=$1
	echo "++++++++++ Check $APP_NAME +++++++++"

	for STR in `ps -el | grep $APP_NAME`
	do
		if [ $STR = "$APP_NAME" ]
		then
			return 0
		fi
	done

	return 1
}

RUNNING="TRUE"
while [ "$RUNNING" = "TRUE" ]
do
	if [ "$1" = "stop" ]
	then
		killall dabigate
		killall fwdserver
		killall trapapp
		RUNNING="FALSE"
	fi

	if [ $RUNNING = "TRUE" ]
	then
		if CheckApp fwdserver
		then
			echo "fwdserver is running success"
		else
			echo "$FWDSERVER_DIR/fwdserver"
			cd $FWDSERVER_DIR
			$FWDSERVER_DIR/fwdserver > /dev/null
		fi

		sleep 3

		if CheckApp dabigate
		then
        		echo "dabigate is running success"
		else
			echo "$DABIGATE_DIR/dabigate"
			cd $DABIGATE_DIR
        		$DABIGATE_DIR/dabigate > /dev/null
		fi

		sleep 3
	
		if CheckApp trapapp
        	then
                	echo "trapapp is running success"
        	else
                	echo "$TRAPAPP_DIR"
                	cd $TRAPAPP_DIR
                	$TRAPAPP_DIR/trapapp > /dev/null
        	fi
	fi
done
