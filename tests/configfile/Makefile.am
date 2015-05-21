### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
SCRIPT_PREFIX = configfile
TESTS         =
XFAIL_TESTS   =

TESTS        += $(SCRIPT_PREFIX)-filter_chain-1.sh
TESTS        += $(SCRIPT_PREFIX)-message_format-1.sh
TESTS        += $(SCRIPT_PREFIX)-output-devlog.sh
TESTS        += $(SCRIPT_PREFIX)-output-file.sh
TESTS        += $(SCRIPT_PREFIX)-output-invalid.sh
XFAIL_TESTS  += $(SCRIPT_PREFIX)-output-invalid.sh
TESTS        += $(SCRIPT_PREFIX)-output-socket.sh
TESTS        += $(SCRIPT_PREFIX)-syslog_facility.sh
TESTS        += $(SCRIPT_PREFIX)-syslog_level.sh
