#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Snoopy Logger], [1.7.2], [http://sourceforge.net/projects/snoopylogger/], [snoopy])
AC_DEFINE(PACKAGE_URL, "http://sourceforge.net/projects/snoopylogger/")
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

AC_CONFIG_FILES([Makefile])



dnl ============================================================================
AC_ARG_ENABLE(cwd-logging,
    [AC_HELP_STRING([--enable-cwd-logging],
	[enable logging of Current Working Directory [default=yes]]
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
AC_OUTPUT
