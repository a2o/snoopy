#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Output data
#
VAL_REAL=`date +%s`

MY_PID=$$
FILE_SOCKET="output_socket.sh.$MY_PID.socket"
FILE_OUT="output_socket.sh.$MY_PID.sock.out"

# Listen
rm -f $FILE_SOCKET
rm -f $FILE_OUT
touch $FILE_OUT
socat UNIX-LISTEN:$FILE_SOCKET OPEN:$FILE_OUT &
SOCAT_PID=$!

# Sleep a bit, if socket file, or sometimes this fails as socket is not yet present
if [ ! -e $FILE_SOCKET ]; then
    sleep 0.2
elif [ ! -S $FILE_SOCKET ]; then
    sleep 0.5
elif [ ! -S $FILE_SOCKET ]; then
    sleep 2
fi

# Send content to this socket
echo "$VAL_REAL" | socat - UNIX-CONNECT:$FILE_SOCKET

# Kill the listener if it is still alive by accident
#wait $SOCAT_PID
PS_COUNT=`ps -p 1,$SOCAT_PID -o pid,cmd | grep -E -c '^[ ]*[0-9]+[ ]+'`
if [ "$PS_COUNT" -gt "2" ]; then
    kill -9 $SOCAT_PID
fi



# Read what has been written
VAL_SNOOPY=`cat $FILE_OUT`
rm -f $FILE_OUT



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
