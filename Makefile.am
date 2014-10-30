include   $(top_srcdir)/build/Makefile.am.common

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src bin etc

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
