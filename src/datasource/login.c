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
 * Includes order: from local to global
 */
#include "login.h"

#include "snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * Local defines
 */
#define   SNOOPY_DATASOURCE_LOGIN_loginSizeMaxWithoutNull   254
#define   SNOOPY_DATASOURCE_LOGIN_loginSizeMaxWithNull      255



/*
 * SNOOPY DATA SOURCE: login
 *
 * Description:
 *     Returns literal login name of logged-in user executing this process
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_login (char * const result, char const * const arg)
{
    int          loginSizeMaxWithoutNull = SNOOPY_DATASOURCE_LOGIN_loginSizeMaxWithoutNull;
    int          loginSizeMaxWithNull    = SNOOPY_DATASOURCE_LOGIN_loginSizeMaxWithNull;
    static char  login[SNOOPY_DATASOURCE_LOGIN_loginSizeMaxWithNull];
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
            strncpy(login, loginptr, loginSizeMaxWithoutNull);   // Coverity suggests using -1 size here
            if ((int)strlen(loginptr) > loginSizeMaxWithoutNull) {
                login[loginSizeMaxWithoutNull] = '\0';
            }
        }
    }
    return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", login);
}
