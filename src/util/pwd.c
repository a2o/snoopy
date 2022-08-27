/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2022 Bostjan Skufca Jese (bostjan _A_T_ a2o.si)
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
 * Includes order: from local to global (local, this-subsystem, snoopy.h, other-subsystems, global)
 */
#include "pwd-snoopy.h"

#include "snoopy.h"

#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



/*
 * Convert UID to username (malloc()-ed buffer, must be freed by the caller)
 *
 * Params:
 *     uid:     UID to convert to username
 *
 * Return:
 *     char *:  Returns:
 *              - Pointer to malloc()-ed buffer containing the username
 *              - Pointer to malloc()-ed buffer containing string "user-UID" for non-existent user
 *              - NULL on failure
 */
char * snoopy_util_pwd_convertUidToUsername (uid_t uid)
{
    struct passwd  pwd;
    struct passwd *pwd_uid = NULL;
    char          *buffpwd_uid = NULL;
    long           buffpwdsize_uid = 0;
    char          *username = NULL;


    /* Allocate memory */
    buffpwdsize_uid = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (-1 == buffpwdsize_uid) {
        buffpwdsize_uid = 16384;
    }

    buffpwd_uid = malloc(buffpwdsize_uid);
    if (NULL == buffpwd_uid) {
        return NULL;
    }

    username = malloc(LOGIN_NAME_MAX+1);
    if (NULL == username) {
        free(buffpwd_uid);
        return NULL;
    }
    username[0] = '\0';


    /* Try uid->username conversion */
    if (0 != getpwuid_r(uid, &pwd, buffpwd_uid, buffpwdsize_uid, &pwd_uid)) {
        free(buffpwd_uid);
        free(username);
        return NULL;
    }


    // Format the return
    if (NULL == pwd_uid) {
        snprintf(username, LOGIN_NAME_MAX, "user-%d", (int)uid);
    } else {
        snprintf(username, LOGIN_NAME_MAX, "%s", pwd_uid->pw_name);
    }
    username[LOGIN_NAME_MAX] = '\0'; // Just in case


    // Return
    free(buffpwd_uid);
    return username;
}
