#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. ./_bootstrap.sh



### Get data from Snoopy
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE username`

if ! tty -s; then
    VAL_REAL=""
else
    MY_TTY=`tty | sed -e 's#/dev/##'`
    VAL_REAL=`last -n1 $MY_TTY | grep 'still logged in' | awk '{print $1}'`
fi

if [ "$VAL_REAL" == "" ]; then
    VAL_REAL=${SUDO_USER:-}
fi
if [ "$VAL_REAL" == "" ]; then
    VAL_REAL=${LOGNAME:-}
fi
if [ "$VAL_REAL" == "" ]; then
    VAL_REAL="(unknown)"
fi



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
