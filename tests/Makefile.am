### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Subdir processing order
#
SUBDIRS      =

SUBDIRS     += bin
SUBDIRS     += cli
if CONFIGFILE_ENABLED
    SUBDIRS += configfile
endif
SUBDIRS     += datasource
if FILTERING_ENABLED
    SUBDIRS += filter
endif
SUBDIRS     += general
SUBDIRS     += message
SUBDIRS     += output
if THREAD_SAFETY_ENABLED
    SUBDIRS += threads
endif
SUBDIRS     += unit
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



### Include these files too
#
EXTRA_DIST  =
EXTRA_DIST += _bootstrap.sh
EXTRA_DIST += _bootstrap-paths.sh



### Clean all test leftovers
#
# These are usually left behind when we rename files
#
clean-local-this-dir:
	rm -f */*.log ;
	rm -f */*.trs ;
