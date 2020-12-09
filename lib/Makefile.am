### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common
SUBDIRS =


### INI file parser
#
if CONFIGFILE_ENABLED
SUBDIRS += inih
endif


if THREAD_SAFETY_ENABLED
SUBDIRS += liblcthw
endif
