Checklists for Pull requests
----------------------------

About pull request itself:
- [X] I am submitting a pull request! :)
- [ ] My submission does one logical thing only (one bugfix, one new feature). If I will want to supply multiple logical changes, I will submit multiple pull requests.
- [ ] I have read and understood the [CONTRIBUTING guide](https://github.com/a2o/snoopy/blob/master/.github/CONTRIBUTING.md)

Code quality:
(not applicable for non-code fixes)
- [ ] My submission is passing the test suite run (./configure --enable-everything && make tests) - test suite reports zero unexpected failures.

Commits:
- [ ] My commits are logical, easily readable, and with concise comments.
- [ ] My commits follow the KISS principle: do one thing, and do it well.

Licensing:
- [ ] I am the author of this submission or I have been authorized by the submission copyright holder to issue this pull request. By issuing this pull request the copyright holder agrees that their contribution is included in Snoopy and released under the current Snoopy license (currently GPLv2).

Branching:
- [ ] My submission is based on `master` branch.
- [ ] My submission is compatible with the latest `master` branch (no conflicts, I did a rebase if it was necessary).
- [ ] The name of the branch I want to merge upstream is not `master`.
- [ ] My branch name is `feature/my-shiny-new-snoopy-feature-title` (for new features).
- [ ] My branch name is `bugfix/my-totally-non-hackish-workaround` (for bugfixes).
- [ ] My branch name is `doc/what-i-did-to-documentation` (for documentation updates).

Continuous integration:
- [ ] Once I submit this pull request, I will wait for a CI report (normally done in a few minutes) and fix any issues CI points out.



Pull request description
------------------------
<DESCRIBE YOUR PULL REQUEST HERE>
