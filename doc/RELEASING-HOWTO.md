# Snoopy releasing guide



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


6. All done.
