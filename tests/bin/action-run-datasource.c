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
#include "action-run-datasource.h"
#include "action-common.h"

#include "snoopy.h"
#include "entrypoint/test-cli.h"

#include "configuration.h"
#include "error.h"
#include "datasourceregistry.h"
#include "inputdatastorage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void snoopyTestCli_action_run_datasource_showList ()
{
    printf("Available datasources:\n");
    int dCount = snoopy_datasourceregistry_getCount();
    for (int i=0 ; i<dCount ; i++) {
        printf("    %s\n", snoopy_datasourceregistry_getName(i));
    }
}



void snoopyTestCli_action_run_datasource_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `run` :: Subsystem `datasource`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test run datasource DATASOURCE [ARGS]\n"
        "    snoopy-test run datasource --all    # Runs all datasources\n"
        "    snoopy-test run datasource --help   # Shows this help message\n"
        "    snoopy-test run datasource --list   # Lists all available datasources\n"
        "\n";
    printf("%s", helpContent);

    snoopyTestCli_action_run_datasource_showList();
}



int snoopyTestCli_action_run_datasource (int argc, char ** argv)
{
    const char *arg1;
    const char *datasourceName;
    const char *datasourceArg;
    char  datasourceResult[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE];
    int   retVal;


    /* Initialize Snoopy */
    snoopy_entrypoint_test_cli_init((char const *)g_argv[0], g_argv, NULL);


    /* Check if there is a data source name passed as an argument */
    if (argc < 1) {
        snoopyTestCli_action_run_datasource_showHelp();
        fatalError("Missing argument: `datasource name` or `--list`");
    }
    arg1 = argv[0];


    /* Is second argument --list? */
    if (0 == strcmp(arg1, "--all")) {
        snoopyTestCli_action_run_datasource_all();
        return 0;
    }
    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_run_datasource_showHelp();
        return 0;
    }
    if (0 == strcmp(arg1, "--list")) {
        snoopyTestCli_action_run_datasource_showList();
        return 0;
    }
    datasourceName = arg1;


    /* Check if what we got is a valid datasource name */
    if (SNOOPY_FALSE == snoopy_datasourceregistry_doesNameExist(datasourceName)) {
        snoopyTestCli_action_run_datasource_showHelp();
        fatalError("Invalid datasource name given");
    }

    /* Is there an argument for this data source */
    if (argc >= 2) {
        datasourceArg = argv[1];
    } else {
        datasourceArg = "";
    }


    /* Call the datasource */
    retVal = snoopy_datasourceregistry_callByName(datasourceName, datasourceResult, datasourceArg);
    if (SNOOPY_DATASOURCE_FAILED(retVal)) {
        fatalErrorValue("Datasource failed", datasourceResult);
    }


    /* Display */
    printf("%s\n", datasourceResult);


    /* Housekeeping and return */
    snoopy_entrypoint_test_cli_exit();
    return 0;
}



void snoopyTestCli_action_run_datasource_all ()
{
    char *itemName   = NULL;
    const char *itemArgs   = NULL;
    char *itemResult = NULL;
    int   itemResultSize;
    int   dCount;

    /* Initialize variables and spaces */
    itemResult = malloc(SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE + 1);

    /* Loop through all datasources and just send to output */
    dCount = snoopy_datasourceregistry_getCount();
    for (int i=0 ; i<dCount ; i++) {

        itemName = snoopy_datasourceregistry_getName(i);
        printf("Datasource %15s: ", itemName);

        /* Which arguments to pass to data source */
        if (strcmp(itemName, "env") == 0) {
            itemArgs = "HOME";
        } else if (strcmp(itemName, "snoopy_literal") == 0) {
            itemArgs = "somestring";
        } else {
            itemArgs = "";
        }

        /* Execute the data source function */
        itemResultSize = snoopy_datasourceregistry_callById(i, itemResult, itemArgs);
        if (itemResultSize > SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE) {
            snoopy_error_handler("Maximum data source message size exceeded");
        }

        /* Copy content, append */
        printf("%s\n", itemResult);
    }

    /* Memory housekeeping */
    free(itemResult);
}
