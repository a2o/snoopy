include   $(top_srcdir)/build/Makefile.am.common

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST +=\
    snoopy.ini

if CONFIG_FILE_ENABLED
install-exec-local:
	if [ ! -f $(sysconfdir)/snoopy.ini ]; then \
	    echo ; \
	    echo "Installing configuration file: $(sysconfdir)/snoopy.ini" ; \
	    echo ; \
	    install snoopy.ini $(sysconfdir)/snoopy.ini ; \
	else \
	    if diff snoopy.ini $(sysconfdir)/snoopy.ini; then \
	        echo ; \
	        echo "No changes to configuration file are required: $(sysconfdir)/snoopy.ini" ; \
	        echo ; \
	    else \
	        install snoopy.ini $(sysconfdir)/snoopy.ini.NEW ; \
	        echo ; \
	        echo "Existing config file: $(sysconfdir)/snoopy.ini" ; \
	        echo "     New config file: $(sysconfdir)/snoopy.ini.NEW" ; \
	        echo ; \
	        echo "WARNING: CONFIGURATION FILE CHANGES NEED MANUAL REVIEW" ; \
	        echo ; \
	    fi \
	fi
endif
