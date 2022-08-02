### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS =
XFAIL_TESTS =

TESTS        += configfile-bom.sh
TESTS        += configfile-error_logging-y1.sh
TESTS        += configfile-error_logging-y2.sh
TESTS        += configfile-error_logging-y3.sh
TESTS        += configfile-error_logging-n1.sh
TESTS        += configfile-error_logging-n2.sh
TESTS        += configfile-error_logging-n3.sh
TESTS        += configfile-inline-comment.sh
TESTS        += configfile-line-length-1023.sh
TESTS        += configfile-line-length-1024.sh
XFAIL_TESTS  += configfile-line-length-1024.sh
if FILTERING_ENABLED
TESTS        += configfile-filter_chain-1.sh
endif
TESTS        += configfile-message_format-1.sh
TESTS        += configfile-message_format-unquoted.sh
TESTS        += configfile-message_format-single-quoted.sh
TESTS        += configfile-message_format-double-quoted.sh
TESTS        += configfile-output-devlog.sh
if OUTPUT_ENABLED_file
TESTS        += configfile-output-file.sh
endif
TESTS        += configfile-output-invalid.sh
XFAIL_TESTS  += configfile-output-invalid.sh
TESTS        += configfile-output-socket.sh
TESTS        += configfile-syslog_facility.sh
TESTS        += configfile-syslog_ident.sh
TESTS        += configfile-syslog_level.sh



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += configfile-generic.sh
EXTRA_DIST += $(TESTS)
