/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/datasource/cwd.c
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
#include "cwd.h"

#include "snoopy.h"

#include <stdio.h>
#include <unistd.h>
#include <limits.h>



/*
 * SNOOPY DATA SOURCE: cwd
 *
 * Description:
 *     Returns current working directory of current process.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_cwd (char * const resultBuf, size_t resultBufSize, __attribute__((unused)) char const * const arg)
{
    char cwdBuf[PATH_MAX+1];

    if (getcwd(cwdBuf, PATH_MAX+1)) {
        return snprintf(resultBuf, resultBufSize, "%s", cwdBuf);
    }
    return SNOOPY_DATASOURCE_FAILURE;
}
