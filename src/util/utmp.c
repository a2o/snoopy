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
#include "utmp-snoopy.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE // for getutline_r()
#endif
#include "snoopy.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <utmp.h>



/*
 * Try to find utmp entry matching TTY line
 *
 * Params:
 *     ttyLine:         TTY ("line") to search for, without the "/dev/" prefix
 *     resultEntryBuf:  Buffer to return result in
 *
 * Return:
 *     int:        SNOOPY_TRUE or
 *                 SNOOPY_FALSE (when not found, or error occurs)
 */
int snoopy_util_utmp_findUtmpEntryByLine (char const * const ttyLine, struct utmp * const resultEntryBuf)
{
    struct utmp   searchEntry;
    struct utmp * resultEntry;
    int           retVal;

    // Prepare the search conditions
    strncpy(searchEntry.ut_line, ttyLine, UT_LINESIZE);
    searchEntry.ut_line[UT_LINESIZE-1] = '\0';

    // Do the search
    setutent();
    retVal = getutline_r(&searchEntry, resultEntryBuf, &resultEntry);
    endutent();

    // Failure/not found
    if (retVal != 0) {
        return SNOOPY_FALSE;
    }

    // Found
    return SNOOPY_TRUE;
}



/*
 * Try to find utmp entry matching TTY path
 *
 * Params:
 *     ttyLine:         TTY defice path to search for, _including_ the "/dev/" prefix
 *     resultEntryBuf:  Buffer to return result in
 *
 * Return:
 *     int:        SNOOPY_TRUE or
 *                 SNOOPY_FALSE (when not found, or error occurs)
 */
int snoopy_util_utmp_findUtmpEntryByPath (char const * const ttyPath, struct utmp * const resultEntryBuf)
{
    const char * ttyLine;

    // "Remove" the initial `/dev/` part
    if (0 != strncmp(ttyPath, "/dev/", strlen("/dev/"))) {
        return SNOOPY_FALSE;
    }
    ttyLine = ttyPath + strlen("/dev/");

    return snoopy_util_utmp_findUtmpEntryByLine(ttyLine, resultEntryBuf);
}



/*
 * Check if utmp entry contains an IP address
 *
 * Params:
 *     utmpEntry:       UTMP entry to work with
 *
 * Return:
 *     int:        SNOOPY_TRUE or
 *                 SNOOPY_FALSE
 */
int snoopy_util_utmp_doesEntryContainIpAddr (struct utmp const * const utmpEntry)
{
    if ((utmpEntry->ut_addr_v6[0] == 0) &&
        (utmpEntry->ut_addr_v6[1] == 0) &&
        (utmpEntry->ut_addr_v6[2] == 0) &&
        (utmpEntry->ut_addr_v6[3] == 0)
    ) {
        return SNOOPY_FALSE;
    } else {
        return SNOOPY_TRUE;
    }
}



/*
 * Extract utmp entry's IP address as string
 *
 * Params:
 *     utmpEntry:       UTMP entry to work with
 *     resultBuf:       Buffer to return result in
 *     resultBufSize:   Size of resultBuf
 *
 * Return:
 *     int:        SNOOPY_TRUE or
 *                 SNOOPY_FALSE
 */
int snoopy_util_utmp_getUtmpIpAddrAsString (struct utmp const * const utmpEntry, char * const resultBuf, size_t resultBufSize)
{
    if ((utmpEntry->ut_addr_v6[1] == 0) &&
        (utmpEntry->ut_addr_v6[2] == 0) &&
        (utmpEntry->ut_addr_v6[3] == 0)
    ) {
        inet_ntop(AF_INET, &(utmpEntry->ut_addr_v6[0]), resultBuf, (socklen_t) resultBufSize);
    } else {
        inet_ntop(AF_INET6, utmpEntry->ut_addr_v6, resultBuf, (socklen_t) resultBufSize);
    }

    return SNOOPY_TRUE;
}



/*
 * Set path to alternate utmp file, for other functions to use
 *
 * Params:
 *     utmpPath:        Path to UTMP file to use
 *
 * Return:
 *     void
 */
void snoopy_util_utmp_test_setAlternateUtmpFilePath (char const * const utmpPath)
{
    utmpname(utmpPath);
}
