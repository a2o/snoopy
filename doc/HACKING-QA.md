# Snoopy Hacking - QA

A short guide on how to run Snoopy code/builds through some of the common
Quality Assurance tools.

* [Test suite](#test-suite)
* [Autoscan](#autoscan)
* [Travis-CI](#travis-ci)
* [valgring](#valgrind)
* [coverity](#coverity)



## Test suite

Start the in-repository test suite like this:
```shell
./configure --enable-everything &&
make &&
make tests
```
If there are errors, each test case has a corresponding .log file right next to
it, where additional information about the error is (usually) available.



## Autoscan

The `autoscan` tool scans the local sources and generates `configure.scan` file
containing all detected dependelcies.


### Autoscan on GitHub Actions

For each PR and push to `master`, a GitHub action called `Autoscan` is triggered.
Check your PR or find relevant run(s) among Snoopy's [GitHub Actions](https://github.com/a2o/snoopy/actions).


### Autoscan steps

A1: Clean out the repository, or else autoscan picks up unrelated dependencies
from build/aux/ltmain.sh, namely AC_PROG_CPP, AC_PROG_CXX and AC_PROG_RANLIB.
The bootstrap+configure steps are present here to bring the repository into a
known-good state, or else `make gitclean` might simply not work:
```shell
./bootstrap.sh &&
./configure --enable-everything &&
make gitclean
```

A2: Run `autoscan` now:
```shell
autoscan
```

A3: The `configure.scan` file is already committed to the repository from the
previous `autoscan` run. This should make spotting newly-detected entries simple:
```shell
git diff
```

A4: If `git diff` points out any changes, you will most likely want to add them
to the `configure.ac` file.

A5: Once the above additions are done, update `config.h.in` with:
```shell
./bootstrap.sh
```

A6: Commit.



## GitHub Actions

GitHub Actions are used for ensuring build quality, by runing a build + test suite for
a matrix of build configuration combinations and on multiple Linux distributions.

To kick off these tests, all you need to do do is either:
- Create/update a pull request
- Push the code directly into the Snoopy upsteam repository (maintainers only)



## Valgrind

Valgrind is a local diagnostic tool that checks the binary for any memory and/or
file descriptor leaks.


### Valgrind on GitHub

Running Valgrind is configured as part of the test suite running on GitHub actions.
Go to your PR and find the related entry in the list of checks.


### Valgrind steps

Run the valgrind check as follows:
```shell
./configure --enable-everything &&
make valgrind
```



## Coverity

Coverity is a static code analysis tool.


### Coverity steps

Coverity analysis can be triggered by one of the following actions:
- Manually upload the build to Coverity
- GitHub Action - TODO

To manually upload the build to Coverity, the run the following:
```shell
./dev-tools/submit-to-coverity.sh
```

The Coverity scan results will be available [here](https://scan.coverity.com/projects/a2o-snoopy?tab=overview).
