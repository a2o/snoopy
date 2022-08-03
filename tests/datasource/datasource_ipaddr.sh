#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource ipaddr`
VAL_REAL=`who -m --ips | awk '{print $5}'` || true
if [[ "$VAL_REAL" == "" ]]; then
    VAL_REAL="-"
fi



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
