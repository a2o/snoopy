#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_REAL="devlog"

TMP_INI=$SNOOPY_CUR_TEST_FILENAME.ini
rm -f $TMP_INI

cat > $TMP_INI <<EOF
[snoopy]
output = "$VAL_REAL"
EOF

VAL_SNOOPY=`$SNOOPY_TEST_CONFIGFILE "$TMP_INI" "output"`
rm -f $TMP_INI



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
