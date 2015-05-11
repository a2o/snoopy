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

    /* Initialize snoopy */
    snoopy_init();

    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\0';

    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);

    if (SNOOPY_TRUE == snoopy_configuration.config_file_enabled) {
        printf("Configuration file is enabled: %s\n", snoopy_configuration.config_file_path);
        if (SNOOPY_TRUE == snoopy_configuration.config_file_found) {
            printf("Configuration file found.\n");
        } else {
            printf("WARNING: Configuration file does not exist!\n");
        }
        if (SNOOPY_TRUE == snoopy_configuration.config_file_parsed) {
            printf("Configuration file was parsed sucessfully.\n");
        } else {
            printf("WARNING: Configuration file parsing FAILED!\n");
        }
    } else {
        printf("INFO: Configuration file is NOT enabled.\n");
    }

    snoopy_message_generateFromFormat(logMessage, snoopy_configuration.message_format);
    printf("Message generated:\n");
    printf("\n");
    printf("%s\n", logMessage);
    printf("\n");

#if defined(SNOOPY_FILTERING_ENABLED)
    /* Should message be passed to syslog or not? */
    if (
        (SNOOPY_FALSE == snoopy_configuration.filtering_enabled)
        ||
        (
            (SNOOPY_TRUE == snoopy_configuration.filtering_enabled)
            &&
            (SNOOPY_FILTER_PASS == snoopy_filtering_check_chain(logMessage, snoopy_configuration.filter_chain))
        )
    ) {
#endif
        snoopy_log_dispatch(logMessage, SNOOPY_LOG_MESSAGE);
        printf("Message sent to output '%s(%s)'.\n", snoopy_configuration.output, snoopy_configuration.output_arg);
        printf("If snoopy is already enabled on your system, you should see two identical messages.\n");
        printf("If you are testing snoopy via LD_PRELOAD environmental variable, you will see another identical message.\n");
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
