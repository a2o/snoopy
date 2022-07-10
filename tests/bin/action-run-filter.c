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
#include "filterregistry.h"
#include "inputdatastorage.h"
#include "misc.h"

#include <stdio.h>
#include <string.h>






void snoopyTestCli_action_run_filter_showList ()
{
    printf("Available filters:\n");
    int fCount = snoopy_filterregistry_getCount();
    for (int i=0 ; i<fCount ; i++) {
        printf("    %s\n", snoopy_filterregistry_getName(i));
    }
}



void snoopyTestCli_action_run_filter_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `run` :: Subsystem `filter`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test run filter \"LOG MESSAGE\" FILTER [FILTER_ARGS]\n"
        "    snoopy-test run filter --list\n"
        "    snoopy-test run filter --help\n"
        "\n"
        "Result:\n"
        "    Prints the result of a called filter as a \"PASS\" or a \"DROP\" to stdout.\n"
        "    Sets the exit status to 0 or PASS or 1 for DROP.\n"
        "\n";
    printf("%s", helpContent);

    snoopyTestCli_action_run_filter_showList();
}



int snoopyTestCli_action_run_filter (int argc, char **argv)
{
    char * message;
    const char * filterName;
    const char * filterArg;
    int filterResult;


    /* Initialize Snoopy */
    snoopy_configuration_preinit_disableConfigFileParsing();
    snoopy_init();
    snoopy_inputdatastorage_store_filename(g_argv[0]);
    snoopy_inputdatastorage_store_argv(g_argv);


    /* Check if all arguments are present */
    if (argc < 1) {
        snoopyTestCli_action_run_filter_showHelp();
        fatalError("Missing argument: log message, --help or --list");
    }
    message = argv[0];
    if (0 == strcmp(message, "--help")) {
        snoopyTestCli_action_run_filter_showHelp();
        return 0;
    }
    if (0 == strcmp(message, "--list")) {
        snoopyTestCli_action_run_filter_showList();
        return 0;
    }

    if (argc < 2) {
        snoopyTestCli_action_run_filter_showHelp();
        fatalError("Missing argument: filter name");
    }
    filterName = argv[1];

    /* Is there an argument for this data source */
    if (argc > 2) {
        filterArg = argv[2];
    } else {
        filterArg = "";
    }


    /* Check if what we got is a valid filter name */
    if (SNOOPY_FALSE == snoopy_filterregistry_doesNameExist(filterName)) {
        snoopyTestCli_action_run_filter_showHelp();
        fatalErrorValue("Invalid filter name given", filterName);
    }

    /* Call the filter */
    filterResult = snoopy_filterregistry_callByName(filterName, message, filterArg);


    /* Housekeeping */
    snoopy_cleanup();


    /* Display and return */
    if (SNOOPY_FILTER_PASS == filterResult) {
        printf("PASS\n");
        return 0;
    } else {
        printf("DROP\n");
        return 1;
    }
}
