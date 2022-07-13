### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS =
XFAIL_TESTS =

if CONFIGFILE_ENABLED
TESTS += unit-ext-ini.sh
endif
if THREAD_SAFETY_ENABLED
TESTS += unit-ext-list.sh
endif
if FILTERING_ENABLED
TESTS += unit-filterregistry.sh
endif
TESTS += unit-misc.sh
TESTS += unit-outputregistry.sh



### Include tests in distribution archive
#
EXTRA_DIST    = _bootstrap.sh
EXTRA_DIST   += $(TESTS)
