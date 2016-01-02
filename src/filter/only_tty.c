/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/tty.c
 *
 * Copyright (c) 2014-2015 Bostjan Skufca <bostjan@a2o.si>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */



/*
 * Includes order: from local to global
 */
#include "src/filter/only_tty.h"

#include "snoopy.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>



/*
 * SNOOPY FILTER: only_tty
 *
 * Description:
 *     Returns TTY of current process.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_filter_only_tty(char *msg, char const * const arg)
{
    char    ttyPath[SNOOPY_DATASOURCE_TTY_sizeMaxWithNull];
    size_t  ttyPathLen = SNOOPY_DATASOURCE_TTY_sizeMaxWithoutNull;
    int retVal = ttyname_r(0, ttyPath, ttyPathLen);

    // Only pass in this case
    if (0 == retVal) {
        return SNOOPY_FILTER_PASS;
    }
    return SNOOPY_FILTER_DROP;
}
