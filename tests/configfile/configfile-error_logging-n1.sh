#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
VAL_INI="n"
VAL_PARSED="n"

TMP_INI=$SNOOPY_CUR_TEST_FILENAME.ini
rm -f $TMP_INI

cat > $TMP_INI <<EOF
[snoopy]
error_logging = $VAL_INI
EOF

VAL_SNOOPY=`$SNOOPY_TEST_CLI run configfile "$TMP_INI" "error_logging"`
rm -f $TMP_INI



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_PARSED"
