### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS =
XFAIL_TESTS =

if OUTPUT_ENABLED_stdout
TESTS += unit-action-log-message-dispatch.sh
TESTS += unit-action-log-syscall-exec.sh
endif

TESTS += unit-error.sh

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

if DATASOURCE_ENABLED_systemd_unit_name
TESTS += unit-util-systemd.sh
endif



### Include tests in distribution archive
#
EXTRA_DIST    = _bootstrap.sh
EXTRA_DIST   += $(TESTS)
