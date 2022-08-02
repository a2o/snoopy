### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS         =
XFAIL_TESTS   =

TESTS        += general-symbol-leaks.sh



### Include tests in distribution archive
#
EXTRA_DIST    = _bootstrap.sh
EXTRA_DIST   += $(TESTS)



### Remove stale temporary test files, if tests got stuck
#
clean-local-this-dir:
	rm -f *.ini ;
	rm -f *.out ;
