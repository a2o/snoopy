#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
DATE_FORMAT="1-------10--------20--------30--------40--------50--------60--------70---75%s" # should exceed 80 character limit and fail
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource datetime "$DATE_FORMAT"`
snoopy_testRun_info "Snoopy value: $VAL_SNOOPY"
VAL_REAL=`date "+$DATE_FORMAT"`
snoopy_testRun_info "Real value:   $VAL_REAL"



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
