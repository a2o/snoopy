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
#include "src/datasource/tty.h"

#include "snoopy.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>



/*
 * SNOOPY DATA SOURCE: tty
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
int snoopy_datasource_tty (char * const result, char const * const arg)
{
    char    ttyPath[SNOOPY_DATASOURCE_TTY_sizeMaxWithNull];
    size_t  ttyPathLen = SNOOPY_DATASOURCE_TTY_sizeMaxWithoutNull;
    int     retVal;

    retVal = ttyname_r(0, ttyPath, ttyPathLen);
    if (0 != retVal) {
        if (EBADF == retVal) {
            return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(ttyname_r->EBADF)");
        }
        if (ERANGE == retVal) {
            return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(ttyname_r->ERANGE)");
        }
        if (ENOTTY == retVal) {
            return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(none)");
        }
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(ttyname_r->EUNKNOWN)");
    }

    return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", ttyPath);
}
