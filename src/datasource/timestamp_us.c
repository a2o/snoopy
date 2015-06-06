/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/timestamp_us.c
 *
 * Copyright (c) 2015 Bostjan Skufca <bostjan@a2o.si>
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
#include "timestamp_us.h"

#include "snoopy.h"

#include <errno.h>
#include <stdio.h>
#include <sys/time.h>



/*
 * SNOOPY DATA SOURCE: timestamp_us
 *
 * Description:
 *     Returns milliseconds part of current Unix timestamp.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_timestamp_us (char * const result, char const * const arg)
{
    struct timeval tv;
    int            retVal;

    retVal = gettimeofday(&tv, NULL);
    if (0 == retVal) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%06d", (int) tv.tv_usec);
    } else {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(error: %d)", errno);
    }
}
