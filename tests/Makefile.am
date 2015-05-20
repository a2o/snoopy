### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Subdir processing order
#
# Process this dir first, to check for dependencies
#
SUBDIRS      = .
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



### Make check - required programs
#
# Some of there tests are in configure.ac, but we keep
# alternate copy here in case those in configure.ac are
# removed
#
check-local:
	if ! which socat > /dev/null ; then \
	    echo ; \
	    echo "ERROR: Program 'socat' not found!" ; \
	    echo ; \
	    exit 1 ; \
	fi ;
