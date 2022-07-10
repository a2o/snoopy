### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS =
XFAIL_TESTS =

if THREAD_SAFETY_ENABLED
    TESTS += threads_creation.sh
    TESTS += threads_fork_exec.sh
    TESTS += threads_try-to-segfault.sh
endif



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
EXTRA_DIST += $(TESTS)
