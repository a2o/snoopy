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



### Macros for adding data source enable/disable options to ./configure
#
AU_DEFUN([SNOOPY_CONFIGURE_DATASOURCE_ENABLE],
[
    AC_ARG_ENABLE(datasource-$1,
        [AC_HELP_STRING(
            [--disable-datasource-$1],
            [disable data source "$1". This datasource provides $2. [default=enabled]]
        )],
        [
            if test "x$enableval" = "xyes"; then
                enable_datasource_$1=yes
            elif test "x$enableval" = "xno"; then
                enable_datasource_$1=no
            else
                AC_MSG_ERROR([--disable-datasource-$1 does not take any arguments, got: $enableval])
            fi
        ],
        [enable_datasource_$1=yes]
    )

    AS_IF([test "x$enable_datasource_$1" = "xyes"], [
        AC_DEFINE(SNOOPY_CONF_DATASOURCE_ENABLED_$1, 1, [Is data source "$1" available?])
    ])
    AM_CONDITIONAL([DATASOURCE_ENABLED_$1], [test "x$enable_datasource_$1" == "xyes"])
])


AU_DEFUN([SNOOPY_CONFIGURE_DATASOURCE_DISABLE],
[
    AC_ARG_ENABLE(datasource-$1,
        [AC_HELP_STRING(
            [--enable-datasource-$1],
            [enable data source "$1". This datasource provides $2. [default=disabled]]
        )],
        [
            if test "x$enableval" = "xyes"; then
                enable_datasource_$1=yes
            elif test "x$enableval" = "xno"; then
                enable_datasource_$1=no
            else
                AC_MSG_ERROR([--enable-datasource-$1 does not take any arguments, got: $enableval])
            fi
        ],
        [enable_datasource_$1=no]
    )

    AS_IF([test "x$enable_datasource_$1" = "xyes"], [
        AC_DEFINE(SNOOPY_CONF_DATASOURCE_ENABLED_$1, 1, [Is data source "$1" available?])
    ])
    AM_CONDITIONAL([DATASOURCE_ENABLED_$1], [test "x$enable_datasource_$1" == "xyes"])
])
