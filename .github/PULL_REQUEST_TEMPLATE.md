Checklists for Pull requests
----------------------------

About pull request itself:
- [X] I am submitting a pull request:)
- [ ] My submission does one logical thing only (one bugfix, one new feature). If I will want to supply multiple logical changes, I will submit multiple pull requests.
- [ ] I have read and understood CONTRIBUTING guide @ https://github.com/a2o/snoopy/blob/master/.github/CONTRIBUTING.md

Code quality:
(not applicable for non-code fixes of course)
- [ ] My submission is passing test suite run (./configure --enable-everything && make tests) - test suite reports zero unexpected failures.
- [ ] My submission is passing Valgrind check (./configure --enable-everything --enable-dev-tools && make valgrind) - Valgrind reports "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)".

Commits:
- [ ] My commits are logical, easily readable, with concise comments.
- [ ] My commits follow the KISS principle: do one thing, and do it well.

Licensing:
- [ ] I am the author of submission or have been authorized by submission copyright holder to issue this pull request.

Branching:
- [ ] My submission is based on master branch.
- [ ] My submission is compatible with latest master branch updates (no conflicts, I did a rebase if it was necessary).
- [ ] The name of the branch I want to merge upstream is not 'master' (except for only the most trivial fixes, like typos and such).
- [ ] My branch name is *feature/my-shiny-new-snoopy-feature-title* (for new features).
- [ ] My branch name is *bugfix/my-totally-non-hackish-workaround* (for bugfixes).
- [ ] My branch name is *doc/what-i-did-to-documentation* (for documentation updates).

Continuous integration:
- [ ] Once I will submit this pull request, I will wait for Travis-CI report (normally a couple of minutes) and fix any issues I might have introduced.



Pull request description
------------------------
