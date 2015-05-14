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
### GENERAL ENTITY macros
###
#

# GENERAL ENTITY: ./configure output
#
# Args:
#   1: entity type (lower case)
#   2: entity type (upper case)
#   3: entity id
#   4: "yes" if enabled, anything else if not
#
AU_DEFUN([SNOOPY_CONFIGURE_ENTITY_MSG],
[
    AS_IF(
        [test "x$4" = "xyes"],
        [AC_MSG_NOTICE([Snoopy $1 enabled: $3])],
        [AC_MSG_NOTICE([Snoopy $1 disabled: $3])]
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
            if test "x$enableval" = "xyes"; then
                enable_datasource_$1=yes
            elif test "x$enableval" = "xno"; then
                enable_datasource_$1=no
            else
                AC_MSG_ERROR([--$4-datasource-$1 does not take any arguments, got: $enableval])
            fi
        ],
        [
            if test "x$3" = "xenable"; then
                enable_datasource_$1=$enable_all_datasources
            else
                enable_datasource_$1=no
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
            if test "x$enableval" = "xyes"; then
                enable_filter_$1=yes
            elif test "x$enableval" = "xno"; then
                enable_filter_$1=no
            else
                AC_MSG_ERROR([--$4-filter-$1 does not take any arguments, got: $enableval])
            fi
        ],
        [
            if test "x$3" = "xenable"; then
                enable_filter_$1=$enable_all_filters
            else
                enable_filter_$1=no
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
            if test "x$enableval" = "xyes"; then
                enable_output_$1=yes
            elif test "x$enableval" = "xno"; then
                enable_output_$1=no
            else
                AC_MSG_ERROR([--$4-output-$1 does not take any arguments, got: $enableval])
            fi
        ],
        [
            if test "x$3" = "xenable"; then
                enable_output_$1=$enable_all_outputs
            else
                enable_output_$1=no
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
AU_DEFUN([SNOOPY_CONFIGURE_OUTPUT_DISABLE], [SNOOPY_CONFIGURE_OUTPUT_ENABLEDISABLE([$1], [$2], [disable], [enable] )])

# OUTPUT: alias to force certain filter to be enabled
AU_DEFUN([SNOOPY_CONFIGURE_OUTPUT_ENABLE_FORCE],
[
    AC_DEFINE(SNOOPY_CONF_OUTPUT_ENABLED_$1, 1, [Is output "$1" available? Forced "Yes".])
    AM_CONDITIONAL([OUTPUT_ENABLED_$1], [test "x$enable_output_$1" == "xyes"])
    AC_SUBST([enable_output_$1], [yes])
    AC_MSG_NOTICE([Snoopy output enabled: $1 - forced])
])
