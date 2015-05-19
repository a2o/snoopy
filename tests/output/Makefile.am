### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
SCRIPT_PREFIX = output
TESTS  =
XFAIL_TESTS  =

if OUTPUT_ENABLED_file
    TESTS += $(SCRIPT_PREFIX)_file.sh
endif

if OUTPUT_ENABLED_socket
    TESTS += $(SCRIPT_PREFIX)_socket.sh
endif

if OUTPUT_ENABLED_stderr
    TESTS += $(SCRIPT_PREFIX)_stderr.sh
endif

if OUTPUT_ENABLED_stdout
    TESTS += $(SCRIPT_PREFIX)_stdout.sh
endif
