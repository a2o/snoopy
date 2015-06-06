/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/tid.c
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
#include "tid.h"

#include "snoopy.h"

#include <stdio.h>
#include <pthread.h>



/*
 * SNOOPY DATA SOURCE: tid
 *
 * Description:
 *     Returns TID (thread ID) of current process.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_tid (char * const result, char const * const arg)
{
    pthread_t tid = pthread_self();

    // This happens if -lpthread is not given to compiler
    if (0 == (unsigned long int)tid) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(error @ pthread_self())");
    }

    return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%lu", (unsigned long int)tid);
}
