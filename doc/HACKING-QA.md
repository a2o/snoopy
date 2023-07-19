# Hacking QA - Snoopy Command Logger

TLDR version:
- Most of the code quality checks will run automatically (via GitHub Actions)
  on every PR and on every push to relevant branches. Find the automated
  runs' outputs on GitHub (in PR, in a commit or directly in
  the [Actions](https://github.com/a2o/snoopy/actions) tab.
- If you want to run these checks locally, find the steps below.

* [Test suite](#test-suite)
* [Autoscan](#autoscan)
* [Valgrind](#valgrind)
* [SonarCloud](#sonarcloud)
* [coverity](#coverity)



## Test suite

Runs automaticaly on GitHub, for:
- A matrix of common OSes
- A matrix of argument combinations to `./configure`

This is a suite of test cases for verifying sanity of Snoopy's basic functionality.


### Test suite - running manually

Start the in-repository test suite like this:
```shell
./configure --enable-everything &&
make &&
make tests
```
If there are errors, each test case has a corresponding `.log` file right next to
it, where additional information about the error is (usually) available.



## Autoscan

Runs automatically on GitHub.

The `autoscan` tool scans the local sources and generates `configure.scan` file
containing all detected dependencies.

NOTICE: Stock `autoscan` is generating some non-consitent results.
The wrapper script [../../dev-tools/autoscan.sh](dev-tools/autoscan.sh) will take care of those inconsistencies,
therefore it should be used instead.


### Autoscan - running manually

AS1: Clean out the repository, or else autoscan picks up unrelated dependencies
from build/aux/ltmain.sh, namely AC_PROG_CPP, AC_PROG_CXX and AC_PROG_RANLIB.
The bootstrap+configure steps are present here to bring the repository into a
known-good state, or else `make gitclean` might simply not work:
```shell
./bootstrap.sh &&
./configure --enable-everything &&
make gitclean
```

AS2: Run `./dev-tools/autoscan.sh` now:
```shell
./dev-tools/autoscan.sh
```

AS3: The `configure.scan` file is already committed to the repository from the
previous `./dev-tools/autoscan.sh` run. This should make spotting newly-detected entries simple:
```shell
git diff
```

AS4: If `git diff` points out any changes, you will most likely want to add them
to the `configure.ac` file.



## Valgrind

Runs automatically on GitHub.

Valgrind is a local diagnostic tool that checks the binary for any memory and/or
file descriptor leaks.


### Valgrind - running manually

Run the valgrind check as follows:
```shell
./configure --enable-everything &&
make valgrind
```



## Coverity

Exists as a GitHub workflow, but does **not** run automatically on GitHub.
Maintainers can manually trigger this workflow when required (i.e. before releasing a new stable version).

Coverity is a static code analysis tool.


### Coverity - running manually

To manually upload the build to Coverity, the run the following:
```shell
./dev-tools/submit-to-coverity.sh
```

The Coverity scan results will be available [here](https://scan.coverity.com/projects/a2o-snoopy?tab=overview).
