#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $0`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE domain`
VAL_REAL=`hostname -d`



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
