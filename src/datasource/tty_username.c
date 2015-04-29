/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/tty_username.c
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
 * SNOOPY DATA SOURCE: tty_username
 *
 * Description:
 *     Returns litaral username of current controlling terminal
 *
 * Params:
 *     result: pointer to string, to write result into
 *
 * Return:
 *     number of characters in the returned string
 */
#include "snoopy.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>



int snoopy_datasource_tty_username (char *result, char *arg)
{
    /* Variables for tty-to-uid conversion */
    char   *ttyPath = NULL;
    struct  stat statbuffer;
    uid_t   ttyUid;

    /* Variables for uid-to-username conversion */
    struct passwd  pwd;
    struct passwd *pwd_uid = NULL;
    char          *buffpwd_uid = NULL;
    size_t         buffpwdsize_uid = 0;
    int            messageLength   = 0;

    /* Get tty path */
    ttyPath = ttyname(0);
    if (NULL == ttyPath) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(none)");
    }

    /* Get UID of tty */
    if (-1 == stat(ttyPath, &statbuffer)) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(unable to stat() %s)", ttyPath);
    }
    ttyUid = statbuffer.st_uid;

    /* Allocate memory for pwd structure */
    buffpwdsize_uid = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (-1 == buffpwdsize_uid) {
        buffpwdsize_uid = 16384;
    }
    buffpwd_uid = malloc(buffpwdsize_uid);
    if (NULL == buffpwd_uid) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "ERROR(malloc)");
    }

    /* Try to get data */
    if (0 != getpwuid_r(ttyUid, &pwd, buffpwd_uid, buffpwdsize_uid, &pwd_uid)) {
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
