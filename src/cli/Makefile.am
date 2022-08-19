### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Variables to add final targets to
#
noinst_LTLIBRARIES =
sbin_PROGRAMS =



### Build subroutines separately, it gets linked into `snoopy-test` too
#
noinst_LTLIBRARIES += libsnoopy-cli-subroutines.la
libsnoopy_cli_subroutines_la_SOURCES = \
    cli-subroutines.c \
    cli-subroutines.h



### Program-based entry points
#
sbin_PROGRAMS += snoopyctl
snoopyctl_SOURCES = \
	action-conf.c \
	action-conf.h \
	action-disable.c \
	action-disable.h \
	action-enable.c \
	action-enable.h \
	action-status.c \
	action-status.h \
	action-version.c \
	action-version.h \
	snoopyctl.c
snoopyctl_LDADD = \
    libsnoopy-cli-subroutines.la \
    ../util/libsnoopy-utils.la
