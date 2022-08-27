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
#include "action-unit-outputregistry.h"
#include "action-common.h"

#include "snoopy.h"
#include "outputregistry.h"
#include "entrypoint/test-cli.h"
#include "configuration.h"

#include <stdio.h>
#include <string.h>



void snoopyTestCli_action_unit_outputregistry_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `outputregistry`\n"
        "\n"
        "Description:\n"
        "    Mocks outputregistry implementation code paths (mainly for the coverage of code parts/paths not covered by the test suite).\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit outputregistry\n"
        "    snoopy-test unit outputregistry --help\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_outputregistry (int argc, char ** argv)
{
    const char *arg1;

    if (argc > 0) {
        arg1 = argv[0];
    } else {
        arg1 = "";
    }

    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_unit_outputregistry_showHelp();
        return 0;
    }


    // Do the mocking
    const char * outputName = NULL;
    int outputCount = 0;
    int outputIdPreset = 0;
    int outputIdReceived = 0;

    outputCount = snoopy_outputregistry_getCount();
    if (outputCount < 1) {
        fatalError("No outputs available, output count is 0");
    }
    printf("[DEBUG] Number of outputs available: %d\n", outputCount);

    if (snoopy_outputregistry_doesIdExist(outputIdPreset) != SNOOPY_TRUE) {
        fatalError("Output with a preset ID (0) does not exist");
    }

    outputName = snoopy_outputregistry_getName(outputIdPreset);
    outputIdReceived = snoopy_outputregistry_getIdFromName(outputName);
    if (outputIdPreset != outputIdReceived) {
        fatalError("Output ID returned when searching by name differs from the initially used ID to find that same output");
    }

    outputName = "noop";
    outputIdReceived = snoopy_outputregistry_getIdFromName(outputName);
    snoopy_outputregistry_callById(outputIdReceived, NULL, "");

    if (-1 != snoopy_outputregistry_callById(-1, NULL, "")) {
        fatalError("Output ID -1 unexpectedly exists");
    }
    if (-1 != snoopy_outputregistry_callByName("fakeOutputNameThatShouldNeverExist", NULL, "")) {
        fatalError("Output with an unexpected name actually exists");
    }


    /* Initialize Snoopy */
    snoopy_entrypoint_test_cli_init((char const *)g_argv[0], g_argv, NULL);


    snoopy_configuration_t * CFG;
    CFG = snoopy_configuration_get();
    CFG->output = "noop";

    snoopy_outputregistry_dispatch(NULL);


    /* Cleanup */
    snoopy_entrypoint_test_cli_exit();


    printSuccess("Mocking src/outputregistry.c complete.");
    return 0;
}
