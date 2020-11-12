#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
# Read the comment in configfile-line-length-1023.sh file first.
#
# Here, we increase the message size by 1 characters, and this test
# should the fail because the trailing character (double quote) does
# not end up in the internal string buffer, and the inih parser then
# fails to remove enclosing quotes, resulting in an incorrect configuration
# value (contains opening double quote, missing last character of the value)..
#
VAL_REAL="This config line is exactly 1024 bytes long, and this config value is exactly 1005 bytes long............................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................1005"

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
