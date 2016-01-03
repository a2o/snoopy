#
# SNOOPY LOGGER
#
# File: snoopy.m4
#
# Copyright (c) 2015 Bostjan Skufca <bostjan@a2o.si>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#





###
### PROGRAM CHECK Macros
###
#

# GENERAL ENTITY: ./configure output
#
# Args:
#   1: program name to check, exact case
#   2: variable to define, usually upper case (is prefixed with SNOOPY_PROG_ and suffixed with _FOUND)
#   3: additional info if program is not found
#
AU_DEFUN([SNOOPY_PROG_CHECK],
[
    AC_CHECK_PROG(SNOOPY_PROG_$2_FOUND, $1, yes)
    if test x"$SNOOPY_PROG_$2_FOUND" != x"yes" ; then
        AC_MSG_ERROR([Please install "$1" program. Reason: $3])
    fi
])





###
### EVERYTHING default value
###
#
# Defines a variable based on if --enable/--disable-everything was specified,
# and if not, the default value provided.
#

# Set default value
#
# Args:
#   1: valuevariable to check
#
AU_DEFUN([SNOOPY_CONFIGURE_ENABLE_EVERYTHING_SET],
[
    if test "x$1" == "xyes"; then
        everything_enabled="yes"
    elif test "x$1" == "xno"; then
        everything_enabled="no"
    elif test "x$1" == "xunspecified"; then
        everything_enabled="unspecified"
    else
        AC_MSG_ERROR([--enable-everything does not take an argument. Got: $enableval])
    fi
])

# Get default value
#
# Args:
#   1: variable name to define
#   2: default value, if no --enable/--disable-everything is specified.
#
AU_DEFUN([SNOOPY_CONFIGURE_ENABLE_EVERYTHING_GET],
[
    if test "x$everything_enabled" == "xyes"; then
        $1="yes"
    elif test "x$everything_enabled" == "xno"; then
        $1="no"
    else
        $1="$2"
    fi
])





###
### ENABLE GENERIC
###
#
# Defines a variable based on if --enable/--disable-everything was specified,
# and if not, the default value provided.
#

# ENABLE GENERIC: Set values if SPECIFIED (as ./configure argument)
#
# Args:
#   1: configure flag, without --enable/--disable prefix
#   2: variable name, including "_enabled" suffix
#   3: enableval value
#   4: additional help content, if invalid value is provided
#
AU_DEFUN([SNOOPY_CONFIGURE_ENABLE_GENERIC_SPECIFIED],
[
    if test "x$3" == "xyes"; then
        $2="yes"
    elif test "x$3" == "xno"; then
        $2="no"
    else
        AC_MSG_ERROR([--enable/--disable-$1 does not take an argument. $4 Got: $enableval])
    fi
])

# ENABLE GENERIC: Set values if *UN*SPECIFIED (as ./configure argument)
#
# Args:
#   1: configure flag, without --enable/--disable prefix
#   2: variable name, including "_enabled" suffix
#   3: parent default value, used if specified
#   3: default value, used if parent default value is unspecified
#
#
AU_DEFUN([SNOOPY_CONFIGURE_ENABLE_GENERIC_UNSPECIFIED],
[
    if test "x$3" == "xyes"; then
        $2="yes"
    elif test "x$3" == "xno"; then
        $2="no"
    else
        $2="$4"
    fi
])

# ENABLE GENERIC: Final macro
#
# Args:
#   1: configure flag, without --enable/--disable prefix
#   2: variable name, including "_enabled" suffix
#   3: default value
#
#
AU_DEFUN([SNOOPY_CONFIGURE_ENABLE_GENERIC_EVALUATE],
[
    AS_IF([test "x$1" = "xyes"], [
        AC_DEFINE(SNOOPY_CONF_$2, 1, [$3])
    ])
    AM_CONDITIONAL([$2], [test "x$1" == "xyes"])
    AC_MSG_NOTICE([Snoopy $3 enabled: $1])
])





###
### GENERAL ENTITY macros
###
#

# GENERAL ENTITY: ./configure output
#
# Args:
#   1: entity type (lower case)
#   2: entity type (upper case)
#   3: entity id
#   4: "yes" if enabled, "no" if disabled, anything else is displayed literally
#
AU_DEFUN([SNOOPY_CONFIGURE_ENTITY_MSG],
[
    AS_IF(
        [test "x" == "x"],
        [
            dotLine="..................................................";
            whatString="Snoopy $1 enabled: $3 ";
            whatStringPadded="$whatString${dotLine:${#whatString}}";
            if test "x$4" == "xyes"; then
                AC_MSG_NOTICE([$whatStringPadded YES])
            elif test "x$4" == "xno"; then
                AC_MSG_NOTICE([$whatStringPadded no])
            else
                AC_MSG_NOTICE([$whatStringPadded $4])
            fi
        ]
    )
])





###
### DATASOURCE Macros
###
#

# DATASOURCE: ./configure output
AU_DEFUN([SNOOPY_CONFIGURE_DATASOURCE_MSG],
[
    # Do not put newline between last angled and regular closing brace,
    # it gets picked up as part of argument.
    SNOOPY_CONFIGURE_ENTITY_MSG([datasource], [DATASOURCE], [$1], [$2])
])

# DATASOURCE: actual macro
#
# Args:
#   1: datasource name
#   2: datasource info
#   3: enable/disable by default
#   4: oposite of arg #3
#
AU_DEFUN([SNOOPY_CONFIGURE_DATASOURCE_ENABLEDISABLE],
[
    AC_ARG_ENABLE(datasource-$1,
        [AC_HELP_STRING(
            [--$4-datasource-$1],
            [$4 datasource "$1". This datasource provides $2. [default=$3]]
        )],
        [
            if   test "x$enableval" == "xyes"; then
                enable_datasource_explicit_$1="yes"
                enable_datasource_$1="yes"
            elif test "x$enableval" == "xno" ; then
                enable_datasource_explicit_$1="no"
                enable_datasource_$1="no"
            else
                AC_MSG_ERROR([--$4-datasource-$1 does not take any arguments, got: $enableval])
            fi
        ],
        [
            enable_datasource_explicit_$1="undef"

            if   test "x$enable_all_datasources" == "xyes"; then
                enable_datasource_$1="yes"
            elif test "x$enable_all_datasources" == "xno" ; then
                enable_datasource_$1="no"
            else
                if   test "x$enable_everything" == "xyes"; then
                    enable_datasource_$1="yes"
                elif test "x$enable_everything" == "xno" ; then
                    enable_datasource_$1="no"
                else
                    if test "x$3" == "xenable"; then
                        enable_datasource_$1="yes"
                    else
                        enable_datasource_$1="no"
                    fi
                fi
            fi
        ]
    )

    AS_IF([test "x$enable_datasource_$1" = "xyes"], [
        AC_DEFINE(SNOOPY_CONF_DATASOURCE_ENABLED_$1, 1, [Is datasource "$1" available?])
    ])
    AM_CONDITIONAL([DATASOURCE_ENABLED_$1], [test "x$enable_datasource_$1" == "xyes"])
    AC_SUBST([enable_datasource_$1])

    SNOOPY_CONFIGURE_DATASOURCE_MSG([$1], [$enable_datasource_$1])
])

# DATASOURCE: alias macros
AU_DEFUN([SNOOPY_CONFIGURE_DATASOURCE_ENABLE],  [SNOOPY_CONFIGURE_DATASOURCE_ENABLEDISABLE([$1], [$2], [enable],  [disable])])
AU_DEFUN([SNOOPY_CONFIGURE_DATASOURCE_DISABLE], [SNOOPY_CONFIGURE_DATASOURCE_ENABLEDISABLE([$1], [$2], [disable], [enable] )])

# DATASOURCE: force certain datasource to be enabled
AU_DEFUN([SNOOPY_CONFIGURE_DATASOURCE_FORCE],
[
    AC_DEFINE(SNOOPY_CONF_DATASOURCE_ENABLED_$1, 1, [Is datasource "$1" available? Forced "Yes".])
    AM_CONDITIONAL([DATASOURCE_ENABLED_$1], [test "x" == "x"])
    AC_SUBST([enable_datasource_$1], [yes])
    SNOOPY_CONFIGURE_DATASOURCE_MSG([$1], [YES (forced)])
])




###
### FILTER Macros
###
#

# FILTER: ./configure output
AU_DEFUN([SNOOPY_CONFIGURE_FILTER_MSG],
[
    # Do not put newline between last angled and regular closing brace,
    # it gets picked up as part of argument.
    SNOOPY_CONFIGURE_ENTITY_MSG([filter], [FILTER], [$1], [$2])
])

# FILTER: actual macro
#
# Args:
#   1: filter name
#   2: filter info
#   3: enable/disable by default
#   4: oposite of arg #3
#
AU_DEFUN([SNOOPY_CONFIGURE_FILTER_ENABLEDISABLE],
[
    AC_ARG_ENABLE(filter-$1,
        [AC_HELP_STRING(
            [--$4-filter-$1],
            [$4 filter "$1". This filter provides $2. [default=$3]]
        )],
        [

            if test   "x$enableval" == "xyes"; then
                enable_filter_$1=yes
            elif test "x$enableval" == "xno" ; then
                enable_filter_$1=no
            else
                AC_MSG_ERROR([--$4-filter-$1 does not take any arguments, got: $enableval])
            fi
        ],
        [
            if   test "x$enable_all_filters" == "xyes"; then
                enable_filter_$1="yes"
            elif test "x$enable_all_filters" == "xno" ; then
                enable_filter_$1="no"
            else
                if   test "x$enable_filtering" == "xyes"; then
                    enable_filter_$1="yes"
                elif test "x$enable_filtering" == "xno" ; then
                    enable_filter_$1="no"
                else
                    if   test "x$everything_enabled" == "xyes"; then
                        enable_filter_$1="yes"
                    elif test "x$everything_enabled" == "xno" ; then
                        enable_filter_$1="no"
                    else
                        if test "x$3" == "xenable"; then
                            enable_filter_$1="yes"
                        else
                            enable_filter_$1="no"
                        fi
                    fi
                fi
            fi
        ]
    )

    AS_IF([test "x$enable_filter_$1" = "xyes"], [
        AC_DEFINE(SNOOPY_CONF_FILTER_ENABLED_$1, 1, [Is filter "$1" available?])
    ])
    AM_CONDITIONAL([FILTER_ENABLED_$1], [test "x$enable_filter_$1" == "xyes"])
    AC_SUBST([enable_filter_$1])

    SNOOPY_CONFIGURE_FILTER_MSG([$1], [$enable_filter_$1])
])

# FILTER: alias macros
AU_DEFUN([SNOOPY_CONFIGURE_FILTER_ENABLE],  [SNOOPY_CONFIGURE_FILTER_ENABLEDISABLE([$1], [$2], [enable],  [disable])])
AU_DEFUN([SNOOPY_CONFIGURE_FILTER_DISABLE], [SNOOPY_CONFIGURE_FILTER_ENABLEDISABLE([$1], [$2], [disable], [enable] )])

# FILTER: force certain filter to be enabled
AU_DEFUN([SNOOPY_CONFIGURE_FILTER_FORCE],
[
    AC_DEFINE(SNOOPY_CONF_FILTER_ENABLED_$1, 1, [Is filter "$1" available? Forced "Yes".])
    AM_CONDITIONAL([FILTER_ENABLED_$1], [test "x" == "x"])
    AC_SUBST([enable_filter_$1], [yes])
    SNOOPY_CONFIGURE_FILTER_MSG([$1], [YES (forced)])
])




###
### OUTPUT Macros
###
#

# OUTPUT: ./configure output
AU_DEFUN([SNOOPY_CONFIGURE_OUTPUT_MSG],
[
    # Do not put newline between last angled and regular closing brace,
    # it gets picked up as part of argument.
    SNOOPY_CONFIGURE_ENTITY_MSG([output], [OUTPUT], [$1], [$2])
])

# OUTPUT: actual macro
#
# Args:
#   1: output name
#   2: output info
#   3: enable/disable by default
#   4: oposite of arg #3
#
AU_DEFUN([SNOOPY_CONFIGURE_OUTPUT_ENABLEDISABLE],
[
    AC_ARG_ENABLE(output-$1,
        [AC_HELP_STRING(
            [--$4-output-$1],
            [$4 output "$1". This output provides $2. [default=$3]]
        )],
        [
            if   test "x$enableval" == "xyes"; then
                enable_output_$1=yes
            elif test "x$enableval" == "xno" ; then
                enable_output_$1=no
            else
                AC_MSG_ERROR([--$4-output-$1 does not take any arguments, got: $enableval])
            fi
        ],
        [
            if   test "x$enable_all_outputs" == "xyes"; then
                enable_output_$1="yes"
            elif test "x$enable_all_outputs" == "xno" ; then
                enable_output_$1="no"
            else
                if   test "x$enable_everything" == "xyes"; then
                    enable_output_$1="yes"
                elif test "x$enable_everything" == "xno" ; then
                    enable_output_$1="no"
                else
                    if test "x$3" == "xenable"; then
                        enable_output_$1="yes"
                    else
                        enable_output_$1="no"
                    fi
                fi
            fi
        ]
    )

    AS_IF([test "x$enable_output_$1" = "xyes"], [
        AC_DEFINE(SNOOPY_CONF_OUTPUT_ENABLED_$1, 1, [Is output "$1" available?])
    ])
    AM_CONDITIONAL([OUTPUT_ENABLED_$1], [test "x$enable_output_$1" == "xyes"])
    AC_SUBST([enable_output_$1])

    SNOOPY_CONFIGURE_OUTPUT_MSG([$1], [$enable_output_$1])
])

# OUTPUT: alias macros
AU_DEFUN([SNOOPY_CONFIGURE_OUTPUT_ENABLE],  [SNOOPY_CONFIGURE_OUTPUT_ENABLEDISABLE([$1], [$2], [enable],  [disable])])
AU_DEFUN([SNOOPY_CONFIGURE_OUTPUT_DISABLE], [SNOOPY_CONFIGURE_OUTPUT_ENABLEDISABLE([$1], [$2], [disable], [enable])])

# OUTPUT: force certain output to be enabled
AU_DEFUN([SNOOPY_CONFIGURE_OUTPUT_FORCE],
[
    AC_DEFINE(SNOOPY_CONF_OUTPUT_ENABLED_$1, 1, [Is output "$1" available? Forced "Yes".])
    AM_CONDITIONAL([OUTPUT_ENABLED_$1], [test "x" == "x"])
    AC_SUBST([enable_output_$1], [yes])
    SNOOPY_CONFIGURE_OUTPUT_MSG([$1], [YES (forced)])
])
