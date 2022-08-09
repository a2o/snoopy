#!/bin/bash



### Shell configuration and error handler
#
set -e
set -u

_fatalError() {
    echo "ERROR($0): $1" 1>&2
    exit 1
}
_echo() {
    echo "[$0]: $1"
}



### Check the runtime environment
#
if [ ! -d .git ]; then
    _fatalError "This script must be run from the root of the git repository"
fi



### Get the release tag
#
RELEASE_TAG=`./dev-tools/libexec/get-release-tag.sh`
if [ "$RELEASE_TAG" == "" ]; then
    _fatalError "Unable to determine release tag, got: '$RELEASE_TAG'"
fi



### Check if release tag is properly formatted
#
if [[ ! "$RELEASE_TAG" =~ ^snoopy- ]]; then
    _fatalError "Release tag is not properly formatted - snoopy-* format is required"
fi
RELEASE_VERSION=`echo "$RELEASE_TAG" | sed -e 's/snoopy-//'`

# Construct RPM-related version strings
ARCH_PACKAGE_SNOOPY_VERSION=`echo "$RELEASE_VERSION" | sed -e 's/-/_/g'`
ARCH_PACKAGE_RELEASE_NUMBER="1"
ARCH_PACKAGE_RELEASE_TAG="${ARCH_PACKAGE_SNOOPY_VERSION}-${ARCH_PACKAGE_RELEASE_NUMBER}"
ARCHITECTURE=`uname -m`
ARCH_PACKAGE_FULL_TAG="${ARCH_PACKAGE_SNOOPY_VERSION}-${ARCH_PACKAGE_RELEASE_NUMBER}-$ARCHITECTURE"

_echo "Determined release tag:           $RELEASE_TAG"
_echo "Determined release version:       $RELEASE_VERSION"
_echo "Determined Arch package version:  $ARCH_PACKAGE_SNOOPY_VERSION"
_echo "Determined Arch package full tag: $ARCH_PACKAGE_FULL_TAG"



### If this is a production build, do additional checking
#
RELEASE_IS_STABLE="false"
if [[ $RELEASE_TAG =~ ^snoopy-[0-9]+\.[0-9]+\.[0-9]+(-rc[0-9]+)?$ ]]; then
    _echo "This is a stable production build, running additional consistency checks..."
    ./dev-tools/libexec/verify-last-version-everywhere.sh "$RELEASE_VERSION"
    RELEASE_IS_STABLE="true"
else
    _echo "This is a non-stable/non-production build, additional consistency checks will be skipped."
fi



### Does the target release package already exist?
#
RELEASE_PACKAGE_FILE="snoopy-${ARCH_PACKAGE_FULL_TAG}.pkg.tar.zst"
if [ -e $RELEASE_PACKAGE_FILE ]; then
    _fatalError "Release package file already exists: $RELEASE_PACKAGE_FILE (Hint: 'rm -f snoopy-*' or 'make clean')"
fi



### Create the release package
#
./bootstrap.sh
./configure --enable-everything
make gitclean

./bootstrap.sh

# Make gitclean removes this one, so let's (re?)create it
rm -f PKGBUILD
ln -s packaging/arch/PKGBUILD

# One of the steps above updates comments in config.h.in, rendering git tag "-dirty"
git checkout config.h.in

# Pull in some settings as environment variables
source packaging/arch/env

makepkg --holdver



### Cleanup
#
git checkout packaging/arch/PKGBUILD
rm -rf pkg/



### Report success
#
_echo ""
_echo "SUCCESS: Build complete."
_echo "SUCCESS: Result: $RELEASE_PACKAGE_FILE"
_echo ""



### Suggest next step(s)
#
if [ "$RELEASE_IS_STABLE" == "true" ]; then
    _echo ""
    _echo "Next step:"
    _echo "=========="
    _echo "Publish the release using the following command - FIXME:"
    _echo ""
    _echo "    ./dev-tools/publish-release.sh"
    _echo ""
else
    _echo ""
    _echo "Next step:"
    _echo "=========="
    _echo "This is a non-stable version, skipping the publishing suggestion."
    _echo ""
fi
