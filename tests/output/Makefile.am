### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS =
XFAIL_TESTS =

if OUTPUT_ENABLED_devnull
    TESTS += output_devnull.sh
endif

if OUTPUT_ENABLED_file
    TESTS += output_file.sh
if DATASOURCE_ENABLED_username
    TESTS += output_file-dynamic.sh
endif
endif

TESTS += output_noop.sh

if OUTPUT_ENABLED_socket
    TESTS += output_socket.sh
endif

if OUTPUT_ENABLED_stderr
    TESTS += output_stderr.sh
endif

if OUTPUT_ENABLED_stdout
    TESTS += output_stdout.sh
endif



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += $(TESTS)
