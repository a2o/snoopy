/*
 * SNOOPY LOGGER
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
#include "action-common.h"

#include "snoopy.h"

#include "configuration.h"
#include "inputdatastorage.h"
#include "misc.h"
#include "outputregistry.h"

#include <stdio.h>
#include <string.h>



void snoopyTestCli_action_run_output_listOutputs ()
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
        "    snoopy-test run output --list\n"
        "\n";
    printf("%s", helpContent);

    snoopyTestCli_action_run_output_listOutputs();
}



int snoopyTestCli_action_run_output (int argc, char ** argv)
{
    const char * message;
    const char * outputName;
    const char * outputArg;
    int retVal;


    /* Initialize Snoopy */
    snoopy_configuration_preinit_disableConfigFileParsing();
    snoopy_init();
    snoopy_inputdatastorage_store_filename(g_argv[0]);
    snoopy_inputdatastorage_store_argv(g_argv);


    /* Check if all arguments are present */
    if (argc < 1) {
        snoopyTestCli_action_run_output_showHelp();
        fatalError("Missing argument: log message");
    }
    message = argv[0];

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
    retVal = snoopy_outputregistry_callByName(outputName, message, SNOOPY_LOG_MESSAGE, outputArg);
    if (SNOOPY_OUTPUT_FAILED(retVal)) {
        fatalError("Output failure");
    }


    /* Housekeeping and return */
    snoopy_cleanup();
    return 0;
}
