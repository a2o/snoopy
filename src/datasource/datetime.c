/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/datetime.c
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
#include "datetime.h"

#include "snoopy.h"

#include <errno.h>
#include <stdio.h>
#include <time.h>



/*
 * SNOOPY DATA SOURCE: datetime
 *
 * Description:
 *     Returns current date and time in ISO 8601 forma.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_datetime (char * const result, char const * const arg)
{
    time_t     curTime;
    struct tm  curLocalTimeBuf;
    struct tm *curLocalTime;
    char       timeBuffer[80];

    // Get current time
    if ((time_t) -1 == time(&curTime)) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(error @ time(): %d)", errno);
    }

    // Convert to local time
    curLocalTime = localtime_r(&curTime, &curLocalTimeBuf);
    if (NULL == curLocalTime) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(error @ localtime_r())");
    }

    // Format it
    if (0 == strftime(timeBuffer, 80, "%FT%T%z", curLocalTime)) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(error @ strftime())");
    }

    // Copy it
    return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", timeBuffer);
}
