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


2. Edit the `README.md` file, update the latest release version (to the upcoming release version):

    edit README.md


3. Create a commit and tag:

    ./dev-tools/create-release-commit-and-tag.sh


4. Build the release package(s):

    ./dev-tools/build-package-targz.sh


5. Publish the release:

    ./dev-tools/publish-release.sh


5. Go to GitHub and close the corresponding milestone:

    https://github.com/a2o/snoopy/milestones


6. Go to Snoopy's [Actions](https://github.com/a2o/snoopy/actions) tab
   on GitHub and monitor the workflows related to the new release (hint: filter by git tag).


7. All done.
