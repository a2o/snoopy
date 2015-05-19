### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Subdir processing order
#
SUBDIRS  =
SUBDIRS += datasource
if FILTERING_ENABLED
SUBDIRS += filter
endif
#SUBDIRS += message
#SUBDIRS += output
#SUBDIRS += .




TESTS =
check_PROGRAMS =



### How to make C test programFirst process all required subdirs
#
#TESTS          += test1
#check_PROGRAMS += test1
#test1_SOURCES   = test1.c
#test1_LDADD     = $(top_builddir)/src/libsnoopy_no_execve.la
