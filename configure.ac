#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Snoopy Logger], [1.8.0], [http://sourceforge.net/projects/snoopylogger/], [snoopy])
AC_DEFINE(PACKAGE_URL, [], "http://sourceforge.net/projects/snoopylogger/")
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([getcwd])

AC_CONFIG_FILES([
    Makefile
    snoopy-filter.sh
    contrib/sles/snoopy.spec
])



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
AC_ARG_ENABLE(cwd-logging,
    [AC_HELP_STRING(
	[--disable-cwd-logging],
	[disable logging of Current Working Directory [default=enabled]]
    )],
    [
	if test "$enableval" == "yes"; then
	    enable_cwd_logging=yes
	fi
    ],
    [enable_cwd_logging=yes]
)

AS_IF([test "x$enable_cwd_logging" == "xyes"], [
    AC_DEFINE(SNOOPY_CWD_LOGGING, 1, [Enable logging of Current Working Directory])
])



dnl ============================================================================
AC_ARG_WITH(exclude-prefix,
    [AC_HELP_STRING(
	[--with-exclude-prefix=PREFIX],
	[prefix to commands excluded from logging.
	    This feature is mainly intended to specify path to commands which are used by external filter so snoopy does not enter infinite loop.
	    WARNING WARNING WARNING: Do not change unless you are absolutely sure you know what you are doing
	    [default=/usr/local/snoopy/bin]
	]
    )],
    [],
    [with_exclude_prefix=/usr/local/snoopy/bin]
)

AC_DEFINE_UNQUOTED(SNOOPY_EXCLUDE_PREFIX, ["$with_exclude_prefix"], [Prefix to commands excluded from logging])

AC_SUBST(SNOOPY_EXCLUDE_PREFIX, ["$with_exclude_prefix"])



dnl ============================================================================
AC_ARG_ENABLE(external-filter,
    [AC_HELP_STRING(
	[--enable-external-filter],
	[enable external filtering support [default=disabled]]
    )],
    [
	if test "$enableval" == "yes"; then
	    enable_external_filter=yes
	fi
    ],
    [enable_external_filter=no]
)

AS_IF([test "x$enable_external_filter" == "xyes"], [
    AC_DEFINE(SNOOPY_EXTERNAL_FILTER, 1, [Enable external filtering support])
])

AC_SUBST(SNOOPY_EXTERNAL_FILTER, ["$enable_external_filter"])



dnl ============================================================================
AC_ARG_WITH(filter-command,
    [AC_HELP_STRING(
	[--with-filter-command=CMD],
	[external filter command to use.
	    WARNING WARNING WARNING: Do not change unless you are absolutely sure you know what you are doing
	    [default=/usr/local/snoopy/bin/snoopy-filter.sh]
	]
    )],
    [],
    [with_filter_command=/usr/local/snoopy/bin/snoopy-filter.sh]
)

AS_IF(
    [test "x$enable_external_filter" == "xyes"],
    [
        AS_IF(
	    [test `echo "$with_filter_command" | grep -c "^$with_exclude_prefix" | cat` == "0"],
	    [AC_ERROR([Invalid option combination: filter command $with_filter_command must match exclude prefix $with_exclude_prefix])]
	)
	AC_DEFINE_UNQUOTED(SNOOPY_EXTERNAL_FILTER_COMMAND, ["$with_filter_command"], [External filter command to use])
    ]
)

AC_SUBST(SNOOPY_FILTER_COMMAND, ["$with_filter_command"])



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
AC_OUTPUT

