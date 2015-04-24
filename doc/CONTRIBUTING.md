# Contributing to Snoopy development

[![Flattr Snoopy Logger project](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=a2o&url=https://github.com/a2o/snoopy&title=Snoopy Logger)

New ideas are welcome. Most of change requests so far were about additional
log data or filtering capabilities, therefore most of development/changes
is expected in that area.



Table of Contents
=================

  * [Contributing to Snoopy development](#contributing-to-snoopy-development)
    * [Basic rules for input provider development](#basic-rules-for-input-provider-development)
    * [Filter development rules](#filter-development-rules)
    * [Creating feature documentation](#creating-feature-documentation)
    * [Creating pull requests for upstream merges](#creating-pull-requests-for-upstream-merges)



## Basic rules for input provider development

- input providers are located in src/input/;
- input provider names should be lower case, with underscores for word separation;
- data about currently executing process is available in src/inputdatastorage.*
    files. Consult existing providers on how to use it (filename for example);
- each input provider must be self-sufficient. Do not rely on other parts of Snoopy;
- each input provider must be tidy (free all mallocs, close all file descriptors);
- the first argument passed to each provider is a char array to return message into;
- input provider message must not be longer than SNOOPY_INPUT_MESSAGE_MAX_SIZE;
- each input provider must have a corresponding header file;
- all input providers must build with -Wall -Werror flags (enabled by default);
- code indentation: 4 spaces, no tabs;
- reserverd input names:
    - snoopy*

If you have developed a shiny new input provider and you would like to
start using it with Snoopy, there are three additional places where you
need to add references to it to make Snoopy fully aware of it:

- src/input/Makefile.am   (location is evident)
- src/inputregistry.h     (one reference)
- src/inputregistry.c     (two references)



## Filter development rules

Rules for filter development are the same as for new input provider development,
with the following additional specifics:

- filters are located in src/filter/;
- each filter is passed two arguments: logMessage and filter argument (if any,
    otherwise an empty string is passed);
- filter argument is literal. If it contains multiple arguments (separated by
    comma, for example), the filter itself must do the parsing/tokenization.
- filter MAY modify logMessage. If it does so, the new log message MUST NOT
    EXCEED the maximum log message size, defined in snoopy.h.
- filter MUST return SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP constant;
- if SNOOPY_FILTER_DROP is returned by filter, it causes immediate termination
    of filter chain processing and message is not logged to syslog;
- reserved filter names:
    - snoopy*

If you have developed a shiny new filter and you would like to
start using it with Snoopy, there are three additional places where you
need to add references to it to make Snoopy fully aware of it:

- src/filter/Makefile.am   (location is evident)
- src/filterregistry.h     (one reference)
- src/filterregistry.c     (two references)



## Creating feature documentation

Here is a brief description of Snoopy documentation structure.

File /README.md:
- general information and quick install instructions;

Directory /doc/:
- documentation which is useful to average Snoopy user;
- /doc/INSTALL.md: detailed installation instructions;
- /doc/FAQ.md: location of answers to frequently asked questions;

- /doc/INPUT-PROVIDERS.md: general, input-provider-specific information;
- /doc/INPUT-PROVIDER_complex_input_provider_name.md: documentation for advanced
    and configurable input provider called "complex_input_provider_name", which
    has its code located in src/input/complex_input_provider_name.c;

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
