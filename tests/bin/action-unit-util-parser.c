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
#include "action-unit-util-parser.h"
#include "action-common.h"

#include "snoopy.h"
#include "util/parser-snoopy.h"

#include <stdio.h>
#include <string.h>
#include <syslog.h>



void snoopyTestCli_action_unit_util_parser_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `misc` :: Subunit `parser`\n"
        "\n"
        "Description:\n"
        "    Mocks src/unit/parser.c implementation code paths (mainly for the coverage of code parts/paths not covered by the test suite).\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit util parser\n"
        "    snoopy-test unit util parser --help\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_util_parser (int argc, char ** argv)
{
    const char *arg1;

    if (argc > 0) {
        arg1 = argv[0];
    } else {
        arg1 = "";
    }

    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_unit_util_parser_showHelp();
        return 0;
    }


    // Do the mocking
    if (snoopy_util_parser_strByteLength(    "1", 100,   30000, 2000) !=     100)     fatalErrorValue("Error parsing byte length string",     "1");
    if (snoopy_util_parser_strByteLength(  "100", 100,   30000, 2000) !=     100)     fatalErrorValue("Error parsing byte length string",   "100");
    if (snoopy_util_parser_strByteLength(  "101", 100,   30000, 2000) !=     101)     fatalErrorValue("Error parsing byte length string",   "101");
    if (snoopy_util_parser_strByteLength( "1000", 100,   30000, 2000) !=    1000)     fatalErrorValue("Error parsing byte length string",  "1000");
    if (snoopy_util_parser_strByteLength( "2000", 100,   30000, 2000) !=    2000)     fatalErrorValue("Error parsing byte length string",  "2000");
    if (snoopy_util_parser_strByteLength("29999", 100,   30000, 2000) !=   29999)     fatalErrorValue("Error parsing byte length string", "29999");
    if (snoopy_util_parser_strByteLength("30000", 100,   30000, 2000) !=   30000)     fatalErrorValue("Error parsing byte length string", "30000");
    if (snoopy_util_parser_strByteLength("30001", 100,   30000, 2000) !=   30000)     fatalErrorValue("Error parsing byte length string", "30001");

    if (snoopy_util_parser_strByteLength(   "1k", 100, 3000000, 2000) !=    1024)     fatalErrorValue("Error parsing byte length string",    "1k");
    if (snoopy_util_parser_strByteLength(   "3k", 100, 3000000, 2000) !=    3072)     fatalErrorValue("Error parsing byte length string",    "3k");
    if (snoopy_util_parser_strByteLength(  "32k", 100, 3000000, 2000) !=   32768)     fatalErrorValue("Error parsing byte length string",   "32k");
    if (snoopy_util_parser_strByteLength(   "1m", 100, 3000000, 2000) != 1048576)     fatalErrorValue("Error parsing byte length string",    "1m");

    if (snoopy_util_parser_strByteLength("" ,     100,   30000, 2000) !=    2000)     fatalErrorValue("Error parsing byte length string", "(nul)");
    if (snoopy_util_parser_strByteLength("0",     100,   30000, 2000) !=    2000)     fatalErrorValue("Error parsing byte length string",     "0");
    if (snoopy_util_parser_strByteLength("asdf",  100,   30000, 2000) !=    2000)     fatalErrorValue("Error parsing byte length string",  "asdf");

    printSuccess("Mocking src/util/parser.c (strByteLength-only) complete.");
    return 0;
}
