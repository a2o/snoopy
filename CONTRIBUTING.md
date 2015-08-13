# Contributing to Snoopy development

[![Flattr Snoopy Logger project](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=a2o&url=https://github.com/a2o/snoopy&title=Snoopy Logger)

New ideas are welcome. Most of change requests so far were about additional
log data or filtering capabilities, therefore most of development/changes
is expected in that area.



Table of Contents
=================

  * [Contributing to Snoopy development](#contributing-to-snoopy-development)
    * [OS for development](#os-for-development)
    * [Git branches - persistent](#git-branches---persistent)
    * [Git branches - transient](#git-branches---transient)
    * [Data source development](#data-source-development)
    * [Filter development](#filter-development)
    * [Output development](#output-development)
    * [Useful helpers](#useful-helpers)
    * [Creating feature documentation](#creating-feature-documentation)
    * [Creating pull requests for upstream merges](#creating-pull-requests-for-upstream-merges)



## OS for development

Development of Snoopy is (and should) be done on fairly recent OS releases.
Ubuntu 14.04 would be a good choice ATM, or RedHat/CentOS 7.

Required versions of software:
- automake 1.11
- autoconf 2.69

With a few tweaks Snoopy can be built from git repository on older systems too.
Just run ./bootstrap.sh and fix the issues that crop up. Patches/pull requests
that "fix" build process on these obsolete OSes will be rejected.



## Clone git repository

File [INSTALL.md](INSTALL.md) contains detailed instructions about git repository
cloning and git submodule initialization. Also, do not forget to run ./bootstrap.sh
after submodules have been updated.

Recap of the steps:
```shell
git clone https://github.com/a2o/snoopy.git
cd snoopy
git submodule init
git submodule update
./bootstrap.sh

```



## Git branches - persistent

The following git branches are always available in main Snoopy repository:

- master: contains latest Snoopy development efforts. All new features
    snould be based on this branch.
- stable: present when master branch has sufficiently drifted from latest
    stable release, but is still far away from becoming stable. Used
    for quick bugfixes and stable release maintenance.
- install: branch dedicated to Snoopy automated install procedure. When
    you use it, install-snoopy.sh script is downloaded from this
    branch.
- coverity_scan: when Coverity analysis of a build is required, it is
    performed using this branch. Usually it lazily follows master
    branch. Coverity has a limited build submission frequency, thus
    a dedicated branch has been created to trigger analysis on
    as-needed basis.
- legacy: orphaned branch, which contains imported Snoopy releases from
    1.00 to 1.6.1 + changes to bring it in sync with initial commit
    from master branch. This is a lousy attempt to recreate lost
    SVN history (commits 1-31). Present for historical completeness
    only.


## Git branches - transient

The following branch-naming patterns are intended for branches whose sole
purpose of existence is pull request submission. These branches should have
concise naming. Main reason for this is clean git history. When one-line-per-
commit history view shows "Merge branch 'contributor/master'" our dear history
viewer has no idea what is going on there. But, if instead of that, the first
line of that commit message is "Merge branch 'bugfix/invalid-data-from-ttyname'"
then a single glance is enough to understand the big picture.

Thus pull requests should only be made for concise-named branches that follow
these patterns:
- bugfix/*:        pull request branch, contains one bugfix,
- documentation/*: pull request branch, contains documentation work,
- enhancement/*:   pull request branch, contains one enhancement (not a new feature, but improvement nonetheless)
- feature/*:       pull request branch, contains a new feature,
- refactoring/*:   pull request branch, contains code refactoring,



### On what branch should you base your contribution to Snoopy?

**As a general rule, base your contribution on MASTER branch.**

Exception:
- you found a bug that impacts current stable release - base your work on
    **stable** branch, if it is currently present in Snoopy repo. Maintainer
    will merge it into master branch.

Non-exception:
- you have developed a new feature and would like it to land in the next bugfix
    release: **do not bother**. Bugfix releases are bugfix releases, they are
    not intended to introduce new features. Base your work on **master** branch.
    If you somehow manage to convince maintainer(s) that your feature should
    land in next bugfix release anyway, maintainer will do the integration.



## Data source development

- data sources are located in src/datasource/;
- data source names should be lower case, with underscores for word separation;
- data about currently executing process is available in src/inputdatastorage.*
    files. Consult existing data sources on how to use it (filename for example);
- each data source must be self-sufficient. Do not rely on other parts of Snoopy;
- each data source must be tidy (free all mallocs, close all file descriptors);
- the first argument passed to each provider is a char array to return message into;
- data source message must not be longer than SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE;
- each data source must have a corresponding header file;
- all data sources must build with -Wall -Werror -strict -std=c99 flags
    (enabled by default);
- code indentation: 4 spaces, no tabs;
- reserverd data source names:
    - snoopy*
- IMPORTANT: Every data source must have corresponding test(s) in tests/datasource/
    directory, which enables automated testing for all upcoming changes. This
    enabled future developers.
- IMPORTANT: All tests must pass! If your new implementation broke something,
    fit it.


If you have developed a shiny new data source and you would like to start using
it with Snoopy, you have to add it to the following files to make Snoopy fully
aware of it:

- src/datasource/NAME.h        (your data source's main header file)
- src/datasource/NAME.c        (your data source's main source code goes here)
- src/datasource/Makefile.am   (one reference)
- src/datasourceregistry.c     (three references - header, name and function name)
- tests/datasource/Makefile.am (one reference, may contain multiple tests)
- tests/datasource/datasource_NAME.sh         (single test or)
- tests/datasource/datasource_NAME-test1.sh   (multiple tests)
- tests/datasource/datasource_NAME-test2.sh
- configure.ac                 (one reference)
- config.h.in                  (one reference, autogenerated by ./bootstrap.sh)
- etc/snoopy.ini.in            (one reference in comments, for documentation purposes)
- ChangeLog                    (short description of new feature)


## Filter development

Rules for filter development are the same as for new data source development,
with the following additional specifics:

- filters are located in src/filter/;
- each filter is passed two arguments:
    - logMessage and
    - filter argument (if any, otherwise an empty string is passed);
- filter argument is literal. If it contains multiple arguments (separated by
    comma, for example), the filter itself must do the parsing/tokenization.
- filter MAY modify logMessage. If it does so, the new log message MUST NOT
    EXCEED the maximum log message size, defined in snoopy.h.
- filter MUST return SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP constant;
- if SNOOPY_FILTER_DROP is returned by filter, it causes immediate termination
    of filter chain processing and message is not logged to syslog;
- reserved filter names:
    - snoopy*
- filter tests are located in tests/filter/


If you have developed a shiny new filter and you would like to start using
it with Snoopy, you have to add it to the following files to make Snoopy fully
aware of it:

- src/filter/NAME.h            (your filter's main header file)
- src/filter/NAME.c            (your filter's main source code goes here)
- src/filter/Makefile.am       (one reference)
- src/filterregistry.c         (three references - header, name and function name)
- tests/filter/Makefile.am     (one reference, may contain multiple tests)
- tests/filter/filter_NAME.sh         (single test or)
- tests/filter/filter_NAME-test1.sh   (multiple tests)
- tests/filter/filter_NAME-test2.sh
- configure.ac                 (one reference)
- config.h.in                  (one reference, autogenerated by ./bootstrap.sh)
- etc/snoopy.ini.in            (one reference in comments, for documentation purposes)
- ChangeLog                    (short description of new feature)



## Output development

Rules for new output development are the same as for new filter development,
with the following additional specifics:

- outputs are located in src/output/;
- as output names are generally very generic, files and functions are suffixed with "...output";
- each output is passed three arguments:
    - logMessage
    - flag whether logMessage is a regular Snoopy log message or an error message
    - output argument string (might not be used by output)
- output argument is literal. If it contains multiple arguments (separated by
    comma, for example), the output itself must do the parsing/tokenization.
- reserved output names:
    - snoopy*
- output tests are located in tests/output/


If you have developed a shiny new output and you would like to start using
it with Snoopy, you have to add it to the following files to make Snoopy fully
aware of it:

- src/output/NAME.h            (your output's main header file)
- src/output/NAME.c            (your output's main source code goes here)
- src/output/Makefile.am       (one reference)
- src/outputregistry.c         (three references - header, name and function name)
- tests/output/Makefile.am     (one reference, may contain multiple tests)
- tests/output/output_NAME.sh         (single test or)
- tests/output/output_NAME-test1.sh   (multiple tests)
- tests/output/output_NAME-test2.sh
- configure.ac                 (one reference)
- config.h.in                  (one reference, autogenerated by ./bootstrap.sh)
- etc/snoopy.ini.in            (one reference in comments, for documentation purposes)
- ChangeLog                    (short description of new feature)



## Useful helpers

The most obvious thing that data sources, filters and outputs might have in
common is argument parsing. If your new feature accepts a comma-separated
argument list, then you are encouraged to use provided implementation. Consult
file src/filter/only_uid.c to see an actual usage of said implementation.



## Creating feature documentation

Here is a brief description of Snoopy documentation structure.

File /README.md:
- general information and quick install instructions;

Directory /doc/:
- documentation which is useful to average Snoopy user;
- /doc/INSTALL.md: detailed installation instructions;
- /doc/FAQ.md: location of answers to frequently asked questions;

- /doc/DATA-SOURCES.md: general, data-sources-specific information;
- /doc/DATA-SOURCE_complex_data_source_name.md: documentation for advanced
    and configurable data source called "complex_data_source_name", which
    has its code located in src/datasource/complex_data_source_name.c;
    (also, do not include "_name" suffix in data source's name:);

- /doc/FILTERING.md: filtering-specific information;
- /doc/FILTER_complex_filter_name.md: documentation for advanced filter called
    "complex_filter_name", which has its code located in
    src/filter/complex_filter_name.c;

Directory /doc/internals/:
- location of documetation useful to Snoopy developers and contributors;
- /doc/internals/README.md: main document about Snoopy internals



## Creating pull requests for upstream merges

Commits:
- your commits should be easily readable, with concise comments;
- your commits should follow the KISS principle: do one thing, and do it well.

Branching:
- see section about branches above.
- branch your contribution branch from Snoopy's master branch;
- branch name: feature/my-shiny-new-snoopy-feature-title for new features;
- branch name: bugfix/my-totally-non-hackish-workaround for bugfixes;
- branch name: master (avoiding creating dedicated branch) may only be used for trivial fixes.

Pull requests:
- do not create pull requests from master branch;
- one pull request should contain one change only
    (one bugfix or one feature at a time);
- if you have developed multiple features and/or bugfixes, create separate
    branches for each one of them, and request merges for each branch;
- Snoopy uses Travis-CI for testing builds. When you submit a pull request,
    wait for Travis-CI to finish the build and see if everything went
    smoothly.
- the cleaner you code/change/changeset is,
    the faster it will be merged.
- Coverity scan is run on as-needed basis, but keep in mind that you might be
    "called to court:)" - contacted to defend your contribution should Coverity
    point out some defects in your code.

That is it. Happy coding! :)
