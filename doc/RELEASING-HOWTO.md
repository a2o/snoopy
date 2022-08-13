# Snoopy releasing guide



## Changing the release process / Editing this document

This document contains the overall releasing process descrtiption, but hints
for what to do after each step are scattered around the related scripts in the
[dev-tools/](../dev-tools/) directory, to speed up the process.

Therefore, when changing the release process and documenting the changes in
this file, replicate the changes into the `Next step(s):` sections at the end of
each releasing-related script in the [dev-tools/](../dev-tools/) directory.



## (Before the release) QA

Run through the Quality Assurance-related steps in the [HACKING-QA.md](HACKING-QA.md) document
and work out any kinks that get pointed out.



## (Before the release) Assumptions

The following assumptions are, well, assumed, by the releasing scripts:
- `master` branch is used for releasing.
- All the code that is to be released is already merged into the `master` branch.
- The final release-related updates to `ChangeLog` and `README.md` files are already
  present in the local files, **but not yet committed**.
- The release commit does not yet exist.
- The release tag does not yet exist.
- The release does not exist yet on GitHub.



## Steps to release Snoopy

1. Edit the `ChangeLog` file, update the upcoming release version and date:

    edit ChangeLog


2. Edit the `README.md` file, update:
- The latest shown release version
- The latest release version in URLs right next to the edit above
- Add the latest release to the `News` section:

    edit README.md


3. Create a commit and tag:

    ./dev-tools/create-release-commit-and-tag.sh


4. Build the release package:

    ./dev-tools/build-release-package.sh


5. Publish the release:

    ./dev-tools/publish-release.sh


6. Go to GitHub and close the corresponding milestone:

    https://github.com/a2o/snoopy/milestones


7. Go to Snoopy's [Actions](https://github.com/a2o/snoopy/actions) tab
   on GitHub and monitor the workflows related to the new release:

    https://github.com/a2o/snoopy/actions?query=branch%3A$RELEASE_TAG


8. Trigger the native packaging build jobs:

    gh workflow run -R $GIT_REMOTE_GITHUB_ID publish-native-packages.yml --ref=$RELEASE_TAG


9. Monitor the native packaging build jobs:

    https://github.com/a2o/snoopy/actions/workflows/publish-native-packages.yml?query=branch%3A$RELEASE_TAG


10. All done.
