/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-message-format.c
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
//#include "snoopy-test-message-format.h"

#include "snoopy.h"

#include "configuration.h"
#include "inputdatastorage.h"
#include "message.h"
#include "misc.h"

#include <stdio.h>
#include <stdlib.h>



/*
 * We do not use separate .h file here
 */
int  main (int argc, char **argv);
void displayHelp();
int  fatalError(char *errorMsg);



int main (int argc, char **argv)
{
    char *messageFormat;
    char *message;


    /* Initialize Snoopy */
    snoopy_configuration_preinit_disableConfigFileParsing();
    snoopy_init();
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);


    /* Check if all arguments are present */
    if (argc < 2) {
        displayHelp();
        return fatalError("Missing argument: message format");
    }
    messageFormat = argv[1];


    /* Initialize message */
    message    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    message[0] = '\0';


    /* Call the filter */
    snoopy_message_generateFromFormat(message, messageFormat);


    /* Display result */
    printf("%s\n", message);


    /* Housekeeping and return */
    free(message);
    snoopy_cleanup();
    return 0;
}



/*
 * displayHelp()
 *
 * Description:
 *     Displays help
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void displayHelp ()
{
    printf("\n");
    printf("Usage: \n");
    printf("    snoopy-test-message-format MESSAGE_FORMAT\n");
    printf("\n");
}



/*
 * fatalError()
 *
 * Description:
 *     Displays error message + help and returns non-zero exit status
 *
 * Params:
 *     errorMsg   Error message to display to user
 *
 * Return:
 *     int        Exit status to return to calling process
 */
int fatalError (char *errorMsg)
{
    printf("ERROR: %s\n", errorMsg);
    printf("\n");
    return 127;
}
