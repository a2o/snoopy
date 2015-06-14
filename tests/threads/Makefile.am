### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
SCRIPT_PREFIX = threads
TESTS  =
XFAIL_TESTS  =

if THREAD_SAFETY_ENABLED
    TESTS += $(SCRIPT_PREFIX)_creation.sh
    TESTS += $(SCRIPT_PREFIX)_fork_exec.sh

### Test commented out intentionally
#
# This one is commented out because it takes a lot of time.
# If threading issues are suspected, it should be run manually
#
#    TESTS += $(SCRIPT_PREFIX)_try-to-segfault.sh
endif



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += $(TESTS)
