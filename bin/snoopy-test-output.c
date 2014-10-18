/*
 * SNOOPY LOGGER
 *
 * snoopy-test-output
 * Copyright (c) 2014 bostjan@a2o.si
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
#include "snoopy.h"
#include "log.h"
#include "inputdatastorage.h"
#include "filterregistry.h"
#include <stdio.h>
#include <stdlib.h>



int main (int argc, char **argv)
{
    char *logMessage       = NULL;

    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\0';

    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);

    snoopy_log_message_generate(logMessage, SNOOPY_LOG_MESSAGE_FORMAT);
    printf("Message generated:\n");
    printf("\n");
    printf("%s\n", logMessage);
    printf("\n");

#if defined(SNOOPY_FILTER_ENABLED)
    if (SNOOPY_FILTER_PASS == snoopy_log_filter_check_chain(logMessage, SNOOPY_FILTER_CHAIN)) {
        /* Send it to syslog */
        snoopy_log_send_to_syslog(logMessage);
        printf("Message sent to syslog, check your syslog output.\n");
        printf("If snoopy is already enabled on your system, you should see two identical messages.\n");
        printf("If you are testing snoopy via LD_PRELOAD environmental variable, you will see another identical message.\n");
    } else {
        printf("Message NOT sent to syslog. One of the filters dropped it.\n");
    }
#else
    snoopy_log_send_to_syslog(logMessage);
    printf("Message sent to syslog, check your syslog output.\n");
    printf("If snoopy is already enabled on your system, you should see two identical messages.\n");
    printf("If you are testing snoopy via LD_PRELOAD environmental variable, you will see another identical message.\n");
#endif

    /* Housekeeping */
    free(logMessage);
    return 0;
}
