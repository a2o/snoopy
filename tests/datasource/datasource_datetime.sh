#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_SNOOPY=`$SNOOPY_TEST_CLI run datasource datetime`
snoopy_testRun_info "Snoopy value: $VAL_SNOOPY"
VAL_REAL=`date "+%Y-%m-%dT%H:%M:%S%z"`
snoopy_testRun_info "Real value:   $VAL_REAL"

# Busybox-based `date` utility does not recognize "yyyy-mm-ddThh:mm:ss+zzzz" format,
# so let's convert it to "yyyy-mm-dd hh:mm:ss" format for a more universal handling.
VAL_SNOOPY=`echo "$VAL_SNOOPY" | cut -d+ -f1 | sed -e 's/T/ /'`
VAL_REAL=`echo "$VAL_REAL"     | cut -d+ -f1 | sed -e 's/T/ /'`

VAL_SNOOPY_TS=`date --date="$VAL_SNOOPY"  "+%s"`
VAL_REAL_TS=`  date --date="$VAL_REAL"    "+%s"`
VAL_DIFF=`expr $VAL_SNOOPY_TS - $VAL_REAL_TS | sed -e 's/^-//'`
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
