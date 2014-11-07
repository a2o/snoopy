#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Snoopy Logger], [2.0.1], [https://github.com/a2o/snoopy/], [snoopy])
AC_DEFINE(PACKAGE_URL, [], "https://github.com/a2o/snoopy/")
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

dnl We are using automake and libtool
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 gnu silent-rules subdir-objects -Wall -Werror])
AM_PROG_AR   dnl Required by Ubuntu 14.04 build receipe, must appear before LT_INIT
LT_INIT([disable-static])

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_SED

dnl Checks for libraries.
dnl FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])

dnl Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h syslog.h unistd.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UID_T

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([ \
    getcwd \
    getsid \
    memset \
    strchr \
    strdup \
    strstr \
    strtol \
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
                 etc/Makefile
                 src/Makefile
                 src/lib/Makefile
                 src/filter/Makefile
                 src/input/Makefile])
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
        [enable INI configuration file parsing [default=disabled, if enabled then path is SYSCONFDIR/snoopy.ini]]
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

dnl CONFIG_FILE_ENABLED is used in etc/Makefile.am
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
        [format to use to log messages with [default=see src/snoopy.h]]
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



dnl ============================================================================
AC_ARG_ENABLE(filter,
    [AC_HELP_STRING(
        [--enable-filter],
        [enable general message filtering [default=disabled]]
    )],
    [
        if test "x$enableval" = "xyes"; then
            enable_filter=yes
        elif test "x$enableval" = "xno"; then
            enable_filter=no
        else
            AC_MSG_ERROR([--enable-filter does not take an argument: $enableval])
        fi
    ],
    [enable_filter=no]
)

AS_IF([test "x$enable_filter" = "xyes"], [
    AC_DEFINE(SNOOPY_CONF_FILTER_ENABLED, 1, [Enable filtering])
])



dnl ============================================================================
AC_ARG_WITH(filter-chain,
    [AC_HELP_STRING(
        [--with-filter-chain=CHAINSPEC],
        [default filter chain to use [default=see src/snoopy.h and src/filter/ for available filters and their usage]]
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
AC_ARG_WITH(syslog-facility,
    [AC_HELP_STRING(
        [--with-syslog-facility=FACILITY],
        [facility to use when logging, check 'man 3 syslog' or 'man openlog' [default=LOG_AUTHPRIV]]
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
        fi
    ],
    # Not specified on ./configure line
    [with_syslog_facility_val="LOG_AUTHPRIV"]
)

AC_DEFINE_UNQUOTED(SNOOPY_CONF_SYSLOG_FACILITY, [$with_syslog_facility_val], [Syslog facility to use])



dnl ============================================================================
AC_ARG_WITH(syslog-level,
    [AC_HELP_STRING(
        [--with-syslog-level=LEVEL],
        [facility to use when logging, check 'man 3 syslog' or 'man openlog' [default=LOG_INFO]]
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
        fi
    ],
    # Not specified on command line
    [with_syslog_level_val="LOG_INFO"]
)

AC_DEFINE_UNQUOTED(SNOOPY_CONF_SYSLOG_LEVEL, [$with_syslog_level_val], [Syslog level to use])



dnl ============================================================================
AC_OUTPUT
