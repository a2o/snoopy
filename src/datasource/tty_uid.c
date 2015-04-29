/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/tty_uid.c
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
 * SNOOPY DATA SOURCE: tty_uid
 *
 * Description:
 *     Returns UID (User ID) of current controlling terminal, or -1 if not found.
 *
 * Params:
 *     result: pointer to string, to write result into
 *
 * Return:
 *     number of characters in the returned string
 */
#include "snoopy.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



int snoopy_datasource_tty_uid (char *result, char *arg)
{
    char   *ttyPath = NULL;
    struct  stat statbuffer;
    long    ttyUid;

    /* Get tty path */
    ttyPath = ttyname(0);
    if (NULL == ttyPath) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(none)");
    }

    /* Get owner of tty */
    if (-1 == stat(ttyPath, &statbuffer)) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(unable to stat() %s)", ttyPath);
    }
    ttyUid = statbuffer.st_uid;

    return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%ld", ttyUid);
}
