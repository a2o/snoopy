#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
MY_UID=`$SNOOPY_TEST_DATASOURCE uid`
MY_UID_PLUS_1=`expr $MY_UID + 1`
if ! $SNOOPY_TEST_FILTER   "msg"   "exclude_uid"   "$MY_UID_PLUS_1" > /dev/null; then
    snoopy_testResult_fail "My UID: $MY_UID"
else
    snoopy_testResult_pass
fi
