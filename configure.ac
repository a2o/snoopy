#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Snoopy Logger], [1.9.0], [https://github.com/a2o/snoopy/], [snoopy])
AC_DEFINE(PACKAGE_URL, [], "https://github.com/a2o/snoopy/")
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# We are using automake and libtool
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 gnu silent-rules subdir-objects -Wall -Wno-portability])
LT_INIT([disable-static])
#LT_INIT

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
    src/Makefile
    src/input/Makefile
    bin/Makefile
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
