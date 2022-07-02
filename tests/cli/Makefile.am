### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
SCRIPT_PREFIX = cli
TESTS         =
XFAIL_TESTS   =

TESTS        += $(SCRIPT_PREFIX)-action-about.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-1-nofile.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-2-emptyfile.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-31-only-entry.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-32-only-entry-no-newline.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-33-only-entry-and-space.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-34-only-entry-and-tab.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-35-only-entry-and-comment.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-41-first.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-42-middle.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-43-last.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-44-last-no-newline.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-45-after-identical-comment.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-51-dupe-mine.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-52-dupe-alien.sh
TESTS        += $(SCRIPT_PREFIX)-action-disable-53-dupe-mixed.sh
TESTS        += $(SCRIPT_PREFIX)-action-enable-1-nofile.sh
TESTS        += $(SCRIPT_PREFIX)-action-enable-2-emptyfile.sh
TESTS        += $(SCRIPT_PREFIX)-action-enable-31-non-empty.sh
TESTS        += $(SCRIPT_PREFIX)-action-enable-32-non-empty-no-newline.sh
TESTS        += $(SCRIPT_PREFIX)-action-enable-45-after-identical-comment.sh
TESTS        += $(SCRIPT_PREFIX)-action-enable-51-already-enabled.sh
TESTS        += $(SCRIPT_PREFIX)-action-enable-52-another-instance-enabled.sh
TESTS        += $(SCRIPT_PREFIX)-action-help.sh
TESTS        += $(SCRIPT_PREFIX)-action-status-ld.so.preload-absent.sh
TESTS        += $(SCRIPT_PREFIX)-action-status-ld.so.preload-present.sh
TESTS        += $(SCRIPT_PREFIX)-action-status-LD_PRELOAD-absent.sh
TESTS        += $(SCRIPT_PREFIX)-action-status-LD_PRELOAD-present.sh
TESTS        += $(SCRIPT_PREFIX)-action-status-LD_PRELOAD-set-but-not-present.sh
TESTS        += $(SCRIPT_PREFIX)-action-version-cli.sh
TESTS        += $(SCRIPT_PREFIX)-action-version-library.sh
TESTS        += $(SCRIPT_PREFIX)-err-action-missing-msg.sh
TESTS        += $(SCRIPT_PREFIX)-err-action-missing-status.sh
TESTS        += $(SCRIPT_PREFIX)-err-action-unknown-msg.sh
TESTS        += $(SCRIPT_PREFIX)-err-action-unknown-status.sh

### Include tests in distribution archive
#
EXTRA_DIST    = _bootstrap.sh
EXTRA_DIST   += $(TESTS)



### Remove stale temporary test files, if tests got stuck
#
clean-local:
	rm -f *.sh.* ;
