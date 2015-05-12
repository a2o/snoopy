#!/bin/bash



### Configure shell
#
set -e



### Settings
#
COVERITY_SETTINGS_FILE="./build/coverity-settings.sh.conf"
BUILD_DIR="cov-int"
BUILD_FILE_PREFIX="cov-int-snoopy"



### Help function
#
displayHelp()
{
    echo
    echo "To automatically submit build to Coverity, some environment variables must be defined:"
    echo
    echo "export COVERITY_EMAIL=\"\""
    echo "export COVERITY_TOKEN=\"\""
    echo "export COVERITY_BUILD_VERSION=\"\""
    echo
}



### Include Coverity settings file
#
if [ -f $COVERITY_SETTINGS_FILE ]; then
    echo "Reading Coverity settings file $COVERITY_SETTINGS_FILE"
    . $COVERITY_SETTINGS_FILE
else
    echo "Coverity settings file does not exist: $COVERITY_SETTINGS_FILE"
fi



### Check if all environmental variables are set
#
if [ "x$COVERITY_EMAIL" == "x" ]; then
    echo "ERROR: COVERITY_EMAIL is not set."
    displayHelp
    exit 1
fi
if [ "x$COVERITY_TOKEN" == "x" ]; then
    echo "ERROR: COVERITY_TOKEN is not set."
    displayHelp
    exit 1
fi
if [ "x$COVERITY_BUILD_VERSION" == "x" ]; then
    echo "ERROR: COVERITY_BUILD_VERSION is not set."
    displayHelp
    exit 1
fi
if ! which cov-build; then
    echo "ERROR: Unable to find cov-build."
    echo
    echo "export PATH=/path/to/cov-analysis-linux64-a.b.c/bin:\$PATH"
    echo
    displayHelp
    exit 1
fi



### Create build first
#

# Clean
./bootstrap.sh
./configure
make distclean

# Configure for real
./configure \
    --enable-config-file \
    --enable-filtering

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

cov-build \
    --dir=$BUILD_DIR \
    make

tar \
    -c -z \
    -f ${BUILD_FILE_PREFIX}-${COVERITY_BUILD_VERSION}.tar.gz \
    $BUILD_DIR



### Submit to Coverity
#
curl \
    --form email="$COVERITY_EMAIL" \
    --form token="$COVERITY_TOKEN" \
    --form version="$COVERITY_VERSION" \
    --form description="$COVERITY_DESCRIPTION" \
    --form file=@${BUILD_FILE_PREFIX}-${COVERITY_BUILD_VERSION}.tar.gz \
    https://scan.coverity.com/builds?project=Snoopy+Logger



### Tell the user what has been done
#
echo
echo "Submitted file to Coverity: ${BUILD_FILE_PREFIX}-${COVERITY_BUILD_VERSION}.tar.gz"
echo "All done."
echo
