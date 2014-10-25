#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Snoopy Logger], [2.0.0rc6], [https://github.com/a2o/snoopy/], [snoopy])
AC_DEFINE(PACKAGE_URL, [], "https://github.com/a2o/snoopy/")
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

dnl We are using automake and libtool
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 gnu silent-rules subdir-objects -Wall -Werror])
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

dnl Checks for libraries.
dnl FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])

dnl Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h syslog.h unistd.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([getcwd getsid strstr])

dnl Generate these (Make)files
AC_CONFIG_FILES([Makefile
                 bin/Makefile
                 contrib/sles/snoopy.spec
                 etc/Makefile
                 src/Makefile
                 src/lib/Makefile
                 src/filter/Makefile
                 src/input/Makefile])



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
AC_ARG_ENABLE(root-only,
    [AC_HELP_STRING(
	[--enable-root-only],
	[only log commands executed by root [default=disabled]]
    )],
    [
	if test "$enableval" == "yes"; then
	    enable_root_only=yes
	fi
    ],
    [enable_root_only=no]
)

AS_IF([test "x$enable_root_only" == "xyes"], [
    AC_DEFINE(SNOOPY_ROOT_ONLY, 1, [Only log commands executed by root])
])

AC_SUBST(SNOOPY_ROOT_ONLY, ["$enable_root_only"])



dnl ============================================================================
AC_ARG_WITH(message-format,
    [AC_HELP_STRING(
	[--with-message-format=FORMAT],
	[format to use to log messages with [default=see src/snoopy.h]]
    )],
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_LOG_MESSAGE_FORMAT_custom, "$withval", [Custom message format to use])
)



dnl ============================================================================
AC_ARG_ENABLE(filter,
    [AC_HELP_STRING(
        [--enable-filter],
        [enable general message filtering [default=disabled]]
    )],
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_FILTER_ENABLED, "1", [Enable filtering])
)



dnl ============================================================================
AC_ARG_WITH(filter-chain,
    [AC_HELP_STRING(
        [--with-filter-chain=CHAINSPEC],
        [filter chain to use [default=see src/snoopy.h and src/filter/ for available filters and their usage]]
    )],
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_FILTER_CHAIN_custom, "$withval", [Custom filter chain to use])
)



dnl ============================================================================
AC_ARG_WITH(syslog-facility,
    [AC_HELP_STRING(
	[--with-syslog-facility=FACILITY],
	[facility to use when logging, check 'man 3 syslog' or 'man openlog' [default=LOG_AUTHPRIV]]
    )],
    [],
    [with_syslog_facility=LOG_AUTHPRIV]
)

AC_DEFINE_UNQUOTED(SNOOPY_SYSLOG_FACILITY, [$with_syslog_facility], [Syslog facility to use])



dnl ============================================================================
AC_ARG_WITH(syslog-level,
    [AC_HELP_STRING(
	[--with-syslog-level=LEVEL],
	[facility to use when logging, check 'man 3 syslog' or 'man openlog' [default=LOG_INFO]]
    )],
    [],
    [with_syslog_level=LOG_INFO]
)

AC_DEFINE_UNQUOTED(SNOOPY_SYSLOG_LEVEL, [$with_syslog_level], [Syslog level to use])



dnl ============================================================================
AC_ARG_WITH(config-file,
    [AC_HELP_STRING(
        [--with-config-file=PATH],
        [enable INI configuration file parsing [default=disabled, if enabled then default path is SYSCONFDIR/snoopy.ini]]
    )],
    [
        if test "$with_config_file" == "yes"; then
            with_config_file_path="$SYSCONFDIR/snoopy.ini"
        else
            with_config_file_path="$with_config_file"
        fi
        AC_DEFINE_UNQUOTED(SNOOPY_CONF_CONFIG_FILE, "$with_config_file_path", [INI configuration file path to use])
    ]
)



dnl ============================================================================
AC_OUTPUT
