#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Set parameters
#
CONFIG_VARIABLE="log_message_max_length"
VAL_REAL="1512"



### Execute generic test
#
$SNOOPY_CUR_TEST_DIR/configfile-generic.sh "$CONFIG_VARIABLE" "$VAL_REAL"
