#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
DATE_FORMAT="staticStringArgumentHere"
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource datetime "$DATE_FORMAT"`
snoopy_testRun_info "Snoopy value: $VAL_SNOOPY"
VAL_REAL=`date "+$DATE_FORMAT"`
snoopy_testRun_info "Real value:   $VAL_REAL"



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
