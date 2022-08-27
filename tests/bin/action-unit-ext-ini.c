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
#include "action-unit-ext-ini.h"
#include "action-common.h"

#include "snoopy.h"
#include "entrypoint/test-cli.h"

#include "lib/inih/src/ini.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void snoopyTestCli_action_unit_ext_ini_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `ext-ini`\n"
        "\n"
        "Description:\n"
        "    Mocks external ini implementation code paths (mainly for the code coverage of parts not used by Snoopy).\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit ext-ini\n"
        "    snoopy-test unit ext-ini --help\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_ext_ini_parserCallback (
    void * userPtr,
    const char* section,
    const char* name,
    const char* value
) {
    const char ** errorMessage = (const char **) userPtr;

    // Check if we're already in an error state
    if (*errorMessage != NULL) {
        return 1;
    }

    if (0 != strcmp(section, "sectionName")) {
        *errorMessage = "Unexpected section";
        return 1;
    }
    if (0 != strcmp(name, "testKey")) {
        *errorMessage = "Unexpected key";
        return 1;
    }

    if (0 != strcmp(value, "testVal")) {
        *errorMessage = "Unexpected value";
        return 1;
    }

    return 1;
}



int snoopyTestCli_action_unit_ext_ini (int argc, char ** argv)
{
    const char *arg1;


    /* Initialize Snoopy */
    snoopy_entrypoint_test_cli_init((char const *)g_argv[0], g_argv, NULL);


    if (argc > 0) {
        arg1 = argv[0];
    } else {
        arg1 = "";
    }


    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_unit_ext_ini_showHelp();
        return 0;
    }


    // Do the mocking
    const char * iniContent = "[sectionName]\n"
        "testKey=testVal\n";
    const char * errorMessage = NULL;
    int parserStatus = 0;

    parserStatus = ini_parse_string(iniContent, snoopyTestCli_action_unit_ext_ini_parserCallback, (void *)&errorMessage);
    if (parserStatus < 0) {
        fatalError("INI parsing failed");
    }
    if (errorMessage != NULL) {
        fatalErrorValue("INI parsing failure", errorMessage);
    }
    printSuccess("Mocking lib/inih/src/ini.c complete.");


    /* Housekeeping and return */
    snoopy_entrypoint_test_cli_exit();
    return 0;
}
