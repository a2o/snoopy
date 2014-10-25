include   $(top_srcdir)/build/Makefile.am.common

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

install-exec-local:
	if [ -f $(sysconfdir)/snoopy.ini ]; then \
	    echo ; \
	    echo "CONFIG FILE EXISTS, skipping: $(sysconfdir)/snoopy.ini" ; \
	    echo ; \
	else \
	    echo "Installing: $(sysconfdir)/snoopy.ini" ; \
	    install snoopy.ini $(sysconfdir)/snoopy.ini ; \
	fi
