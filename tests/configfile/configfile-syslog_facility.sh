#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Set parameters
#
CONFIG_VARIABLE="syslog_facility"
RANDOM_NUMBER=$(( (RANDOM % 7) + 1))
VAL_REAL="LOCAL$RANDOM_NUMBER"



### Execute generic test
#
$SNOOPY_CUR_TEST_DIR/configfile-generic.sh "$CONFIG_VARIABLE" "$VAL_REAL"
