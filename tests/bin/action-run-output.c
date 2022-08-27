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
#include "action-run-output.h"
#include "action-common.h"

#include "snoopy.h"
#include "entrypoint/test-cli.h"

#include "configuration.h"
#include "inputdatastorage.h"
#include "outputregistry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



void snoopyTestCli_action_run_output_showList ()
{
    printf("Available outputs:\n");
    int oCount = snoopy_outputregistry_getCount();
    for (int i=0 ; i<oCount ; i++) {
        printf("    %s\n", snoopy_outputregistry_getName(i));
    }
}



void snoopyTestCli_action_run_output_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `run` :: Subsystem `output`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test run output \"LOG MESSAGE\" OUTPUT [OUTPUT_ARGS]\n"
        "    snoopy-test run output --all\n"
        "    snoopy-test run output --list\n"
        "    snoopy-test run output --help\n"
        "\n";
    printf("%s", helpContent);

    snoopyTestCli_action_run_output_showList();
}



int snoopyTestCli_action_run_output (int argc, char ** argv)
{
    const char * arg1;
    const char * message;
    const char * outputName;
    const char * outputArg;
    int retVal;


    /* Initialize Snoopy */
    snoopy_entrypoint_test_cli_init((char const *)g_argv[0], g_argv, NULL);


    /* Check if all arguments are present */
    if (argc < 1) {
        snoopyTestCli_action_run_output_showHelp();
        fatalError("Missing argument: log message, or --all or --list");
    }
    arg1 = argv[0];

    // Meta actions
    if (0 == strcmp(arg1, "--all")) {
        snoopyTestCli_action_run_output_all();
        return 0;
    }
    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_run_output_showHelp();
        return 0;
    }
    if (0 == strcmp(arg1, "--list")) {
        snoopyTestCli_action_run_output_showList();
        return 0;
    }
    message = arg1;

    if (argc < 2) {
        snoopyTestCli_action_run_output_showHelp();
        fatalError("Missing argument: output name");
    }
    outputName = argv[1];

    /* Is there an argument for this data source */
    if (argc > 2) {
        outputArg = argv[2];
    } else {
        outputArg = "";
    }


    /* Check if what we got is a valid output name */
    if (SNOOPY_FALSE == snoopy_outputregistry_doesNameExist(outputName)) {
        snoopyTestCli_action_run_output_showHelp();
        fatalErrorValue("Invalid output name given", outputName);
    }

    /* Dispatch message to output */
    retVal = snoopy_outputregistry_callByName(outputName, message, outputArg);
    if (SNOOPY_OUTPUT_FAILED(retVal)) {
        fatalError("Output failure");
    }


    /* Housekeeping and return */
    snoopy_entrypoint_test_cli_exit();
    return 0;
}



void snoopyTestCli_action_run_output_all ()
{
    char *message    = NULL;
    char *itemName   = NULL;
    const char *itemArgs   = NULL;
    int   itemResult;
    int   iCount;


    /* Initialize variables and spaces */
    message  = malloc(SNOOPY_LOG_MESSAGE_BUF_SIZE);
    snprintf(message, SNOOPY_LOG_MESSAGE_BUF_SIZE, "Snoopy output debugging");

    /* Loop throught all outputs and run them with bogus arguments */
    iCount = snoopy_outputregistry_getCount();
    for (int i=0 ; i<iCount ; i++) {

        itemName = snoopy_outputregistry_getName(i);
        printf("Output %19s: ", itemName);

        /* Which arguments to pass */
        if (strcmp(itemName, "file") == 0) {
            itemArgs = "./fileoutput.out";
        } else if (strcmp(itemName, "socket") == 0) {
            itemArgs = "/dev/log";
        } else {
            itemArgs = "";
        }

        /* Execute the output function */
        itemResult = snoopy_outputregistry_callById(i, message, itemArgs);

        /* Evaluate */
        printf("%d chars transmitted. (output arg:%s)\n", itemResult, itemArgs);

        /* Cleanup */
        if (strcmp(itemName, "file") == 0) {
            unlink(itemArgs);
        }
    }

    /* Memory housekeeping */
    free(message);
}
