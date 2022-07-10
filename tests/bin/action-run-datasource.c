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
#include "datasourceregistry.h"
#include "inputdatastorage.h"
#include "libsnoopy-debug-addons.h"
#include "misc.h"

#include <stdio.h>
#include <string.h>



void snoopyTestCli_action_run_datasource_listDatasources ()
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
        "    snoopy-test run datasource --list   # Lists all available data sources\n"
        "    snoopy-test run datasource --all    # Runs all datasources\n"
        "\n";
    printf("%s", helpContent);

    snoopyTestCli_action_run_datasource_listDatasources();
}



int snoopyTestCli_action_run_datasource (int argc, char ** argv)
{
    const char *datasourceName;
    const char *datasourceArg;
    char  datasourceResult[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE];
    int   retVal;


    /* Initialize Snoopy */
    snoopy_configuration_preinit_disableConfigFileParsing();
    snoopy_init();
    snoopy_inputdatastorage_store_filename((char const *)g_argv[0]);
    snoopy_inputdatastorage_store_argv(g_argv);



    /* Check if there is a data source name passed as an argument */
    if (argc < 1) {
        snoopyTestCli_action_run_datasource_showHelp();
        fatalError("Missing argument: `datasource name` or `--list`");
    }
    datasourceName = argv[0];


    /* Is second argument --list? */
    if (0 == strcmp(argv[0], "--list")) {
        snoopyTestCli_action_run_datasource_listDatasources();
        return 0;
    }
    if (0 == strcmp(argv[0], "--all")) {
        snoopy_debug_test_all_datasources();
        return 0;
    }


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
        fatalError("Datasource failed");
    }


    /* Display */
    printf("%s\n", datasourceResult);


    /* Housekeeping and return */
    snoopy_cleanup();
    return 0;
}
