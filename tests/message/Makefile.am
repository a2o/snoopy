### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS =
XFAIL_TESTS =

TESTS += message_format-datasource-arg.sh
TESTS += message_format-datasource-noarg.sh
TESTS += message_format-ds-closing-tag-missing.sh
TESTS += message_format-ds-unknown.sh
TESTS += message_format-ds-failure.sh
TESTS += message_format-empty.sh
TESTS += message_format-static.sh



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += $(TESTS)
