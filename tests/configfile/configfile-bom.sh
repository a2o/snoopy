#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_REAL="someRandomValue"

TMP_INI=$SNOOPY_CUR_TEST_FILENAME.ini
rm -f $TMP_INI

echo -e "\xEF\xBB\xBF[snoopy]
message_format = $VAL_REAL" > $TMP_INI

VAL_SNOOPY=`$SNOOPY_TEST_CLI run configfile "$TMP_INI" "message_format"`
rm -f $TMP_INI



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
