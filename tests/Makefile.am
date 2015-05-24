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



### Include tests in distribution archive
#
EXTRA_DIST  = _bootstrap.sh
