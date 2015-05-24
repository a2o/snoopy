### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
SCRIPT_PREFIX = message
TESTS         =
XFAIL_TESTS   =

TESTS += $(SCRIPT_PREFIX)_format-datasource-arg.sh
TESTS += $(SCRIPT_PREFIX)_format-datasource-noarg.sh
TESTS += $(SCRIPT_PREFIX)_format-empty.sh
TESTS += $(SCRIPT_PREFIX)_format-static.sh



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += $(TESTS)
