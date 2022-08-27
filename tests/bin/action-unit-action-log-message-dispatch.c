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
#include "action-unit-action-log-message-dispatch.h"
#include "action-common.h"

#include "snoopy.h"
#include "action/log-message-dispatch.h"
#include "entrypoint/test-cli.h"
#include "configuration.h"

#include <stdio.h>
#include <string.h>



void snoopyTestCli_action_unit_action_log_message_dispatch_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `action` :: Subunit 'log-message-dispatch'\n"
        "\n"
        "Description:\n"
        "    Mocks src/action/log-message-dispatch.c implementation code paths (mainly for the coverage of code parts/paths not covered by the test suite).\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit action log-message-dispatch\n"
        "    snoopy-test unit action log-message-dispatch --help\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_action_log_message_dispatch (int argc, char ** argv)
{
    const char *arg1;

    if (argc > 0) {
        arg1 = argv[0];
    } else {
        arg1 = "";
    }

    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_unit_action_log_message_dispatch_showHelp();
        return 0;
    }



    /* Initialize Snoopy */
    snoopy_entrypoint_test_cli_init((char const *)g_argv[0], g_argv, NULL);


    // Do the mocking
    snoopy_configuration_t * CFG;
    CFG = snoopy_configuration_get();
    CFG->output = "stdout";
    snoopy_action_log_message_dispatch("Test error message, sent out via stdout output.");


    /* Cleanup */
    snoopy_entrypoint_test_cli_exit();


    printSuccess("Mocking src/action/log-message-dispatch.c complete.");
    return 0;
}
