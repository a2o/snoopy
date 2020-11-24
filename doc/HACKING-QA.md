# Snoopy Hacking - QA

A short guide on how to run Snoopy code/builds through some of the common
Quality Assurance tools.

* [Autoscan](#autoscan)
* [Travis-CI](#travis-ci)
* [valgring](#valgrind)
* [coverity](#coverity)



## Autoscan

The `autoscan` tool scans the local sources and generates `configure.scan` file
containing all detected dependelcies.



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



## Travis-CI

Travis-CI is used for ensuring build quality, by runing a build + test suite for
a matrix of build configuratio combinations.

### Travis-CI steps

To kick off Travis-CI, all you need to do do is push the code into the Snoopy
upsteam repository and wait for the results. Travis-CI builds are triggered by
pushing to the `master` branch, or by issuing a PR agains Snoopy repository.



## Valgrind

Valgrind is a local diagnostic tool that checks the binary for any memory and/or
file descriptor leaks.

### Valgrind steps

Run the valgrind check as follows:
```shell
./configure --enable-everything &&
make valgrind
```



## Coverity

Coverity is a static code analysis tool.

### Coverity steps

Coverity analysis can be triggered by one of the following two actions:

- Push the code into a branch called `coverity_scan` in Snoopy's GitHub repository.
- Manually upload the build to Coverity.

To manually upload the build to Coverity, the run the following:
```shell
./configure --enable-everything &&
./dev-tools/submit-to-coverity.sh
```

The Coverity scan results will be available [here](https://scan.coverity.com/projects/a2o-snoopy?tab=overview).
