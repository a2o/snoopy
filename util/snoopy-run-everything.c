/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-filter.c
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
//#include "snoopy-test-filter.h"

#include "snoopy.h"
#include "libsnoopy-debug-addons.h"

#include "configuration.h"
#ifdef SNOOPY_FILTERING_ENABLED
#include "filtering.h"
#endif
#include "log.h"
#include "message.h"
#include "misc.h"
#include "inputdatastorage.h"

#include <stdio.h>
#include <stdlib.h>



int  main (int argc, char **argv);



int main (int argc, char **argv)
{
    char                     *logMessage = NULL;
    snoopy_configuration_t   *CFG;


    /* Initialize Snoopy */
    snoopy_init();
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);

    /* Get config pointer */
    CFG = snoopy_configuration_get();


    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\0';


    /* Run throught as much code as possible */
    /* TODO: Parse config file with all settings */
    printf("-----[ Datasources ]-----------------------------------\n");
    snoopy_debug_test_all_datasources();
#ifdef SNOOPY_FILTERING_ENABLED
    printf("-----[ Filters ]---------------------------------------\n");
    snoopy_debug_test_all_filters();
#endif
    printf("-----[ Outputs ]---------------------------------------\n");
    snoopy_debug_test_all_outputs();


    printf("-----[ Message formatting ]----------------------------\n");
    snoopy_message_generateFromFormat(logMessage, CFG->message_format);
    printf("Message: %s\n", logMessage);

    printf("-----[ Filtering ]-------------------------------------\n");
#ifdef SNOOPY_FILTERING_ENABLED
    snoopy_filtering_check_chain(logMessage, "exclude_uid:10,11,12;only_uid=0,1,2,3");
    printf("Done.\n");
#else
    printf("SKIPPED - not enabled.\n");
#endif

    printf("-----[ Dispatching ]-----------------------------------\n");
    snoopy_log_dispatch(logMessage, SNOOPY_LOG_MESSAGE);
    printf("Done.\n");

    printf("\nAll done.\n");

    /* Cleanup and return */
    free(logMessage);
    snoopy_cleanup();

    /* Close these FDs too, otherwise valgrind complains */
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    return 0;
}
