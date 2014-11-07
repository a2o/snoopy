include   $(top_srcdir)/build/Makefile.am.common

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

# Process etc/ before other subdirs
# If snoopy has config file enabled, then there is a small time frame
# between installation of library and subsequent installation of
# initial configuration file where iniparser emits an error about
# configuration file not being found. Therefore we first install
# configuration file and only then the snoopy library itself.
SUBDIRS = \
	etc \
	src \
	bin

EXTRA_DIST +=\
	autogen.sh

enable: install
	echo ; \
	$(top_srcdir)/bin/snoopy-enable ; \
	echo ;

disable:
	echo ; \
	$(top_srcdir)/bin/snoopy-disable \
	echo ;

# Disable it if we are uninstalling it, to avoid error messages
# about missing library that should be preloaded.
uninstall-local: disable
