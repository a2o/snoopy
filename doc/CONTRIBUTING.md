# Contributing to Snoopy development

[![Flattr Snoopy Logger project](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=a2o&url=https://github.com/a2o/snoopy&title=Snoopy Logger)



New ideas are welcome. Most of change requests so far were about additional
log data or filtering capabilities, therefore most of development/changes
is expected in that area.

## Here are basic rules for input provider development:

- input providers are located in src/input/
- input provider names should be lower case, with underscores for word separation
- data about currently executing process is available in src/inputdatastorage.*
    files. Consult existing providers on how to use it (filename for example)
- each input provider must be self-sufficient. Do not rely on other parts of Snoopy
- each input provider must be tidy (free all mallocs, close all file descriptors)
- the first argument passed to each provider is a char array to return message into
- input provider message must not be longer than SNOOPY_INPUT_MESSAGE_MAX_SIZE
- each input provider must have a corresponding header file
- all input providers must build with -Wall -Werror flags (enabled by default)
- code indentation: 4 spaces, no tabs
- reserverd input names:
    - snoopy*

If you have developed a shiny new input provider and you would like to
start using it with Snoopy, there are three additional places where you
need to add references to it to make Snoopy fully aware of it:

- src/input/Makefile.am   (location is evident)
- src/inputregistry.h     (one reference)
- src/inputregistry.c     (two references)


## New filter development rules

Rules for filter development are the same as for new input provider development,
with the following additional specifics:

- filters are located in src/filter/
- each filter is passed two arguments: logMessage and filter argument (if any,
    otherwise an empty string is passed)
- filter argument is literal. If it contains multiple arguments (separated by
    comma, for example), the filter itself must do the parsing/tokenization.
- filter MAY modify logMessage. If it does so, the new log message MUST NOT
    EXCEED the maximum log message size, defined in snoopy.h.
- filter MUST return SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP constant
- if SNOOPY_FILTER_DROP is returned by filter, it causes immediate termination
    of filter chain processing and message is not logged to syslog
- reserved filter names:
    - snoopy*

If you have developed a shiny new filter and you would like to
start using it with Snoopy, there are three additional places where you
need to add references to it to make Snoopy fully aware of it:

- src/filter/Makefile.am   (location is evident)
- src/filterregistry.h     (one reference)
- src/filterregistry.c     (two references)


# Creating pull requests for upstream merges

Commits:
- your commits should be easily readable, with concise comments
- your commits should follow the KISS principle: do one thing, and do it well

Branching:
- branch name: feature/my-shiny-new-snoopy-feature-title for new features
- branch name: bugfix/my-totally-non-hackish-workaround for bugfixes
- branch name: master (avoiding creating dedicated branch) may only be used for trivial fixes

Pull requests:
- do not create pull requests from master branch, unless absolutely sure to do so
- one pull request should contain one change only
    (one bugfix or one feature at a time)
- if you have developed multiple features and/or bugfixes, create separate
    branches for each one of them, and request merges for each branch
- the cleaner you code/change/changeset is, the faster it will be merged

That is it. Happy coding! :)
