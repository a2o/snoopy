#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_REAL="100"
#$SNOOPY_TEST_THREADS  $VAL_REAL
#VAL_SNOOPY=`$SNOOPY_TEST_THREADS  $VAL_REAL| grep '^SUCCESS' | grep -Eo '[0-9]+$'`
$SNOOPY_TEST_THREADS  $VAL_REAL



### Evaluate
#
#snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
