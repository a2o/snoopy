### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Subdir processing order
#
SUBDIRS      =

SUBDIRS     += bin
if CONFIGFILE_ENABLED
    SUBDIRS += configfile
endif
SUBDIRS     += datasource
if FILTERING_ENABLED
    SUBDIRS += filter
endif
SUBDIRS     += message
SUBDIRS     += output
if THREAD_SAFETY_ENABLED
    SUBDIRS += threads
endif
#
# Functional tests at last
#
if CONFIGFILE_ENABLED
if DATASOURCE_ENABLED_cmdline
if OUTPUT_ENABLED_file
SUBDIRS     += combined
endif
endif
endif



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
