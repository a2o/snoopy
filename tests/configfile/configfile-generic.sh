#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
CONFIG_VARIABLE="$1"
VAL_REAL="$2"

TMP_INI=$SNOOPY_CUR_TEST_FILENAME.$CONFIG_VARIABLE.$$.ini
rm -f $TMP_INI

cat > $TMP_INI <<EOF
[snoopy]
$CONFIG_VARIABLE = "$VAL_REAL"
EOF

VAL_SNOOPY=`$SNOOPY_TEST_CONFIGFILE "$TMP_INI" "$CONFIG_VARIABLE"`
rm -f $TMP_INI



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_REAL"
