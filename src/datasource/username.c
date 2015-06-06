/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/username.c
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
#include "username.h"

#include "snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>



/*
 * SNOOPY DATA SOURCE: username
 *
 * Description:
 *     Returns literal username of current process
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_username (char * const result, char const * const arg)
{
    struct passwd  pwd;
    struct passwd *pwd_uid = NULL;
    char          *buffpwd_uid = NULL;
    long           buffpwdsize_uid = 0;
    int            messageLength   = 0;

    /* Allocate memory */
    buffpwdsize_uid = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (-1 == buffpwdsize_uid) {
        buffpwdsize_uid = 16384;
    }
    buffpwd_uid = malloc(buffpwdsize_uid);
    if (NULL == buffpwd_uid) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(malloc)");
    }

    /* Try to get data */
    if (0 != getpwuid_r(getuid(), &pwd, buffpwd_uid, buffpwdsize_uid, &pwd_uid)) {
        messageLength  = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(getpwuid_r)");
    } else {
        if (NULL == pwd_uid) {
            messageLength = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(undefined)");
        } else {
            messageLength = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", pwd_uid->pw_name);
        }
    }

    /* Cleanup and return */
    free(buffpwd_uid);
    return messageLength;
}
