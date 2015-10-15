# Contributing to Snoopy development

[![Flattr Snoopy Logger project](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=a2o&url=https://github.com/a2o/snoopy&title=Snoopy Logger)

New ideas are welcome. Most of change requests so far were about additional
log data or filtering capabilities, therefore most of development/changes
is expected in that area.

Initial information about Snoopy hacking is available here: [doc/HACKING.md](doc/HACKING.md)



Table of Contents
=================

  * [Contributing to Snoopy development](#contributing-to-snoopy-development)
    * [Git branches - persistent](#git-branches---persistent)
    * [Git branches - transient](#git-branches---transient)
    * [Creating feature documentation](#creating-feature-documentation)
    * [Creating pull requests for upstream merges](#creating-pull-requests-for-upstream-merges)



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



That is it. May the successfull-patch-submission-force be with you! :)
