/*
 * SNOOPY LOGGER
 *
 * File: snoopy-dev-helper.c
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
#include "inputdatastorage.h"
#include "message.h"
#include "misc.h"



int main (int argc, char **argv)
{
    char *logMessage = NULL;

    /* Initialize snoopy */
    snoopy_init();

    /* Fake init */
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);

    // Init log message as empty string
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\0';

    printf("Snoopy: testing custom message formatting:\n\n");
    printf("Message format used: \n%s\n\n", SNOOPY_LOG_MESSAGE_FORMAT);
    snoopy_message_generateFromFormat(logMessage, SNOOPY_LOG_MESSAGE_FORMAT);
    printf("Produced output:\n%s\n\n", logMessage);


    /* Housekeeping */
    free(logMessage);
    snoopy_cleanup();
    return 0;
}
