include   $(top_srcdir)/build/Makefile.am.common

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST +=\
    snoopy.ini

if CONFIG_FILE_ENABLED
install-exec-local:
	if [ -f $(sysconfdir)/snoopy.ini ]; then \
	    echo ; \
	    echo "CONFIG FILE EXISTS, skipping: $(sysconfdir)/snoopy.ini" ; \
	    echo ; \
	else \
	    echo ; \
	    echo "Installing: $(sysconfdir)/snoopy.ini" ; \
	    echo ; \
	    install snoopy.ini $(sysconfdir)/snoopy.ini ; \
	fi
endif
