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

if DATASOURCE_ENABLED_cmdline
TESTS += unit-datasource-cmdline.sh
endif

TESTS += unit-error.sh

if CONFIGFILE_ENABLED
TESTS += unit-ext-ini.sh
endif

if FILTERING_ENABLED
TESTS += unit-filterregistry.sh
endif

TESTS += unit-outputregistry.sh
TESTS += unit-util-syslog.sh

if DATASOURCE_ENABLED_systemd_unit_name
TESTS += unit-util-systemd.sh
endif

if DATASOURCE_ENABLED_ipaddr
TESTS += unit-util-utmp-entry-missing.sh
TESTS += unit-util-utmp-entry-without-ip-address.sh
TESTS += unit-util-utmp-ipv4.sh
TESTS += unit-util-utmp-ipv6.sh
endif



### Include tests in distribution archive
#
EXTRA_DIST    = _bootstrap.sh
EXTRA_DIST   += $(TESTS)

if DATASOURCE_ENABLED_ipaddr
EXTRA_DIST += unit-util-utmp-testUtmpFile
endif
