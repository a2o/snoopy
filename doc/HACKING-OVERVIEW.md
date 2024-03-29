# Hacking - Snoopy Command Logger

New ideas are welcome. Most of change requests so far were about additional
log data or filtering capabilities, therefore most of development/changes
is expected in that area, and the workflow is optimised for it too.

More advanced changes or change proposals are also welcome. You are advised to
read this document first, and then if you still have questions, proceed to
[HACKING-INTERNALS.md](HACKING-INTERNALS.md) and then to Snoopy source code itself.



Table of Contents
=================

  * [Hacking Snoopy](#hacking-snoopy)
    * [OS for development](#os-for-development)
    * [Clone git repository](#clone-git-repository)
    * [Data source development](#data-source-development)
    * [Filter development](#filter-development)
    * [Output development](#output-development)
    * [Useful helpers](#useful-helpers)



## OS for development

Development of Snoopy is (and should) be done on fairly recent OS releases.
Latest Ubuntu LST would be a good choice.

Required versions of software:
- automake 1.11
- autoconf 2.69
- libtool



## Clone git repository

File [INSTALL.md](INSTALL.md) contains detailed instructions about git repository
cloning. Also, do not forget to run ./bootstrap.sh:

Recap of the steps:
```shell
git clone https://github.com/a2o/snoopy.git
cd snoopy
./bootstrap.sh
```

If `./bootstrap.sh` reports an error, you're probably missing some development
tools on your system. There is a helper available to install them quickly:
```shell
./dev-tools/install-dev-software.sh
```



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
- reserved data source names:
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
- etc/snoopy.ini.in            (one reference in comments, for documentation purposes)
- ChangeLog                    (short description of new feature)



## Filter development

Rules for filter development are the same as for new data source development,
with the following additional specifics:

- filters are located in src/filter/;
- each filter is passed one argument:
    - filter argument (if any, otherwise an empty string is passed);
- filter argument is literal. If it contains multiple arguments (separated by
    comma, for example), the filter itself must do the parsing/tokenization.
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
- etc/snoopy.ini.in            (one reference in comments, for documentation purposes)
- ChangeLog                    (short description of new feature)



## Output development

Rules for new output development are the same as for new filter development,
with the following additional specifics:

- outputs are located in src/output/;
- as output names are generally very generic, files and functions are suffixed with "...output";
- each output is passed three arguments:
    - logMessage
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
- etc/snoopy.ini.in            (one reference in comments, for documentation purposes)
- ChangeLog                    (short description of new feature)



## Useful helpers

The most obvious thing that data sources, filters and outputs might have in
common is argument parsing. If your new feature accepts a comma-separated
argument list, then you are encouraged to use provided implementation. Consult
file src/filter/only_uid.c to see an actual usage of said implementation.



That is it. Happy hacking! :)
