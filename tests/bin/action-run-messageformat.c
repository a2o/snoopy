/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy-test-message-format.c
 *
 * Copyright (c) 2015 Bostjan Skufca <bostjan@a2o.si>
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
#include "action-common.h"

#include "snoopy.h"
#include "entrypoint/test-cli.h"

#include "configuration.h"
#include "inputdatastorage.h"
#include "message.h"

#include <stdio.h>
#include <stdlib.h>



void snoopyTestCli_action_run_messageformat_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `run` :: Subsystem `message formatter`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test run messageformat \"FORMAT SPECIFICATION\"\n"
        "\n"
        "Result:\n"
        "    Prints a log message formatted according to the given format specification.\n"
        "    Process data is taken from self.\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_run_messageformat (int argc, char **argv)
{
    const char * messageFormat;
    char       * message;


    /* Initialize Snoopy */
    snoopy_entrypoint_test_cli_init((char const *)g_argv[0], g_argv, NULL);


    /* Check if all arguments are present */
    if (argc < 1) {
        snoopyTestCli_action_run_messageformat_showHelp();
        fatalError("Missing argument: message format");
    }
    messageFormat = argv[0];


    /* Initialize message */
    message    = malloc(SNOOPY_LOG_MESSAGE_BUF_SIZE);
    message[0] = '\0';


    /* Call the filter */
    snoopy_message_generateFromFormat(message, SNOOPY_LOG_MESSAGE_BUF_SIZE, messageFormat);


    /* Display result */
    printf("%s\n", message);


    /* Housekeeping and return */
    free(message);
    snoopy_entrypoint_test_cli_exit();
    return 0;
}
