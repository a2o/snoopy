### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
SCRIPT_PREFIX = combined
TESTS         =
XFAIL_TESTS   =

TESTS        += $(SCRIPT_PREFIX)-nofilter-1.sh
TESTS        += $(SCRIPT_PREFIX)-nofilter-2.sh

if FILTERING_ENABLED
if FILTER_ENABLED_exclude_uid
TESTS        += $(SCRIPT_PREFIX)-filter-exclude_uid-drop.sh
TESTS        += $(SCRIPT_PREFIX)-filter-exclude_uid-pass.sh
endif
if FILTER_ENABLED_only_uid
TESTS        += $(SCRIPT_PREFIX)-filter-only_uid-drop.sh
TESTS        += $(SCRIPT_PREFIX)-filter-only_uid-pass.sh
endif
endif



### Include tests in distribution archive
#
EXTRA_DIST    = _bootstrap.sh
EXTRA_DIST   += combined-generic.sh
EXTRA_DIST   += $(TESTS)



### Remove stale temporary test files, if tests got stuck
#
clean-local:
	rm -f *.ini ;
	rm -f *.out ;
