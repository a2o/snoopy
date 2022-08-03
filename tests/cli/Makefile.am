### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Which tests to run
#
TESTS =
XFAIL_TESTS =

TESTS += cli-action-about.sh
if CONFIGFILE_ENABLED
TESTS += cli-action-conf.sh
endif
TESTS += cli-action-disable-1-nofile.sh
TESTS += cli-action-disable-2-emptyfile.sh
TESTS += cli-action-disable-31-only-entry.sh
TESTS += cli-action-disable-32-only-entry-no-newline.sh
TESTS += cli-action-disable-33-only-entry-and-space.sh
TESTS += cli-action-disable-34-only-entry-and-tab.sh
TESTS += cli-action-disable-35-only-entry-and-comment.sh
TESTS += cli-action-disable-41-first.sh
TESTS += cli-action-disable-42-middle.sh
TESTS += cli-action-disable-43-last.sh
TESTS += cli-action-disable-44-last-no-newline.sh
TESTS += cli-action-disable-45-after-identical-comment.sh
TESTS += cli-action-disable-51-dupe-mine.sh
TESTS += cli-action-disable-52-dupe-alien.sh
TESTS += cli-action-disable-53-dupe-mixed.sh
TESTS += cli-action-enable-1-nofile.sh
TESTS += cli-action-enable-2-emptyfile.sh
TESTS += cli-action-enable-31-non-empty.sh
TESTS += cli-action-enable-32-non-empty-no-newline.sh
TESTS += cli-action-enable-45-after-identical-comment.sh
TESTS += cli-action-enable-51-already-enabled.sh
TESTS += cli-action-enable-52-another-instance-enabled.sh
TESTS += cli-action-help.sh
TESTS += cli-action-status-ld.so.preload-alien.sh
TESTS += cli-action-status-ld.so.preload-absent.sh
TESTS += cli-action-status-ld.so.preload-dupe.sh
TESTS += cli-action-status-ld.so.preload-present.sh
TESTS += cli-action-status-LD_PRELOAD-absent.sh
TESTS += cli-action-status-LD_PRELOAD-present.sh
TESTS += cli-action-status-LD_PRELOAD-set-but-not-present.sh
TESTS += cli-action-version-cli.sh
TESTS += cli-action-version-library.sh
TESTS += cli-err-action-missing-msg.sh
TESTS += cli-err-action-missing-status.sh
TESTS += cli-err-action-unknown-msg.sh
TESTS += cli-err-action-unknown-status.sh

### Include tests in distribution archive
#
EXTRA_DIST    = _bootstrap.sh
EXTRA_DIST   += $(TESTS)



### Remove stale temporary test files, if tests got stuck
#
clean-local-this-dir:
	rm -f *.sh.* ;
