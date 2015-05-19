#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE eusername`
MY_EUID="$EUID"
VAL_REAL=`getent passwd "$MY_EUID" | cut -d: -f1`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
