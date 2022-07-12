### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS =
XFAIL_TESTS =

if FILTER_ENABLED_exclude_spawns_of
    TESTS += filter_exclude_spawns_of-mustdrop-multiarg-first.sh
    TESTS += filter_exclude_spawns_of-mustdrop-multiarg-last.sh
    TESTS += filter_exclude_spawns_of-mustdrop-multiarg-mid.sh
    TESTS += filter_exclude_spawns_of-mustdrop-singlearg.sh
    TESTS += filter_exclude_spawns_of-mustdrop-spaceparent.sh
    TESTS += filter_exclude_spawns_of-mustlog-multiarg.sh
    TESTS += filter_exclude_spawns_of-mustlog-singlearg.sh
    TESTS += filter_exclude_spawns_of-mustlog-spaceparent.sh
endif

if DATASOURCE_ENABLED_uid
if FILTER_ENABLED_exclude_uid
    TESTS += filter_exclude_uid-pass.sh
    TESTS += filter_exclude_uid-pass-multi.sh
    TESTS += filter_exclude_uid-drop.sh
    TESTS += filter_exclude_uid-drop-multi.sh
endif
endif

if FILTER_ENABLED_only_tty
    TESTS += filter_only_tty.sh
endif

if DATASOURCE_ENABLED_uid
if FILTER_ENABLED_only_uid
    TESTS += filter_only_uid-pass.sh
    TESTS += filter_only_uid-pass-multi.sh
    TESTS += filter_only_uid-drop.sh
    TESTS += filter_only_uid-drop-multi.sh
    TESTS += filterchain-drop.sh
    TESTS += filterchain-pass.sh
endif
endif

TESTS += filterchain-unknown-filter.sh



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += $(TESTS)
