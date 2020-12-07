#!/bin/bash



### Configure shell
#
set -e
set -u
set -o pipefail



### Settings
#
SONAR_SETTINGS_FILE="./dev-tools/submit-to-sonarcloud.conf"
BUILD_WRAPPER_OUTPUT_DIR="sonarcloud-build-wrapper-output"



### Help function
#
displayHelp()
{
    cat <<EOF
To submit build to SonarCloud, some environment variables must be defined:

export SONAR_TOKEN=""

Alternatively you may store those settings in the following config file:

    $SONAR_SETTINGS_FILE

EOF
}



### Include Coverity settings file
#
if [ -f $SONAR_SETTINGS_FILE ]; then
    echo "Reading SonarCloud settings file $SONAR_SETTINGS_FILE"
    . $SONAR_SETTINGS_FILE
else
    echo "Coverity settings file does not exist: $SONAR_SETTINGS_FILE"
fi



### Check if all environmental variables are set
#
if [ "x$SONAR_TOKEN" == "x" ]; then
    echo "ERROR: SONAR_TOKEN is not set."
    displayHelp
    exit 1
fi

# Export, as we're not passing the token explicitly as an argument
export SONAR_TOKEN



### Check for the presence of required tools
#
if ! command -v build-wrapper-linux-x86-64 > /dev/null; then
    echo "ERROR: Unable to find 'build-wrapper-linux-x86-64'."
    exit 1
fi
if ! command -v sonar-scanner > /dev/null; then
    echo "ERROR: Unable to find 'sonar-scanner'."
    exit 1
fi



### Configure
#

# Clean
./bootstrap.sh
./configure --enable-everything
make gitclean

# Configure for real
./bootstrap.sh
./configure --enable-everything



### Build with SonarCloud wrapper
#
build-wrapper-linux-x86-64 \
    --out-dir $BUILD_WRAPPER_OUTPUT_DIR \
    make



### Analyze and submit
#
CURRENT_BRANCH_NAME=`git branch --show-current`
SONARCLOUD_TAG=`./dev-tools/libexec/get-sonarcloud-tag.sh`
sonar-scanner \
  -Dsonar.organization=a2o \
  -Dsonar.projectKey=snoopy \
  -Dsonar.sources=. \
  -Dsonar.branch.name=$CURRENT_BRANCH_NAME \
  -Dsonar.projectVersion=$SONARCLOUD_TAG \
  -Dsonar.cfamily.build-wrapper-output=$BUILD_WRAPPER_OUTPUT_DIR \
  -Dsonar.cfamily.threads=1 \
  -Dsonar.cfamily.cache.enabled=false \
  -Dsonar.host.url=https://sonarcloud.io
echo "INFO: Submission tag: $SONARCLOUD_TAG"
