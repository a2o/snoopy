### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Subdir processing order
#
SUBDIRS      =

SUBDIRS     += bin
if CONFIG_FILE_ENABLED
    SUBDIRS += configfile
endif
SUBDIRS     += datasource
if FILTERING_ENABLED
    SUBDIRS += filter
endif
SUBDIRS     += message
SUBDIRS     += output
