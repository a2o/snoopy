/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2015 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "action-unit-util-utmp.h"
#include "action-common.h"

#include "snoopy.h"
#include "util/utmp-snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void snoopyTestCli_action_unit_util_utmp_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `util` :: Subunit 'utmp'\n"
        "\n"
        "Description:\n"
        "    Helps with mocking src/unit/utmp.c implementation code paths (mainly for the coverage of code parts/paths not covered by the test suite).\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit util utmp TTY_PATH [UTMP_FILE_PATH]\n"
        "    snoopy-test unit util utmp --help\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_util_utmp (int argc, char ** argv)
{
    const char * arg1 = NULL;
    const char * ttyPath = NULL;
    const char * utmpPath = NULL;

    struct utmp   utmpEntryBuf;
    struct utmp * utmpEntry = &utmpEntryBuf;


    // Parse arg #1
    if (argc == 0) {
        fatalError("First argument is required (a TTY_PATH, or --help)");
    }
    arg1 = argv[0];

    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_unit_util_utmp_showHelp();
        return 0;
    }
    ttyPath = arg1;

    // Parse arg #2
    if (argc >= 2) {
        utmpPath = argv[1];
        snoopy_util_utmp_test_setAlternateUtmpFilePath(utmpPath);
        printWarningValue("Using alternative utmp file path", utmpPath);
    }

    // Find the entry
    if (SNOOPY_TRUE != snoopy_util_utmp_findUtmpEntryByPath(ttyPath, utmpEntry)) {
        printf("(Entry not found.)\n");
        return 0;
    }

    // Check if it contains an IP address
    if (SNOOPY_TRUE != snoopy_util_utmp_doesEntryContainIpAddr(utmpEntry)) {
        printf("-\n");
        return 0;
    }

    // Convert IP address to string
    char   ipAddrStrBuf[256] = {'\0'};
    char * ipAddrStr         = ipAddrStrBuf;
    snoopy_util_utmp_getUtmpIpAddrAsString(utmpEntry, ipAddrStr, 256);

    // Print it
    printf("%s\n", ipAddrStr);

    return 0;
}
