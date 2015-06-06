/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/group.c
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
#include "group.h"

#include "snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>



/*
 * SNOOPY DATA SOURCE: group
 *
 * Description:
 *     Returns literal group name (Group ID) of current process
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_group (char * const result, char const * const arg)
{
    struct group   gr;
    struct group  *gr_gid         = NULL;
    char          *buffgr_gid     = NULL;
    long           buffgrsize_gid = 0;
    int            messageLength  = 0;

    /* Allocate memory */
    buffgrsize_gid = sysconf(_SC_GETGR_R_SIZE_MAX);
    if (-1 == buffgrsize_gid) {
        buffgrsize_gid = 16384;
    }
    buffgr_gid = malloc(buffgrsize_gid);
    if(NULL == buffgr_gid) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(malloc)");
    }

    /* Try to get data */
    if (0 != getgrgid_r(getgid(), &gr, buffgr_gid, buffgrsize_gid, &gr_gid)) {
        messageLength  = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(getgrgid_r)");
    } else {
        if (NULL == gr_gid) {
            messageLength = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(undefined)");
        } else {
            messageLength = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", gr_gid->gr_name);
        }
    }

    /* Cleanup and return */
    free(buffgr_gid);
    return messageLength;
}
