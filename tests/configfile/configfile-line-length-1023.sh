#!/bin/bash



### Configure shell and bootstrap
#
set -e
set -u
. `dirname $BASH_SOURCE`/_bootstrap.sh



### Get data
#
# The (updated) configured line length limit in inih library is 1024 bytes,
# but this must include the trailing null character.
# This leaves 1023 bytes available for the whole single line of content.
# In the example below, the 'message_format = ""' part is 19 characters long,
# leaving 1004 bytes for the actual content of the value.
#
VAL_REAL="This config line is exactly 1023 bytes long, and this config value is exactly 1004 bytes long...........................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................1004"

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
