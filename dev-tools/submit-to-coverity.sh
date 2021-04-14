#!/bin/bash



### Configure shell
#
set -e
set -u
set -o pipefail



### Settings
#
COVERITY_SETTINGS_FILE="./dev-tools/submit-to-coverity.conf"
BUILD_DIR="cov-int"   # Can't change the name of this one, or else Coverity build analysis fails once the build tarball is uploaded
BUILD_FILE_PREFIX="cov-int"



### Help function
#
displayHelp()
{
    cat <<EOF
To automatically submit build to Coverity, some environment variables must
be defined:

export COVERITY_EMAIL=""
export COVERITY_TOKEN=""
export COVERITY_BUILD_VERSION=""
export COVERITY_BUILD_DESCRIPTION=""   # This one is optional

Alternatively you may store those settings in the following config
file:

    $COVERITY_SETTINGS_FILE

EOF
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


if [ -n "${COVERITY_BUILD_DESCRIPTION+1}" ]; then
    true
else
    export COVERITY_BUILD_DESCRIPTION="$COVERITY_BUILD_VERSION"
fi


if ! command -v cov-build > /dev/null; then
    echo
    echo "ERROR: Unable to find cov-build."
    echo
    echo "export PATH=/path/to/cov-analysis-linux64-a.b.c/bin:\$PATH"
    echo
    exit 1
fi
if ! command -v curl > /dev/null; then
    echo
    echo "ERROR: Unable to find CURL. Please install it and rerun the process."
    echo
    exit 1
fi



### Display stuff
#
cat <<EOF

Starting build for Coverity analysis.
Settings:
            email: $COVERITY_EMAIL
    build version: $COVERITY_BUILD_VERSION
            token: (redacted)

EOF



### Create build first
#

# Clean
./bootstrap.sh
./configure --enable-everything
make gitclean

# Configure for real
./bootstrap.sh
./configure --enable-everything

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
curl -f \
    --form project=a2o%2Fsnoopy \
    --form token="$COVERITY_TOKEN" \
    --form email="$COVERITY_EMAIL" \
    --form version="$COVERITY_BUILD_VERSION" \
    --form description="$COVERITY_BUILD_DESCRIPTION" \
    --form file=@${BUILD_FILE_PREFIX}-${COVERITY_BUILD_VERSION}.tar.gz \
    https://scan.coverity.com/builds?project=a2o%2Fsnoopy



### Tell the user what has been done
#
echo
echo "Submitted file to Coverity: ${BUILD_FILE_PREFIX}-${COVERITY_BUILD_VERSION}.tar.gz"
echo "All done."
echo
