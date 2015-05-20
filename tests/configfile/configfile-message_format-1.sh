#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_REAL="fake message format `date +%s`"

TMP_INI=$SNOOPY_CUR_TEST_FILENAME.ini
rm -f $TMP_INI

cat > $TMP_INI <<EOF
[snoopy]
message_format = "$VAL_REAL"
EOF

VAL_SNOOPY=`$SNOOPY_TEST_CONFIGFILE "$TMP_INI" "message_format"`
rm -f $TMP_INI



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
