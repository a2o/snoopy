/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/login.c
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
 * SNOOPY DATA SOURCE: login
 *
 * Description:
 *     Returns literal login name of logged-in user executing this process
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
#include <string.h>



int snoopy_datasource_login (char *result, char *arg)
{
    int          loginSizeMaxWithoutNull = 254;
    int          loginSizeMaxWithNull    = 255;
    static char  login[loginSizeMaxWithNull];
    char        *loginptr = NULL;

    /*
     * Retrive the user login trying in order:
     * - The login information from the processus.
     * - the SUDO_USER environment variable.
     * - the LOGNAME environment variable.
     *
     * TIP to use with sudo and keep LOGNAME, add this in /etc/sudoers:
     * Defaults        env_reset
     * Defaults        env_keep="LOGNAME"
    */
    if (0 != getlogin_r(login, loginSizeMaxWithNull)) {
        loginptr = getenv("SUDO_USER");
        if (!loginptr) {
            loginptr = getenv("LOGNAME");
        }
        if (!loginptr) {
            strcpy(login, "(unknown)");
        } else {
            strncpy(login, loginptr, loginSizeMaxWithNull);
            if (strlen(loginptr) > loginSizeMaxWithoutNull) {
                login[loginSizeMaxWithoutNull] = '\0';
            }
        }
    }
    return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", login);
}
