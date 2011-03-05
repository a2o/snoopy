#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Snoopy Logger], [1.7.10], [http://sourceforge.net/projects/snoopylogger/], [snoopy])
AC_DEFINE(PACKAGE_URL, [], "http://sourceforge.net/projects/snoopylogger/")
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

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
    contrib/sles/snoopy.spec
])



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



dnl ============================================================================
AC_ARG_WITH(filter-command,
    [AC_HELP_STRING(
	[--with-filter-command=CMD],
	[external filter command to use [default=/usr/local/sbin/snoopy-filter.sh]]
    )],
    [],
    [with_filter_command=/usr/local/sbin/snoopy-filter.sh]
)

AS_IF([test "x$enable_external_filter" == "xyes"], [
    AC_DEFINE_UNQUOTED(SNOOPY_EXTERNAL_FILTER_COMMAND, ["$with_filter_command"], [External filter command to use])
])



dnl ============================================================================
AC_ARG_WITH(syslog-facility,
    [AC_HELP_STRING(
	[--with-syslog-facility=FACILITY],
	[facility to use when logging, check 'man 3 syslog' or 'man openlog' [default=LOG_AUTHPRIV]]
    )],
    [],
    [with_syslog_facility=LOG_AUTHPRIV]
)

AC_DEFINE_UNQUOTED(SNOOPY_SYSLOG_FACILITY, [$with_syslog_facility], [External filter command to use])



dnl ============================================================================
AC_OUTPUT
