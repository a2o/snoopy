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

#include "configuration.h"
#include "filterregistry.h"
#include "inputdatastorage.h"
#include "misc.h"

#include <stdio.h>



/*
 * We do not use separate .h file here
 */
int  main (int argc, char **argv);
void displayHelp();
int  fatalError(char *errorMsg);



int main (int argc, char **argv)
{
    char *message;
    char *filterName;
    char *filterArg;
    int   filterResult;


    /* Initialize Snoopy */
    snoopy_configuration_preinit_disableConfigFileParsing();
    snoopy_init();
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
        return fatalError("Missing argument: filter name");
    }
    filterName = argv[2];

    /* Is there an argument for this data source */
    if (3 < argc) {
        filterArg = argv[3];
    } else {
        filterArg = "";
    }


    /* Check if what we got is a valid filter name */
    if (SNOOPY_FALSE == snoopy_filterregistry_doesNameExist(filterName)) {
        displayHelp();
        return fatalError("Invalid filter name given");
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
    printf("    snoopy-test-filter LOG_MESSAGE FILTER [FILTER_ARG]\n");
    printf("\n");

    printf("Available filters:\n");
    int fCount = snoopy_filterregistry_getCount();
    for (int i=0 ; i<fCount ; i++) {
        printf("    %s\n", snoopy_filterregistry_getName(i));
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
