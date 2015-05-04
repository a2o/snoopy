#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Snoopy Logger], [2.3.0rc4], [https://github.com/a2o/snoopy/], [snoopy])
AC_DEFINE(PACKAGE_URL,       "https://github.com/a2o/snoopy")
AC_DEFINE(PACKAGE_BUGREPORT, "https://github.com/a2o/snoopy/issues")
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

dnl We are using automake and libtool
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 gnu silent-rules subdir-objects -Wall -Werror])

dnl Option AM_PROG_AR
dnl
dnl Required for building Snoopy from git on Ubuntu 14.04 (for development, and
dnl by package build receipe. It must appear before LT_INIT.
dnl
dnl Snoopy development priorities:
dnl - git/master version is targeted at modern Linux distributions - this
dnl     means we mostly care if git/master is buildable on fresh/latest-version
dnl     distros (ignore building on legacy systems here);
dnl - source distribution packages are built on modern Linux distributions, thus
dnl     what git/master contains follows this assumption;
dnl - building Snoopy from source packages should be possible on as
dnl     many Linux distros as feasible;
dnl - compatibility for building distro-specific packages comes in only here and
dnl     burden of bending the Snoopy source to distro-packaging-specific needs
dnl     is placed on distro-specific package maintainer.
dnl
dnl Given the priorities above, we include AM_PROG_AR as dependency, as it:
dnl - is needed to build Snoopy from source on Ubuntu, which is somewhat used for
dnl     Snoopy development;
dnl - it does not interfere when building Snoopy from source packages;
dnl - it only interferes when building Snoopy from git, and only on older
dnl     platforms (RH/CentOS 6, see GitHub issue #38).
dnl
dnl Should someone have problems with AM_PROG_AR, they should either:
dnl - figure a way to conditionally remove it from configure.ac on certain distros, or
dnl - comment it out manually.
dnl
dnl The removal was considered for Snoopy 2.3.0, but was ultimately rejected because
dnl of arguments specified above.
dnl
AM_PROG_AR

dnl Enable libtool
LT_INIT([disable-static])

dnl Checks for programs.
dnl
dnl DO NOT REMOVE: CXX, AWK, CPP, RANLIB
dnl     Sometimes autoscan will suggest removing them, next time it will change
dnl     its mind and suggest putting them back in. Let's just keep them and be
dnl     done with it already.
dnl
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_GREP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl AC_PROG_RANLIB
dnl
dnl autogen.sh output:
dnl   libtoolize: `AC_PROG_RANLIB' is rendered obsolete by `LT_INIT'
dnl autoscan output:
dnl   configure.ac: warning: missing AC_PROG_RANLIB wanted by: ltmain.sh:2455
dnl
dnl Libtoolize warning seems less harmful, therefore we leave this option in, for now.
dnl
AC_PROG_RANLIB

AC_PROG_SED


dnl Checks for libraries.
dnl FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])

dnl Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h sys/socket.h syslog.h unistd.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UID_T

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([ \
    getcwd    \
    getsid    \
    memmove   \
    memset    \
    socket    \
    strchr    \
    strdup    \
    strstr    \
    strtol    \
])

dnl Substitute these variables when creating (Make)files below - GENERIC
AC_SUBST([PREFIX])
AC_SUBST([SYSCONFDIR])

dnl Substitute these variables when creating (Make)files below - CUSTOM
dnl $libdir        contains '${exec_prefix}' if not specified as ./configure --libdir=...
dnl ${exec_prefix} contains 'NONE'           if not specified as ./configure --exec-prefix=...
SNOOPY_LIBDIR="`eval echo ${libdir} | sed -e s@^NONE@$prefix@ | sed -e s@^NONE@/usr/local@`"
AC_SUBST([SNOOPY_LIBDIR])

dnl Generate these (Make)files
AC_CONFIG_FILES([Makefile
                 bin/Makefile
                 contrib/rhel/snoopy.spec
                 contrib/sles/snoopy.spec
                 doc/Makefile
                 doc/internals/Makefile
                 etc/Makefile
                 src/Makefile
                 src/datasource/Makefile
                 src/filter/Makefile
                 src/lib/Makefile
                 src/output/Makefile])
AC_CONFIG_FILES([bin/snoopy-disable], [chmod +x bin/snoopy-disable])
AC_CONFIG_FILES([bin/snoopy-enable],  [chmod +x bin/snoopy-enable])



dnl ============================================================================

dnl Decide where things are installed
if test "x$prefix" = "xNONE" ; then
    PREFIX="/usr/local"
else
    PREFIX="$prefix"
fi

if test "x$sysconfdir" = "x\${prefix}/etc" ; then
    SYSCONFDIR="$PREFIX/etc"
else
    SYSCONFDIR="$sysconfdir"
fi



dnl ============================================================================
AC_ARG_ENABLE(config-file,
    [AC_HELP_STRING(
        [--enable-config-file],
        [enable INI configuration file parsing. If enabled, then path is SYSCONFDIR/snoopy.ini [default=disabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            config_file_path="$SYSCONFDIR/snoopy.ini"
        elif test "x$enableval" = "xno"; then
            config_file_path=""
        else
            AC_MSG_ERROR([--enable-config-file does not take an argument: $enableval])
        fi
    ],
    # Not specified on command line
    [config_file_path=""]
)

AS_IF([test "x$config_file_path" != "x"], [
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_CONFIG_FILE, "$config_file_path", [INI configuration file path to use])
])

dnl CONFIG_FILE_ENABLED is used in:
dnl - src/Makefile.am
dnl - etc/Makefile.am
AM_CONDITIONAL([CONFIG_FILE_ENABLED], [test "x$config_file_path" != "x"])



dnl ============================================================================
AC_ARG_ENABLE(error-logging,
    [AC_HELP_STRING(
        [--enable-error-logging],
        [enable error logging through syslog - use only if you are developing snoopy or having problems configuring it [default=disabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            enable_error_logging=yes   # Specified as --enable-error-logging
        elif test "x$enableval" = "xno"; then
            enable_error_logging=no    # Specified as --disable-error-logging
        else
            AC_MSG_ERROR([--enable-error-logging does not take an argument: $enableval])
        fi
    ],
    [enable_error_logging=no]          # Unspecified as ./configure argument
)

AS_IF([test "x$enable_error_logging" = "xyes"], [
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_ERROR_LOGGING_ENABLED, 1, [Enable error logging])
])



dnl ============================================================================
AC_ARG_ENABLE(root-only,
    [AC_HELP_STRING(
        [--enable-root-only],
        [only log commands executed by root [default=disabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            enable_root_only=yes
        elif test "x$enableval" = "xno"; then
            enable_root_only=no
        else
            AC_MSG_ERROR([--enable-root-only does not take an argument: $enableval])
        fi
    ],
    [enable_root_only=no]
)

AS_IF([test "x$enable_root_only" = "xyes"], [
    AC_DEFINE(SNOOPY_CONF_ROOT_ONLY, 1, [Only log commands executed by root])
])



dnl ============================================================================
AC_ARG_WITH(message-format,
    [AC_HELP_STRING(
        [--with-message-format=FORMAT],
        [format to use to log messages with. For default value and available options, see etc/snoopy.ini and src/snoopy.h]
    )],
    [
        if test "x$withval" = "x" -o "x$withval" = "xyes"; then
            AC_MSG_ERROR([--with-message-format requires an appropriate value (message format specification). See etc/snoopy.ini for syntax. To use default log message format, remove this argument from ./configure line.])
        elif test "x$withval" = "xno"; then
            AC_MSG_ERROR([using --without-message-format is not supported, log message format is required, or omit this argument altogether to enable default log message format.])
        else
            with_message_format="$withval"
        fi
    ],
    [with_message_format=""]
)

AS_IF([test "x$with_message_format" != "x"], [
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_LOG_MESSAGE_FORMAT_custom, "$with_message_format", [Custom message format to use])
])



dnl ======= ENABLE FILTERING ===================================================
AC_ARG_ENABLE(filtering,
    [AC_HELP_STRING(
        [--enable-filtering],
        [enable general message filtering [default=disabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            enable_filtering=yes
        elif test "x$enableval" = "xno"; then
            enable_filtering=no
        else
            AC_MSG_ERROR([--enable-filtering does not take an argument: $enableval])
        fi
    ],
    [enable_filtering=no]
)

AS_IF([test "x$enable_filtering" = "xyes"], [
    AC_DEFINE(SNOOPY_CONF_FILTERING_ENABLED, 1, [Enable filtering])
])
AM_CONDITIONAL([FILTERING_ENABLED], [test "x$enable_filtering" == "xyes"])



dnl ============================================================================
dnl ======= ENABLE FILTERING ===================================================
dnl ============================================================================
AC_ARG_ENABLE(filtering,
    [AC_HELP_STRING(
        [--enable-filtering],
        [enable general message filtering [default=disabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            enable_filtering=yes
        elif test "x$enableval" = "xno"; then
            enable_filtering=no
        else
            AC_MSG_ERROR([--enable-filtering does not take an argument: $enableval])
        fi
    ],
    [enable_filtering=no]
)

AS_IF([test "x$enable_filtering" = "xyes"], [
    AC_DEFINE(SNOOPY_CONF_FILTERING_ENABLED, 1, [Enable filtering])
])
AM_CONDITIONAL([FILTERING_ENABLED], [test "x$enable_filtering" == "xyes"])



dnl ============================================================================
dnl ======= WITH FILTER CHAIN ==================================================
dnl ============================================================================
AC_ARG_WITH(filter-chain,
    [AC_HELP_STRING(
        [--with-filter-chain=CHAINSPEC],
        [default filter chain to use. See src/snoopy.h and src/filter/ for available filters [default=empty]]
    )],
    [
        if test "x$withval" = "x" -o "x$withval" = "xno"; then
            # We allow it here, as it might be desired to not have any
            # filter chain configured by default, but will be configured
            # later, using configuration file.
            # Allowed: empty, no
            with_filter_chain=""
        elif test "x$withval" = "xyes"; then
            # This must be an error.
            AC_MSG_ERROR([--with-filter-chain requires an appropriate value (filter chain specification), even empty string will do (--with-filter-chain=""). To disable filtering by default, use --without-filter-chain. Filter chain can later be specified and consequentially enabled by using configuration file. For this to work, configuration file (--with-config-file) and general filtering (--enable-filter) must be enabled.])
        else
            with_filter_chain="$withval"
        fi
    ],
    [with_filter_chain=""]
)

AS_IF([test "x$with_filter_chain" != "x"], [
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_FILTER_CHAIN_custom, "$with_filter_chain", [Custom filter chain to use])
])





dnl ============================================================================
dnl ============================================================================
dnl ======= START individual 'ENABLE FILTER' section ===========================
dnl ============================================================================
dnl ============================================================================

dnl ======= ENABLE FILTER: exclude_spawns_of ============================================
AC_ARG_ENABLE(filter-exclude_spawns_of,
    [AC_HELP_STRING(
        [--disable-filter-exclude_spawns_of],
        [enable filter exclude_spawns_of [default=enabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            enable_filter_exclude_spawns_of=yes
        elif test "x$enableval" = "xno"; then
            enable_filter_exclude_spawns_of=no
        else
            AC_MSG_ERROR([--enable-filter-* does not take an argument: $enableval])
        fi
    ],
    [enable_filter_exclude_spawns_of=yes]
)

AS_IF([test "x$enable_filter_exclude_spawns_of" = "xyes"], [
    AC_DEFINE(SNOOPY_CONF_FILTER_ENABLED_exclude_spawns_of, 1, [Is filter exclude_spawns_of enabled?])
])
AM_CONDITIONAL([ENABLE_FILTER_exclude_spawns_of], [test "x$enable_filter_exclude_spawns_of" == "xyes"])

dnl ======= ENABLE FILTER: exclude_uid ============================================
AC_ARG_ENABLE(filter-exclude_uid,
    [AC_HELP_STRING(
        [--disable-filter-exclude_uid],
        [enable filter exclude_uid [default=enabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            enable_filter_exclude_uid=yes
        elif test "x$enableval" = "xno"; then
            enable_filter_exclude_uid=no
        else
            AC_MSG_ERROR([--enable-filter-* does not take an argument: $enableval])
        fi
    ],
    [enable_filter_exclude_uid=yes]
)

AS_IF([test "x$enable_filter_exclude_uid" = "xyes"], [
    AC_DEFINE(SNOOPY_CONF_FILTER_ENABLED_exclude_uid, 1, [Is filter exclude_uid enabled?])
])
AM_CONDITIONAL([ENABLE_FILTER_exclude_uid], [test "x$enable_filter_exclude_uid" == "xyes"])

dnl ======= ENABLE FILTER: only_root ============================================
AC_ARG_ENABLE(filter-only_root,
    [AC_HELP_STRING(
        [--disable-filter-only_root],
        [enable filter only_root [default=enabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            enable_filter_only_root=yes
        elif test "x$enableval" = "xno"; then
            enable_filter_only_root=no
        else
            AC_MSG_ERROR([--enable-filter-* does not take an argument: $enableval])
        fi
    ],
    [enable_filter_only_root=yes]
)

AS_IF([test "x$enable_filter_only_root" = "xyes"], [
    AC_DEFINE(SNOOPY_CONF_FILTER_ENABLED_only_root, 1, [Is filter only_root enabled?])
])
AM_CONDITIONAL([ENABLE_FILTER_only_root], [test "x$enable_filter_only_root" == "xyes"])

dnl ======= ENABLE FILTER: only_uid ============================================
AC_ARG_ENABLE(filter-only_uid,
    [AC_HELP_STRING(
        [--disable-filter-only_uid],
        [enable filter only_uid [default=enabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            enable_filter_only_uid=yes
        elif test "x$enableval" = "xno"; then
            enable_filter_only_uid=no
        else
            AC_MSG_ERROR([--enable-filter-* does not take an argument: $enableval])
        fi
    ],
    [enable_filter_only_uid=yes]
)

AS_IF([test "x$enable_filter_only_uid" = "xyes"], [
    AC_DEFINE(SNOOPY_CONF_FILTER_ENABLED_only_uid, 1, [Is filter only_uid enabled?])
])
AM_CONDITIONAL([ENABLE_FILTER_only_uid], [test "x$enable_filter_only_uid" == "xyes"])

dnl ============================================================================
dnl ============================================================================
dnl ======= END individual 'ENABLE FILTER' section =============================
dnl ============================================================================
dnl ============================================================================





dnl ============================================================================
AC_ARG_WITH(syslog-facility,
    [AC_HELP_STRING(
        [--with-syslog-facility=FACILITY],
        [syslog facility to use when logging. Check 'man 3 syslog' or 'man openlog' for supported values [default=LOG_AUTHPRIV]]
    )],
    [
        if test "x$withval" = "x" -o "x$withval" = "xyes"; then
            # Enabled, but without explicit value
            AC_MSG_ERROR([--with-syslog-facility requires an appropriate LOG_* value])
        elif test "x$withval" = "xno"; then
            # Explicitly disabled: --without-...
            AC_MSG_ERROR([using --without-syslog-facility is not supported, syslog log facility is required. Either define it explicitly, or, to use the default value, remove this argument from ./configure line.])
        else
            # Enabled with explicit value definition
            with_syslog_facility_val="$withval"

            # Check for value correctness
            syslog_facility_short=`echo "$with_syslog_facility_val" | awk '{print toupper($0)}' | sed -e 's/^LOG_//' | grep -E '^(AUTH|AUTHPRIV|CRON|DAEMON|FTP|KERN|LOCAL[0-7]|LPR|MAIL|NEWS|SYSLOG|USER|UUCP)$'`
            if test "x$syslog_facility_short" = "x"; then
                AC_MSG_ERROR([--with-syslog-facility value provided is invalid: $with_syslog_facility_val])
            fi

            # Prefix value with LOG_
            syslog_facility="LOG_$syslog_facility_short"
        fi
    ],
    # Not specified on ./configure line
    [syslog_facility="LOG_AUTHPRIV"]
)

AC_DEFINE_UNQUOTED(SNOOPY_CONF_SYSLOG_FACILITY, [$syslog_facility], [Syslog facility to use by default])



dnl ============================================================================
AC_ARG_WITH(syslog-level,
    [AC_HELP_STRING(
        [--with-syslog-level=LEVEL],
        [syslog level to use when logging. Check 'man 3 syslog' or 'man openlog' for supported values [default=LOG_INFO]]
    )],
    [
        if test "x$withval" = "x" -o "x$withval" = "xyes"; then
            # Enabled, but without explicit value
            AC_MSG_ERROR([--with-syslog-level requires an appropriate LOG_* value])
        elif test "x$withval" = "xno"; then
            # Explicitly disabled: --without-...
            AC_MSG_ERROR([using --without-syslog-level is not supported, syslog log level is required. Either define it explicitly, or, to use the default value, remove this argument from ./configure line.])
        else
            # Enabled with explicit value definition
            with_syslog_level_val="$withval"

            # Check for value correctness
            syslog_level_short=`echo "$with_syslog_level_val" | awk '{print toupper($0)}' | sed -e 's/^LOG_//' | grep -E '^(EMERG|ALERT|CRIT|ERR|WARNING|NOTICE|INFO|DEBUG)$'`
            if test "x$syslog_level_short" = "x"; then
                AC_MSG_ERROR([--with-syslog-level value provided is invalid: $with_syslog_level_val])
            fi

            # Prefix value with LOG_
            syslog_level="LOG_$syslog_level_short"
        fi
    ],
    # Not specified on command line
    [syslog_level="LOG_INFO"]
)

AC_DEFINE_UNQUOTED(SNOOPY_CONF_SYSLOG_LEVEL, [$syslog_level], [Syslog level to use by default])



dnl ============================================================================
AC_OUTPUT
