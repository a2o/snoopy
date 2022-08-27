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
#include "action-common.h"

#include "action-run-datasource.h"
#include "action-run-filter.h"
#include "action-run-output.h"

#include "snoopy.h"
#include "entrypoint/test-cli.h"

#include "configuration.h"
#ifdef SNOOPY_FILTERING_ENABLED
#include "filtering.h"
#endif
#include "inputdatastorage.h"
#include "action/log-message-dispatch.h"
#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>






void snoopyTestCli_action_run_everything_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `run` :: Run everything (for Valgrind)\n"
        "\n"
        "Usage:\n"
        "    snoopy-test run everything\n"
        "\n"
        "Result:\n"
        "    Runs as many subsystems as possible, to cover as much code as possible.\n"
        "    Useful for Valgrind analysis.\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_run_everything ()
{
    char                     *logMessage = NULL;
    const snoopy_configuration_t * CFG;


    /* Initialize Snoopy */
    snoopy_entrypoint_test_cli_init((char const *)g_argv[0], g_argv, NULL);

    /* Get config pointer */
    CFG = snoopy_configuration_get();


    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_BUF_SIZE);
    logMessage[0] = '\0';


    /* Run throught as much code as possible */
#ifdef SNOOPY_FILTERING_ENABLED
    printf("-----[ Filters ]---------------------------------------\n");
    snoopyTestCli_action_run_filter_all();
#endif
    printf("-----[ Filtering ]-------------------------------------\n");
#ifdef SNOOPY_FILTERING_ENABLED
    snoopy_filtering_check_chain("exclude_uid:10,11,12;only_uid=0,1,2,3");
    printf("Done.\n");
#else
    printf("SKIPPED - not enabled.\n");
#endif
    printf("-----[ Datasources ]-----------------------------------\n");
    snoopyTestCli_action_run_datasource_all();
    printf("-----[ Outputs ]---------------------------------------\n");
    snoopyTestCli_action_run_output_all();


    printf("-----[ Message formatting ]----------------------------\n");
    snoopy_message_generateFromFormat(logMessage, SNOOPY_LOG_MESSAGE_BUF_SIZE, CFG->message_format);
    printf("Message: %s\n", logMessage);


    printf("-----[ Dispatching ]-----------------------------------\n");
    snoopy_action_log_message_dispatch(logMessage);
    printf("Done.\n");

    printf("\nAll done.\n");


    /* Cleanup and return */
    free(logMessage);
    snoopy_entrypoint_test_cli_exit();

    /* Close these FDs too, otherwise valgrind complains */
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    return 0;
}
