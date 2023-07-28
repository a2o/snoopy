#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
CONFIG_VARIABLE="$1"
VAL_CONF="$2"
if [ ! -z "${3:-}" ]; then
    VAL_EXPECTED="$3"
else
    VAL_EXPECTED="$VAL_CONF"
fi

TMP_INI=$SNOOPY_CUR_TEST_FILENAME.$CONFIG_VARIABLE.$$.ini
rm -f $TMP_INI

cat > $TMP_INI <<EOF
[snoopy]
$CONFIG_VARIABLE = "$VAL_CONF"
EOF

VAL_SNOOPY=`$SNOOPY_TEST_CLI run configfile "$TMP_INI" "$CONFIG_VARIABLE"`
rm -f $TMP_INI



### Evaluate
#
snoopy_test_compareValues "$VAL_SNOOPY" "$VAL_EXPECTED"
