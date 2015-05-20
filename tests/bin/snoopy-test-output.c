/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-output.c
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
//#include "snoopy-test-output.h"

#include "snoopy.h"
#include "inputdatastorage.h"
#include "outputregistry.h"

#include <stdio.h>
#include <string.h>


/*
 * We do not use separate .h file here
 */
int  main (int argc, char **argv);
void displayHelp();
int  fatalError(char *errorMsg);



int main (int argc, char **argv)
{
    char *message;
    char *outputName;
    char *outputArg;
    int   retVal;


    /* Initialize Snoopy */
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);


    /* Check if all arguments are present */
    if (argc < 2) {
        displayHelp();
        return fatalError("Missing argument: log message");
    }
    message = argv[1];

    if (argc < 3) {
        displayHelp();
        return fatalError("Missing argument: output name");
    }
    outputName = argv[2];

    /* Is there an argument for this data source */
    if (3 < argc) {
        outputArg = argv[3];
    } else {
        outputArg = "";
    }


    /* Check if what we got is a valid output name */
    if (SNOOPY_FALSE == snoopy_outputregistry_isRegistered(outputName)) {
        displayHelp();
        return fatalError("Invalid output name given");
    }

    /* Dispatch message to output */
    retVal = snoopy_outputregistry_call(outputName, message, SNOOPY_LOG_MESSAGE, outputArg);
    if (SNOOPY_OUTPUT_FAILED(retVal)) {
        return fatalError("Output failure");
    }

    /* Display and return */
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
    printf("    snoopy-test-output LOG_MESSAGE OUTPUT [OUTPUT_ARG]\n");
    printf("\n");

    printf("Available outputs:\n");
    for (int i=0 ; '\0' != snoopy_outputregistry_names[i][0] ; i++) {
        printf("    %s\n", snoopy_outputregistry_names[i]);
    }
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
