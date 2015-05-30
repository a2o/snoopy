/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-output.c
 *
 * Copyright (c) 2014-2015 Bostjan Skufca <bostjan@a2o.si>
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
 * Include all required C resources
 */
#include <stdio.h>
#include <stdlib.h>



/*
 * Include all snoopy-related resources
 */
#include "snoopy.h"
#include "configuration.h"
#if defined(SNOOPY_FILTERING_ENABLED)
#include "filtering.h"
#endif
#include "inputdatastorage.h"
#include "log.h"
#include "message.h"
#include "misc.h"



int main (int argc, char **argv)
{
    char *logMessage       = NULL;
    snoopy_configuration_t *CFG;


    /* Initialize Snoopy */
    snoopy_init();

    /* Get config pointer */
    CFG = snoopy_configuration_get();


    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\0';

    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);

    if (SNOOPY_TRUE == CFG->configfile_enabled) {
        printf("Configuration file is enabled: %s\n", CFG->configfile_path);
        if (SNOOPY_TRUE == CFG->configfile_found) {
            printf("Configuration file found.\n");
        } else {
            printf("WARNING: Configuration file does not exist!\n");
        }
        if (SNOOPY_TRUE == CFG->configfile_parsed) {
            printf("Configuration file was parsed sucessfully.\n");
        } else {
            printf("WARNING: Configuration file parsing FAILED!\n");
        }
    } else {
        printf("INFO: Configuration file is NOT enabled.\n");
    }

    snoopy_message_generateFromFormat(logMessage, CFG->message_format);
    printf("Message generated:\n");
    printf("\n");
    printf("%s\n", logMessage);
    printf("\n");

#if defined(SNOOPY_FILTERING_ENABLED)
    /* Should message be passed to syslog or not? */
    if (
        (SNOOPY_FALSE == CFG->filtering_enabled)
        ||
        (
            (SNOOPY_TRUE == CFG->filtering_enabled)
            &&
            (SNOOPY_FILTER_PASS == snoopy_filtering_check_chain(logMessage, CFG->filter_chain))
        )
    ) {
#endif
        snoopy_log_dispatch(logMessage, SNOOPY_LOG_MESSAGE);
        printf("Message sent to output '%s(%s)'.\n", CFG->output, CFG->output_arg);
        printf("If Snoopy is already enabled on your system, you should see two identical messages.\n");
        printf("If you are testing Snoopy via LD_PRELOAD environmental variable, you will see another identical message.\n");
#if defined(SNOOPY_FILTERING_ENABLED)
    } else {
            printf("Message NOT sent to syslog. One of the filters dropped it.\n");
    }
#endif

    /* Housekeeping */
    free(logMessage);
    snoopy_cleanup();
    return 0;
}
