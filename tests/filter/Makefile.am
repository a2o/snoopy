### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
SCRIPT_PREFIX = filter
TESTS  =
XFAIL_TESTS  =

if DATASOURCE_ENABLED_uid
if FILTER_ENABLED_exclude_uid
    TESTS += $(SCRIPT_PREFIX)_exclude_uid-pass.sh
    TESTS += $(SCRIPT_PREFIX)_exclude_uid-pass-multi.sh
    TESTS += $(SCRIPT_PREFIX)_exclude_uid-drop.sh
    TESTS += $(SCRIPT_PREFIX)_exclude_uid-drop-multi.sh
endif
endif

if FILTER_ENABLED_only_tty
    TESTS += $(SCRIPT_PREFIX)_only_tty.sh
endif

if DATASOURCE_ENABLED_uid
if FILTER_ENABLED_only_uid
    TESTS += $(SCRIPT_PREFIX)_only_uid-pass.sh
    TESTS += $(SCRIPT_PREFIX)_only_uid-pass-multi.sh
    TESTS += $(SCRIPT_PREFIX)_only_uid-drop.sh
    TESTS += $(SCRIPT_PREFIX)_only_uid-drop-multi.sh
endif
endif



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += $(TESTS)
