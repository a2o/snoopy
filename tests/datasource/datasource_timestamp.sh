#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_DATASOURCE timestamp`
snoopy_testRun_info "Snoopy value: $VAL_SNOOPY"
VAL_REAL=`date +"%s"`
snoopy_testRun_info "Real value:   $VAL_REAL"
VAL_DIFF=`expr $VAL_SNOOPY - $VAL_REAL | sed -e 's/^-//'`
snoopy_testRun_info "Difference:   $VAL_DIFF"



### Evaluate
#
# We allow 3 seconds of difference, just in case.
#
if [ "$VAL_DIFF" -le "3" ]; then
    snoopy_testResult_pass
else
    snoopy_testResult_fail "Too much difference (diff=\"$VAL_DIFF\", snoopy=\"$VAL_SNOOPY\", real=\"$VAL_REAL\")"
fi
