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
RPM_PACKAGE_SNOOPY_VERSION=`echo "$RELEASE_VERSION" | sed -e 's/-/_/g'`
RPM_PACKAGE_RELEASE_NUMBER="1"
RPM_PACKAGE_DIST_TAG=`rpm --eval '%{?dist}'`
RPM_PACKAGE_RELEASE_TAG="${RPM_PACKAGE_RELEASE_NUMBER}${RPM_PACKAGE_DIST_TAG}"
ARCH=`rpm --eval '%{_arch}'`
RPM_PACKAGE_FULL_TAG="${RPM_PACKAGE_SNOOPY_VERSION}-${RPM_PACKAGE_RELEASE_TAG}.$ARCH"

_echo "Determined release tag:          $RELEASE_TAG"
_echo "Determined release version:      $RELEASE_VERSION"
_echo "Determined RPM package version:  $RPM_PACKAGE_SNOOPY_VERSION"
_echo "Determined RPM package full tag: $RPM_PACKAGE_FULL_TAG"



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
RELEASE_PACKAGE_FILE="packaging/rpmbuild/RPMS/$ARCH/snoopy-${RPM_PACKAGE_FULL_TAG}.rpm"
if [ -e $RELEASE_PACKAGE_FILE ]; then
    _fatalError "Release package file already exists: $RELEASE_PACKAGE_FILE (Hint: 'rm -rf packaging/rpmbuild' or 'make clean')"
fi



### Create the release package
#
./bootstrap.sh
./configure --enable-everything
make gitclean

./bootstrap.sh
rpmbuild \
    --define "snoopy_rpm_package_version ${RPM_PACKAGE_SNOOPY_VERSION}" \
    --define "snoopy_rpm_release_tag ${RPM_PACKAGE_RELEASE_TAG}" \
    --define "_topdir $PWD/packaging/rpmbuild" \
    --define '_configure ../../../configure' \
    -bb packaging/rpm/snoopy.spec



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
