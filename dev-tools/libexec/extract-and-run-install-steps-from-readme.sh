#!/bin/bash



### Configure shell
#
set -e
set -u
set -o pipefail
MYDIR=`dirname $0`



### Extract the install steps
#
INSTALL_STEPS=`cat $MYDIR/../../README.md | grep -E '## Installation' -A10 | grep -E '^\`\`\`shell$' -A10 | grep -E '^\`\`\`$' -B10 | grep -Ev '^\`\`\`' | sed -e 's/sudo //'`
echo "Extracted install steps ('sudo' was removed, not needed for CI):"
echo "========================"
echo "$INSTALL_STEPS"
echo "========================"



### Run the install steps
#
eval $INSTALL_STEPS
