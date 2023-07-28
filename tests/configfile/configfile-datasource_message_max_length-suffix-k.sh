#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Set parameters
#
CONFIG_VARIABLE="datasource_message_max_length"
VAL_CONF="7k"
VAL_EXPECTED="7168"



### Execute generic test
#
$SNOOPY_CUR_TEST_DIR/configfile-generic.sh "$CONFIG_VARIABLE" "$VAL_CONF" "$VAL_EXPECTED"
