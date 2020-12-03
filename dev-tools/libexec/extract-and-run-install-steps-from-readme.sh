#!/bin/bash



### Configure shell
#
set -e
set -u
set -o pipefail
MYDIR=`dirname $0`



### Local or remote operation?
#
README_LOCATION="local"
if [ "${1:-}" == "-r" ]; then
    README_LOCATION="remote"
fi



### Extract the install steps
#
INSTALL_STEPS=""
if [ "$README_LOCATION" == "remote" ]; then
    SNOOPY_README_URL="https://raw.githubusercontent.com/a2o/snoopy/master/README.md"
    echo "Extracting steps from remore README.md at $SNOOPY_README_URL..."
    INSTALL_STEPS=`wget -O - $SNOOPY_README_URL | grep -E '## Installation' -A15 | grep -E '^\`\`\`shell$' -A15 | grep -E '^\`\`\`$' -B10 | grep -Ev '^\`\`\`' | sed -e 's/sudo //'`
else
    echo "Extracting steps from local README.md..."
    INSTALL_STEPS=`cat $MYDIR/../../README.md   | grep -E '## Installation' -A15 | grep -E '^\`\`\`shell$' -A15 | grep -E '^\`\`\`$' -B10 | grep -Ev '^\`\`\`' | sed -e 's/sudo //'`
    echo
fi
echo "Extracted install steps ('sudo' was removed, not needed for CI):"
echo "========================"
echo "$INSTALL_STEPS"
echo "========================"



### Run the install steps
#
eval $INSTALL_STEPS
