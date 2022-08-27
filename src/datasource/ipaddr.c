/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/datasource/tty.c
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
#include "ipaddr.h"

#include "snoopy.h"

#include "util/utmp-snoopy.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <utmp.h>



#define         SNOOPY_DS_IPADDR_TTY_PATH_BUF_LEN       UT_LINESIZE + 5 // +5 for the initial "/dev/"



/*
 * SNOOPY DATA SOURCE: ipaddr
 *
 * Description:
 *     Returns IP address of the TTY that is connected to the current process.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_ipaddr (char * const result, __attribute__((unused)) char const * const arg)
{
    char          ttyPathBuf[SNOOPY_DS_IPADDR_TTY_PATH_BUF_LEN];
    struct utmp   utmpEntryBuf;
    struct utmp * utmpEntry = &utmpEntryBuf;
    int           retVal;

    // Get the TTY path
    ttyPathBuf[0] = '\0';
    retVal = ttyname_r(STDIN_FILENO, ttyPathBuf, SNOOPY_DS_IPADDR_TTY_PATH_BUF_LEN);
    if (0 != retVal) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "-");
    }
    ttyPathBuf[SNOOPY_DS_IPADDR_TTY_PATH_BUF_LEN-1] = '\0';

    // Find the matching utmp entry
    if (SNOOPY_TRUE != snoopy_util_utmp_findUtmpEntryByPath(ttyPathBuf, utmpEntry)) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "-"); // This can happen, i.e. in in a `docker run ...` environment
    }

    // Does the associated IP address actually exist?
    if (SNOOPY_TRUE != snoopy_util_utmp_doesEntryContainIpAddr(utmpEntry)) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "-");
    }

    // Convert to string IP address
    snoopy_util_utmp_getUtmpIpAddrAsString(utmpEntry, result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE);

    // Clean up
    return (int) strlen(result);
}
