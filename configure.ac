###
### Snoopy configure.ac
###



### Initialize autotools
#
AC_PREREQ([2.69])

# If you are running older OS and trying to build Snoopy from git,
# ./bootstrap.sh will change the line below from script-based version
# determination of Snoopy version to fixed string. DO NOT COMMIT THIS CHANGE!
# Pull requests with this change will be rejected.
# Correct line content that follows is:
#
#   m4_esyscmd_s(echo $(./dev-tools/libexec/get-release-version.sh)),
#
# PS: Since the upgrade to autoconf 2.69, AC_INIT definition can no longer span multiple lines.
#
AC_INIT([Snoopy Command Logger],[m4_esyscmd_s(echo $(./dev-tools/libexec/get-release-version.sh))],[https://github.com/a2o/snoopy/issues/],[snoopy],[https://github.com/a2o/snoopy/])


### Configure autotools
#
AC_CONFIG_SRCDIR([src/entrypoint/execve-wrapper.c])   # Check if we are in correct directory tree
AC_CONFIG_AUX_DIR([build/aux])
AC_CONFIG_MACRO_DIRS([build/m4])
m4_include([build/snoopy.m4])
AM_INIT_AUTOMAKE([1.11 gnu silent-rules subdir-objects -Wall -Werror])


### Silent make output
#
# Use it by default, but use version here that is backwards-compatible.
# Must be used after AM_INIT_AUTOMAKE
#AM_SILENT_RULES([yes])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])


dnl Option AM_PROG_AR
dnl
dnl Required for building Snoopy from git on Ubuntu (which is what is mainly
dnl used for development).
dnl
dnl Some older platforms do not provide this macro, and there might be some
dnl packaging issues (see issue #38 on GitHub), that is why we're including
dnl it conditionally here.
dnl
dnl This option must appear before LT_INIT.
dnl
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

dnl Enable libtool
LT_INIT([disable-static])



dnl Checks for programs.
dnl
dnl WARNING: Do not run autoscan over a non-gitclean-ed directory!
dnl
dnl If you run autoscan over a dirty directory, it adds the following macros 
dnl that are actually not required:
dnl - AC_PROG_CPP
dnl - AC_PROG_CXX
dnl - AC_PROG_LN_S
dnl - AC_PROG_RANLIB
dnl
AC_PROG_AWK
AC_PROG_CC
AC_PROG_GREP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_SED
LT_PATH_NM



### Check for programs required for TEST suite
#
SNOOPY_PROG_REQUIRE([dirname],  [DIRNAME],  [Needed for "make check" test suite.])
SNOOPY_PROG_REQUIRE([gcov],     [GCOV],     [Needed for "make coverage" build target.])
SNOOPY_PROG_REQUIRE([readlink], [READLINK], [Needed for "make check" test suite.])
SNOOPY_PROG_SUGGEST([socat],    [SOCAT],    [Needed for "make check" test suite.])
SNOOPY_PROG_SUGGEST([ps],       [PS],       [Needed for "make check" test suite.])
SNOOPY_PROG_REQUIRE([wc],       [WC],       [Needed for "make check" test suite.])



dnl Checks for libraries.
AC_CHECK_LIB([dl], [dlsym], [], [
    AC_MSG_ERROR([A library is missing. Unable to continue.])
])

dnl Checks for header files.
AC_CHECK_HEADERS([ \
    arpa/inet.h \
    ctype.h \
    dlfcn.h \
    errno.h \
    fcntl.h \
    features.h \
    grp.h \
    limits.h \
    pwd.h \
    stdio.h \
    stddef.h \
    stdlib.h \
    string.h \
    syslog.h \
    sys/socket.h \
    sys/stat.h \
    sys/syscall.h \
    sys/time.h \
    sys/types.h \
    sys/un.h \
    time.h \
    unistd.h \
    utmp.h \
], [], [
    AC_MSG_ERROR([A header file is missing. Unable to continue.])
])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T

dnl Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS([ \
    getcwd    \
    gethostname \
    getsid    \
    gettimeofday \
    localtime_r \
    socket    \
    strchr    \
    strdup    \
    strerror  \
    strndup    \
    strrchr   \
    strstr    \
], [], [
    AC_MSG_ERROR([A function is missing. Unable to continue.])
])

HOST_SUPPORTS_DATASOURCE_ipaddr="true"
AC_CHECK_FUNCS([ \
    getutline_r \
    utmpname \
], [], [
    AC_MSG_WARN([Function missing, cannot enable datasource ipaddr.])
    HOST_SUPPORTS_DATASOURCE_ipaddr="false"
])

dnl Substitute these variables when creating (Make)files below - GENERIC
AC_SUBST([PREFIX])
AC_SUBST([SYSCONFDIR])

dnl Substitute these variables when creating (Make)files below - CUSTOM
dnl $libdir        contains '${exec_prefix}' if not specified as ./configure --libdir=...
dnl ${exec_prefix} contains 'NONE'           if not specified as ./configure --exec-prefix=...
SNOOPY_LIBDIR="`eval echo ${libdir} | sed -e s@^NONE@$prefix@ | sed -e s@^NONE@/usr/local@`"
AC_SUBST([SNOOPY_LIBDIR])



### Generate these header files
#
AC_CONFIG_HEADERS([config.h])


### Generate these (Make)files
#
AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 etc/Makefile
                 etc/snoopy.ini
                 lib/Makefile
                 lib/inih/Makefile
                 lib/inih/src/Makefile
                 src/Makefile
                 src/action/Makefile
                 src/cli/Makefile
                 src/datasource/Makefile
                 src/entrypoint/Makefile
                 src/filter/Makefile
                 src/output/Makefile
                 src/util/Makefile
                 tests/Makefile
                 tests/bin/Makefile
                 tests/cli/Makefile
                 tests/combined/Makefile
                 tests/configfile/Makefile
                 tests/datasource/Makefile
                 tests/filter/Makefile
                 tests/general/Makefile
                 tests/message/Makefile
                 tests/output/Makefile
                 tests/threads/Makefile
                 tests/unit/Makefile
])
AC_CONFIG_FILES([tests/_bootstrap-paths.sh], [chmod +x tests/_bootstrap-paths.sh])



dnl ============================================================================
dnl ======= Determine target locations =========================================
dnl ============================================================================

if test "x$prefix" = "xNONE" ; then
    PREFIX="/usr/local"
else
    PREFIX="$prefix"
fi

if test "x$exec_prefix" = "xNONE" ; then
    EXEC_PREFIX="$PREFIX"
else
    EXEC_PREFIX="$exec_prefix"
fi

if test "x$sysconfdir" = "x\${prefix}/etc" ; then
    SYSCONFDIR="$PREFIX/etc"
else
    SYSCONFDIR="$sysconfdir"
fi

if test "x$libdir" = "x\${exec_prefix}/lib" ; then
    LIBDIR="$EXEC_PREFIX/lib"
else
    LIBDIR="$libdir"
fi

if test "x$sbindir" = "x\${exec_prefix}/sbin" ; then
    SBINDIR="$EXEC_PREFIX/sbin"
else
    SBINDIR="$sbindir"
fi





dnl ============================================================================
dnl ======= START CONFIGURATION SUMMARY OUTPUT =================================
dnl ============================================================================
AC_MSG_NOTICE([======================================================================])
AC_MSG_NOTICE([Snoopy version: AC_PACKAGE_VERSION])

AC_DEFINE_UNQUOTED(SNOOPY_CONFIGURE_COMMAND, "./configure $(echo $ac_configure_args | sed 's/\\/\\\\/g' | sed 's/"/\\"/g')", [Configure command that was used to build Snoopy])
AC_MSG_NOTICE([Snoopy build configuration command: ./configure $ac_configure_args])
AC_MSG_NOTICE([-])



# ============================================================================
# ======= ENABLE EVERYTHING ==================================================
# ============================================================================
AC_ARG_ENABLE(everything,
    [AS_HELP_STRING(
        [--enable-everything],
        [enable/disable all Snoopy features with one argument. Mainly useful for testing Snoopy builds. Individual features can still be enabled/disabled. [default=depends]]
    )],
[
    SNOOPY_CONFIGURE_ENABLE_EVERYTHING_SET([$enableval])],   # Specified on command line
    SNOOPY_CONFIGURE_ENABLE_EVERYTHING_SET([unspecified])    # Not specified on command line
)
AS_IF([test "x$enable_everything" = "xyes"],
    [AC_MSG_NOTICE([Snoopy all features enabled/disabled: yes - enabled])],
    [
        AS_IF([test "x$enable_everything" = "xno"],
            [AC_MSG_NOTICE([Snoopy all features enabled/disabled: yes - disabled])],
            [AC_MSG_NOTICE([Snoopy all features enabled/disabled: no - using defaults])]
        )
    ]
)



# ============================================================================
# ========= TARGET PATHS =====================================================
# ============================================================================
AC_MSG_NOTICE([Snoopy library directory:     $LIBDIR])
AC_MSG_NOTICE([Snoopy CLI tool directory:    $SBINDIR])

AC_DEFINE_UNQUOTED(SNOOPY_CONF_LIBDIR,  "$LIBDIR",  [Target installation directory for .so library])
AC_DEFINE_UNQUOTED(SNOOPY_CONF_SBINDIR, "$SBINDIR", [Target installation directory for the CLI tool])



dnl ============================================================================
dnl ======= ENABLE CONFIG FILE =================================================
dnl ============================================================================
AC_ARG_ENABLE(config-file,
    [AS_HELP_STRING(
        [--disable-config-file],
        [disable INI configuration file parsing. If enabled, then path is SYSCONFDIR/snoopy.ini [default=enabled] [default location=SYSCONFDIR/snoopy.ini]]
    )],
    SNOOPY_CONFIGURE_ENABLE_GENERIC_SPECIFIED(  [config-file], [configfile_enabled], [$enableval], [Use --sysconfdir to set config file path.]),
    SNOOPY_CONFIGURE_ENABLE_GENERIC_UNSPECIFIED([config-file], [configfile_enabled], [$enable_everything], [yes])
)

AC_MSG_NOTICE([Snoopy config file enabled:   $configfile_enabled])
AS_IF([test "x$configfile_enabled" == "xyes"], [
    configfile_path="$SYSCONFDIR/snoopy.ini"
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_CONFIGFILE_ENABLED, 1,                   [Is config file parsing enabled?])
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_CONFIGFILE_PATH,    "$configfile_path",  [INI configuration file path to use])
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_SYSCONFDIR, "$SYSCONFDIR",               [Target installation directory for the configuration file])
    AC_MSG_NOTICE([Snoopy config file path:      $configfile_path])
])

AM_CONDITIONAL([CONFIGFILE_ENABLED], [test "x$configfile_enabled" == "xyes"])



dnl ============================================================================
dnl ======= ENABLE ERROR LOGGING ===============================================
dnl ============================================================================
AC_ARG_ENABLE(error-logging,
    [AS_HELP_STRING(
        [--enable-error-logging],
        [enable error logging through configured output - use only if you are developing Snoopy or having problems configuring it [default=disabled]]
    )],
    SNOOPY_CONFIGURE_ENABLE_GENERIC_SPECIFIED(  [error-logging], [enable_error_logging], [$enableval], []),
    SNOOPY_CONFIGURE_ENABLE_GENERIC_UNSPECIFIED([error-logging], [enable_error_logging], [$enable_everything], [no])
)

AS_IF([test "x$enable_error_logging" = "xyes"], [
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_ERROR_LOGGING_ENABLED, 1, [Enable error logging])
])
AC_MSG_NOTICE([Snoopy error logging enabled: $enable_error_logging])



dnl ============================================================================
dnl ======= ENABLE THREAD SAFETY ===============================================
dnl ============================================================================
AC_ARG_ENABLE(thread-safety,
    [AS_HELP_STRING(
        [--disable-thread-safety],
        [Disable parts of code that ensure safe multi-threaded operation [default=enabled]]
    )],
    SNOOPY_CONFIGURE_ENABLE_GENERIC_SPECIFIED(  [thread_safety], [thread_safety_enabled], [$enableval], []),
    SNOOPY_CONFIGURE_ENABLE_GENERIC_UNSPECIFIED([thread_safety], [thread_safety_enabled], [$enable_everything], [yes])
)
SNOOPY_CONFIGURE_ENABLE_GENERIC_EVALUATE([$thread_safety_enabled], [THREAD_SAFETY_ENABLED], [thread safety])





dnl ============================================================================
dnl ============================================================================
dnl ======= START section: DATASOURCES & MESSAGE FORMAT ========================
dnl ============================================================================
dnl ============================================================================
AC_MSG_NOTICE([-])



# ==============================================================================
# ======= Enable/disable all data sources at once ==============================
# ==============================================================================
AC_ARG_ENABLE(all-datasources,
    [AS_HELP_STRING(
        [--disable-all-datasources],
        [disables all data sources at once. Use --enable-datasource-NAME to reenable individual data sources. [default=enabled]]
    )],
    SNOOPY_CONFIGURE_ENABLE_GENERIC_SPECIFIED(  [all-datasources], [enable_all_datasources], [$enableval], [Use --enable/--disable-datasource-NAME to reenable/disable individual data sources.]),
    SNOOPY_CONFIGURE_ENABLE_GENERIC_UNSPECIFIED([all-datasources], [enable_all_datasources], [$enable_everything], [unspecified])
)



# ==============================================================================
# ======= START section: ENABLE INDIVIDUAL DATA SOURCES ========================
# ==============================================================================
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [cgroup],         [cgroup information])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [cmdline],        [full command line including arguments])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [cwd],            [current working directory])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [datetime],       [current date and time, supports strftime-based formatting argument, defaults to ISO 8601 format])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [domain],         [domain of this system])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [egid],           [effective GID])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [egroup],         [effective literal group name])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [env],            [particular environment variable])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [env_all],        [all environment variables])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [euid],           [effective UID])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [eusername],      [effective literal username])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [filename],       [full filename of process being executed])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [gid],            [GID of process performing the execution])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [group],          [literal group name of process performing the execution])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [hostname],       [hostname of this system])
if test x"$HOST_SUPPORTS_DATASOURCE_ipaddr" = xfalse ; then
    SNOOPY_CONFIGURE_DATASOURCE_FORCEDISABLE( [ipaddr], [IP address of a connected terminal])
else
    SNOOPY_CONFIGURE_DATASOURCE_ENABLE(       [ipaddr], [IP address of a connected terminal])
fi
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [login],          [login username of user doing the execution])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [pid],            [PID of process performing the execution])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [ppid],           [parent PID of process performing the execution])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [rpname],         [real parent name found up the process tree])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [sid],            [session leader PID])
SNOOPY_CONFIGURE_DATASOURCE_FORCE(  [snoopy_configure_command], [returns the ./configure command that was used to build Snoopy])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [snoopy_literal], [dummy data source - only returns what it receives as argument])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [snoopy_threads], [number of threads that Snoopy currently is initialized for])
SNOOPY_CONFIGURE_DATASOURCE_FORCE(  [snoopy_version], [installed Snoopy version])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [systemd_unit_name], [SystemD unit name, as found in /proc/self/cgroup file under the name=systemd cgroup])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [tid],            [Thread ID of process performing the execution])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [tid_kernel],     [Thread ID of process performing the execution, as retured by kernel via syscall])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [timestamp],      [current Unix timestamp])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [timestamp_ms],   [millisecond part of current Unix timestamp])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [timestamp_us],   [microsecond part of current Unix timestamp])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [tty],            [path to TTY performing the execution])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [tty_uid],        [UID of TTY performing the execution],              [INCLUDE_tty__common])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [tty_username],   [literal username of TTY performing the execution], [INCLUDE_tty__common])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [uid],            [numeric UID that executed the process])
SNOOPY_CONFIGURE_DATASOURCE_ENABLE( [username],       [literal username of UID that executed the process])
# ==============================================================================
# ======= END section: ENABLE INDIVIDUAL DATA SOURCES ==========================
# ==============================================================================



dnl ============================================================================
dnl ======= DEFAULT MESSAGE FORMAT =============================================
dnl ============================================================================
AC_ARG_WITH(message-format,
    [AS_HELP_STRING(
        [--with-message-format=FORMAT],
        [format to use to log messages with. For default value and available options, see comments in etc/snoopy.ini for format specification.]
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
    [with_message_format="@<:@uid:%{uid} sid:%{sid} tty:%{tty} cwd:%{cwd} filename:%{filename}@:>@: %{cmdline}"]
)
AC_DEFINE_UNQUOTED(SNOOPY_CONF_MESSAGE_FORMAT, "$(echo $with_message_format | sed 's/\\/\\\\/g' | sed 's/"/\\"/g')", [Custom message format to use])
AC_SUBST([SNOOPY_CONF_MESSAGE_FORMAT], [$with_message_format])
AC_MSG_NOTICE([Snoopy default message format: "$with_message_format"])



dnl ============================================================================
dnl ============================================================================
dnl ======= END section: DATASOURCES & MESSAGE FORMAT ==========================
dnl ============================================================================
dnl ============================================================================





dnl ============================================================================
dnl ============================================================================
dnl ======= START section: FILTERING ===========================================
dnl ============================================================================
dnl ============================================================================
AC_MSG_NOTICE([-])



dnl ============================================================================
dnl ======= DEPRECATED FILTER: root-only =======================================
dnl ============================================================================
AC_ARG_ENABLE(root-only,
    [AS_HELP_STRING(
        [--enable-root-only],
        [DEPRECATED. Use --enable-filtering --with-filter-chain="only_uid:0" instead.]
    )],
    [
        AC_MSG_ERROR([Use of --enable-root-only is DEPRECATED. Use --enable-filtering --with-filter-chain="only_uid:0" instead.])
    ]
)



dnl ============================================================================
dnl ======= ENABLE FILTERING ===================================================
dnl ============================================================================
AC_ARG_ENABLE(filtering,
    [AS_HELP_STRING(
        [--disable-filtering],
        [disable general message filtering [default=enabled]]
    )],
    SNOOPY_CONFIGURE_ENABLE_GENERIC_SPECIFIED(  [filtering], [enable_filtering], [$enableval], []),
    SNOOPY_CONFIGURE_ENABLE_GENERIC_UNSPECIFIED([filtering], [enable_filtering], [$enable_everything], [yes])
)
SNOOPY_CONFIGURE_ENABLE_GENERIC_EVALUATE([$enable_filtering], [FILTERING_ENABLED], [filtering subsystem])



# ==============================================================================
# ======= Enable/disable all filters at once ===================================
# ==============================================================================
AC_ARG_ENABLE(all-filters,
    [AS_HELP_STRING(
        [--disable-all-filters],
        [disables all filters at once. This does not disable filtering in general. It only causes that none of filters is going to be built unless explicitly enabling it by specifying --enable-filter-NAME at ./confgure time. [default=enabled]]
    )],
    SNOOPY_CONFIGURE_ENABLE_GENERIC_SPECIFIED(  [all-filters], [enable_all_filters], [$enableval], []),
    SNOOPY_CONFIGURE_ENABLE_GENERIC_UNSPECIFIED([all-filters], [enable_all_filters], [$enable_filtering], [yes])
)



# ==============================================================================
# ======= START section: ENABLE INDIVIDUAL FILTERS =============================
# ==============================================================================
SNOOPY_CONFIGURE_FILTER_ENABLE( [exclude_spawns_of], [Drops messages that originate for specified process trees.])
SNOOPY_CONFIGURE_FILTER_ENABLE( [exclude_uid],       [Drops messages that match given UIDs.])
SNOOPY_CONFIGURE_FILTER_ENABLE( [only_root],         [Passes only messages generated by root.])
SNOOPY_CONFIGURE_FILTER_ENABLE( [only_tty],          [Passes only messages that do not originate from processes associated with a TTY - daemons, basically.])
SNOOPY_CONFIGURE_FILTER_ENABLE( [only_uid],          [Passes only messages that match given UIDs.])
# ==============================================================================
# ======= END section: ENABLE INDIVIDUAL FILTERS ===============================
# ==============================================================================



dnl ============================================================================
dnl ======= WITH FILTER CHAIN ==================================================
dnl ============================================================================
AC_ARG_WITH(filter-chain,
    [AS_HELP_STRING(
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

AC_DEFINE_UNQUOTED(SNOOPY_CONF_FILTER_CHAIN, "$with_filter_chain", [Filter chain to use])
AC_MSG_NOTICE([Snoopy default filter chain: ${with_filter_chain:-(none)}])



dnl ============================================================================
dnl ============================================================================
dnl ======= END section: FILTERING =============================================
dnl ============================================================================
dnl ============================================================================





dnl ============================================================================
dnl ============================================================================
dnl ======= START section: OUTPUTS =============================================
dnl ============================================================================
dnl ============================================================================
AC_MSG_NOTICE([-])



# ==============================================================================
# ======= Enable/disable all outputs at once ===================================
# ==============================================================================
AC_ARG_ENABLE(all-outputs,
    [AS_HELP_STRING(
        [--disable-all-outputs],
        [disables all outputs at once. Use --enable-output-NAME to reenable individual output. [default=enabled]]
    )],
    SNOOPY_CONFIGURE_ENABLE_GENERIC_SPECIFIED(  [all-outputs], [enable_all_outputs], [$enableval], [Use --enable/--disable-output-NAME to reenable/disable individual data sources.]),
    SNOOPY_CONFIGURE_ENABLE_GENERIC_UNSPECIFIED([all-outputs], [enable_all_outputs], [$enable_everything], [unspecified])
)



# ==============================================================================
# ======= START section: ENABLE INDIVIDUAL OUTPUTS =============================
# ==============================================================================
SNOOPY_CONFIGURE_OUTPUT_ENABLE(  [devlog],         [This is the default output. Writes to /dev/log in a non-blocking manner.])
SNOOPY_CONFIGURE_OUTPUT_ENABLE(  [devnull],        [This output is a black hole - messages are discarded.])
SNOOPY_CONFIGURE_OUTPUT_ENABLE(  [devtty],         [Writes to /dev/tty which will send the output to the current console.])
SNOOPY_CONFIGURE_OUTPUT_FORCE(   [file],           [Writes directly to a file. NOTICE: If applicable, make sure the file has proper permissions set for non-root users too.])
SNOOPY_CONFIGURE_OUTPUT_FORCE(   [socket],         [Writes to a socket.])
SNOOPY_CONFIGURE_OUTPUT_ENABLE(  [stderr],         [Writes to stderr.])
SNOOPY_CONFIGURE_OUTPUT_ENABLE(  [stdout],         [Writes to stdout.])
SNOOPY_CONFIGURE_OUTPUT_DISABLE( [syslog],         [WARNING: This output is causing system hangs with SystemD. Do not use it unless you are absolutely certain you know better. Similar to devlog output, but blocking.])
# ==============================================================================
# ======= END section: ENABLE INDIVIDUAL OUTPUTS ===============================
# ==============================================================================


dnl ============================================================================
AC_ARG_WITH(default-output,
    [AS_HELP_STRING(
        [--with-default-output=OUTPUT_SPEC],
        [default output to use. See snoopy.ini or src/output/* for available values. [default=devlog]]
    )],
    [
        if test "x$withval" = "x" -o "x$withval" = "xyes"; then
            # Enabled, but without explicit value
            AC_MSG_ERROR([--with-default-output requires an appropriate value])
        elif test "x$withval" = "xno"; then
            # Explicitly disabled: --without-...
            AC_MSG_ERROR([using --without-default-output is not supported, as it makes no sense. Either define it explicitly, or, to use the default value, remove this argument from ./configure line.])
        else
            # Enabled with explicit value definition
            default_output_val="$withval"

            # Separate output from argument
            if echo "$default_output_val" | grep ':' > /dev/null ; then
                default_output=`echo "$default_output_val" | cut -d: -f1`
                default_output_arg=`echo "$default_output_val" | cut -d: -f2-`
            else
                default_output="$default_output_val"
                default_output_arg=""
            fi

            # Strip "output" suffix if exists
            if ! echo "$default_output" | grep 'output$' > /dev/null ; then
                default_output=`echo "$default_output" | sed -e 's/output$//'`
            fi

            # Check if given output exist
            if test ! -f src/output/${default_output}output.c ; then
                AC_MSG_ERROR([--with-default-output value provided is invalid, output does not exist: $default_output])
            fi
        fi
    ],

    # Not specified on ./configure line, decided in snoopy.h
    [
        default_output=""
        default_output_arg=""
    ]
)

AS_IF([test "x$default_output" != "x"], [
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_OUTPUT_DEFAULT,     ["$default_output"],     [Default output provider])
    AC_DEFINE_UNQUOTED(SNOOPY_CONF_OUTPUT_DEFAULT_ARG, ["$default_output_arg"], [Default output arguments])
])
AC_MSG_NOTICE([Snoopy default output:          ${default_output:-default (system dependent)}])
AC_MSG_NOTICE([Snoopy default output argument: ${default_output_arg:-(none)}])



dnl ============================================================================
dnl ============================================================================
dnl ======= END section: OUTPUTS ===============================================
dnl ============================================================================
dnl ============================================================================





dnl ============================================================================
dnl ============================================================================
dnl ======= START section: SYSLOG ==============================================
dnl ============================================================================
dnl ============================================================================
AC_MSG_NOTICE([-])



dnl ============================================================================
AC_ARG_WITH(syslog-facility,
    [AS_HELP_STRING(
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
            syslog_facility_short=`echo "$with_syslog_facility_val" | awk '{print toupper($0)}' | sed -e 's/^LOG_//' | grep -E '^(AUTH|AUTHPRIV|CRON|DAEMON|FTP|KERN|LOCAL(0|1|2|3|4|5|6|7)|LPR|MAIL|NEWS|SYSLOG|USER|UUCP)$'`
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
AC_MSG_NOTICE([Snoopy default syslog facility: $syslog_facility])


dnl ============================================================================
AC_ARG_WITH(syslog-level,
    [AS_HELP_STRING(
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
AC_MSG_NOTICE([Snoopy default syslog level:    $syslog_level])


dnl ============================================================================
AC_ARG_WITH(syslog-ident,
    [AS_HELP_STRING(
        [--with-syslog-ident=STRING],
        [syslog identity to use when logging. [default=snoopy]]
    )],
    [
        if test "x$withval" = "x" -o "x$withval" = "xyes"; then
            # Enabled, but without explicit value
            AC_MSG_ERROR([--with-syslog-ident requires a value])
        elif test "x$withval" = "xno"; then
            # Explicitly disabled: --without-...
            AC_MSG_ERROR([using --without-syslog-ident is not supported, syslog ident is required. Either define it explicitly, or, to use the default value, remove this argument from ./configure line.])
        else
            # Enabled with explicit value definition
            syslog_ident="$withval"
        fi
    ],
    # Not specified on command line
    [syslog_ident="snoopy"]
)

AC_DEFINE_UNQUOTED(SNOOPY_CONF_SYSLOG_IDENT_FORMAT,  ["$syslog_ident"], [Syslog ident to use by default])
AC_MSG_NOTICE([Snoopy default syslog ident:    "$syslog_ident"])

dnl ============================================================================
dnl ============================================================================
dnl ======= END section: SYSLOG ================================================
dnl ============================================================================
dnl ============================================================================





dnl ============================================================================
dnl ============================================================================
dnl ======= START section: DEVELOPMENT =========================================
dnl ============================================================================
dnl ============================================================================
AC_MSG_NOTICE([-])



dnl ============================================================================
dnl ======= ENABLE TEST COVERAGE BUILD =========================================
dnl ============================================================================
AC_ARG_ENABLE(code-coverage,
    [AS_HELP_STRING(
        [--enable-code-coverage],
        [Embed code coverage instrumentation into Snoopy [default=disabled]]
    )],
    SNOOPY_CONFIGURE_ENABLE_GENERIC_SPECIFIED(  [code_coverage], [code_coverage_enabled], [$enableval], []),
    SNOOPY_CONFIGURE_ENABLE_GENERIC_UNSPECIFIED([code_coverage], [code_coverage_enabled], [no], [])
)
SNOOPY_CONFIGURE_ENABLE_GENERIC_EVALUATE([$code_coverage_enabled], [CODE_COVERAGE_ENABLED], [code coverage])

AS_IF([test "x$code_coverage_enabled" = "xyes"], [
    # Disable optimization, add code coverage-related build and linking flags
    changequote({,})
    CFLAGS=`echo $CFLAGS | sed 's/-O[1-9s]/-O0/g'`
    CXXFLAGS=`echo $CXXFLAGS | sed 's/-O[1-9s]/-O0/g'`
    CFLAGS="$CFLAGS -fprofile-arcs -ftest-coverage"
    CXXFLAGS="$CFLAGS -fprofile-arcs -ftest-coverage"
    LDFLAGS="$LDFLAGS -lgcov --coverage"
    changequote([,])
])



dnl ============================================================================
dnl ============================================================================
dnl ======= END section: DEVELOPMENT ===========================================
dnl ============================================================================
dnl ============================================================================





dnl ============================================================================
dnl ======= END CONFIGURATION SUMMARY OUTPUT ===================================
dnl ============================================================================
AC_MSG_NOTICE([======================================================================])





dnl ============================================================================
dnl ============================================================================
dnl ======= START: Check for config-dependent things ===========================
dnl ============================================================================
dnl ============================================================================


### Datasource snoopy_threads
#
# Bail out if this datasource is explicitly enabled, but thread_safety is not
#
AS_IF([test "x$enable_datasource_explicit_snoopy_threads" = "xyes" -a "x$thread_safety_enabled" = "xno"], [
    AC_MSG_ERROR([For datasource snoopy_threads to be enabled, thread safety must be enabled too. Unable to continue.])
])


### Datasource systemd_unit_name requires datasource cgroup
#
AS_IF([test "x$enable_datasource_systemd_unit_name" = "xyes" -a "x$enable_datasource_cgroup" = "xno"], [
    AC_MSG_ERROR([For datasource systemd_unit_name to be enabled, datasource cgroup must be enabled too. Unable to continue.])
])


### Datasource tid - check for pthread library
#
AS_IF([test "x$enable_datasource_tid" = "xyes"], [
    AC_CHECK_HEADER([pthread.h], [], [
        AC_MSG_ERROR([A header file is missing. Unable to continue.])
    ])
    AC_CHECK_LIB([pthread], [pthread_self], [], [
        AC_MSG_ERROR([A library is missing. Unable to continue. (Needed by "tid" datasource.)])
    ])
])


dnl ============================================================================
dnl ============================================================================
dnl ======= END: Check for config-dependent things =============================
dnl ============================================================================
dnl ============================================================================





dnl ============================================================================
dnl ======= Final output =======================================================
dnl ============================================================================
AC_OUTPUT
