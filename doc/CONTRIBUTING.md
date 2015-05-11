# Contributing to Snoopy development

[![Flattr Snoopy Logger project](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=a2o&url=https://github.com/a2o/snoopy&title=Snoopy Logger)

New ideas are welcome. Most of change requests so far were about additional
log data or filtering capabilities, therefore most of development/changes
is expected in that area.



Table of Contents
=================

  * [Contributing to Snoopy development](#contributing-to-snoopy-development)
    * [Basic rules for data source development](#basic-rules-for-data-source-development)
    * [Filter development rules](#filter-development-rules)
    * [Creating feature documentation](#creating-feature-documentation)
    * [Creating pull requests for upstream merges](#creating-pull-requests-for-upstream-merges)



## Basic rules for data source development

- data sources are located in src/datasource/;
- data source names should be lower case, with underscores for word separation;
- data about currently executing process is available in src/inputdatastorage.*
    files. Consult existing data sources on how to use it (filename for example);
- each data source must be self-sufficient. Do not rely on other parts of Snoopy;
- each data source must be tidy (free all mallocs, close all file descriptors);
- the first argument passed to each provider is a char array to return message into;
- data source message must not be longer than SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE;
- each data source must have a corresponding header file;
- all data sources must build with -Wall -Werror flags (enabled by default);
- code indentation: 4 spaces, no tabs;
- reserverd data source names:
    - snoopy*

If you have developed a shiny new data source and you would like to start using
it with Snoopy, you have to add it to the following files to make Snoopy fully
aware of it:

- src/datasource/NAME.h        (your data source's main header file)
- src/datasource/NAME.c        (your data source's main source code goes here)
- src/datasource/Makefile.am   (one reference)
- src/datasourceregistry.c     (three references - header, name and function name)
- configure.ac                 (one reference)
- etc/snoopy.ini.in            (one reference in comments, for documentation purposes)
- Changelog                    (short description of new feature)


## Filter development rules

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

If you have developed a shiny new filter and you would like to start using
it with Snoopy, you have to add it to the following files to make Snoopy fully
aware of it:

- src/filter/NAME.h            (your filter's main header file)
- src/filter/NAME.c            (your filter's main source code goes here)
- src/filter/Makefile.am       (one reference)
- src/filterregistry.c         (three references - header, name and function name)
- configure.ac                 (one reference)
- etc/snoopy.ini.in            (one reference in comments, for documentation purposes)
- Changelog                    (short description of new feature)



## Output development rules

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

If you have developed a shiny new output and you would like to start using
it with Snoopy, you have to add it to the following files to make Snoopy fully
aware of it:

- src/output/NAME.h            (your output's main header file)
- src/output/NAME.c            (your output's main source code goes here)
- src/output/Makefile.am       (one reference)
- src/outputregistry.c         (three references - header, name and function name)
- src/configfile.c             (one reference)
- src/snoopy.h                 (one reference (or more if this is the new default output))
- configure.ac                 (one reference)
- etc/snoopy.ini.in            (one reference in comments, for documentation purposes)
- Changelog                    (short description of new feature)



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
- branch name: feature/my-shiny-new-snoopy-feature-title for new features;
- branch name: bugfix/my-totally-non-hackish-workaround for bugfixes;
- branch name: master (avoiding creating dedicated branch) may only be used for trivial fixes.

Pull requests:
- do not create pull requests from master branch, unless absolutely sure to do so;
- one pull request should contain one change only
    (one bugfix or one feature at a time);
- if you have developed multiple features and/or bugfixes, create separate
    branches for each one of them, and request merges for each branch;
- the cleaner you code/change/changeset is,
    the faster it will be merged.

That is it. Happy coding! :)
