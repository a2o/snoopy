# Contributing to Snoopy development

New ideas are welcome. Most of change requests so far were about additional
log data or filtering capabilities, therefore most of development/changes
is expected in that area.

Initial information about Snoopy hacking is available here: [doc/HACKING-OVERVIEW.md](../doc/HACKING-OVERVIEW.md)



## Table of Contents

* [Git branches - persistent](#git-branches---persistent)
* [Git branch naming for pull requests](#git-branch-naming-for-pull-requests)
* [Creating feature documentation](#creating-feature-documentation)
* [Creating a pull request](#creating-a-pull-request)



## Git branches - persistent

The following git branches are always available in main Snoopy repository:

- `master`: contains latest Snoopy development efforts. All new features
    should be based on this branch.
- `install`: branch dedicated to Snoopy automated install procedure. When
    you use it, install-snoopy.sh script is downloaded from this
    branch.
- `coverity_scan`: when Coverity analysis of a build is required, it is
    performed using this branch. Usually it lazily follows master
    branch. Coverity has a limited build submission frequency, thus
    a dedicated branch has been created to trigger analysis on
    as-needed basis.
- `legacy`: orphaned branch, which contains imported Snoopy releases from
    1.00 to 1.6.1 + changes to bring it in sync with initial commit
    from master branch. This is a lousy attempt to recreate lost
    SVN history (commits 1-31). Present for historical completeness
    only.



## Git branch naming for pull requests

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

**As a general rule, base your contribution on the `master` branch.**

Non-exception:
- you have developed a new feature and would like it to land in the next bugfix
    release: **do not bother**. Bugfix releases are bugfix releases, they are
    not intended to introduce new features. Base your work on **master** branch.
    If you somehow manage to convince maintainer(s) that your feature should
    land in next bugfix release anyway, maintainer will do the integration.



## Creating feature documentation

Here is a brief description of Snoopy documentation structure in the [doc/](../doc) directory:

- [doc/INSTALL.md](../doc/INSTALL.md): detailed installation instructions;
- [doc/FAQ.md](../doc/FAQ.md): location of answers to frequently asked questions;

- `doc/DATA-SOURCES.md`: general, data-sources-specific information;
- `doc/DATA-SOURCE_complex_data_source_name.md`: documentation for advanced
    and configurable data source called "complex_data_source_name", which
    has its code located in `src/datasource/complex_data_source_name.c`;
    (also, do not include "_name" suffix in data source's name:);

- `doc/FILTERING.md`: filtering-specific information;
- `doc/FILTER_complex_filter_name.md`: documentation for advanced filter called
    "complex_filter_name", which has its code located in
    src/filter/complex_filter_name.c;

- `doc/HACKING-*.md`: location of documentation useful to Snoopy developers and contributors;



## Creating a pull request

Commits:
- Commits should follow the KISS principle: do one thing, and do it well (keep it simple, stupid).
- Commit messages should be easily readable, imperative style ("Fix memory leak in...", not "FixES mem...")

Branching and PR branch naming:
- Branch your contribution branch from Snoopy's `master` branch;
- Consult the sections about branch naming above;
- Branch name example #1: `feature/my-shiny-new-snoopy-feature-title` for new features;
- branch name example #2: `bugfix/my-totally-non-hackish-workaround` for bugfixes;

Pull requests:
- Do not create pull requests for merging your master branch - see PR branch naming section above;
- One pull request should contain one logical change only
    (one bugfix or one feature at a time);
- If you have developed multiple features and/or bugfixes, create separate
    branches for each one of them, and request merges for each branch;
- Snoopy uses Travis-CI for testing builds. When you submit a pull request,
    wait for Travis-CI to finish the build and see if everything went
    smoothly.
- The cleaner you code/change/changeset is,
    the faster it will be merged.
- Coverity scan is run on an as-needed basis, but keep in mind that you might be
    "called to court" ;)



That is it. Your pull requests are welcome! ;)
