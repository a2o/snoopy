/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-datasource.c
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
//#include "snoopy-test-datasource.h"

#include "snoopy.h"
#include "inputdatastorage.h"
#include "datasourceregistry.h"

#include <stdio.h>



/*
 * We do not use separate .h file here
 */
int  main (int argc, char **argv);
void displayHelp();
int  fatalError(char *errorMsg);



int main (int argc, char **argv)
{
    char *datasourceName;
    char *datasourceArg;
    char  datasourceResult[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE];
    int   retVal;


    /* Initialize Snoopy */
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);


    /* Check if there is a data source name passed as an argument */
    if (argc < 2) {
        displayHelp();
        return fatalError("Missing argument: datasource name");
    }
    datasourceName = argv[1];

    /* Check if what we got is a valid datasource name */
    if (SNOOPY_FALSE == snoopy_datasourceregistry_isRegistered(datasourceName)) {
        displayHelp();
        return fatalError("Invalid datasource name given");
    }

    /* Is there an argument for this data source */
    if (2 < argc) {
        datasourceArg = argv[2];
    } else {
        datasourceArg = "";
    }


    /* Call the datasource */
    retVal = snoopy_datasourceregistry_call(datasourceName, datasourceResult, datasourceArg);
    if (SNOOPY_DATASOURCE_FAILED(retVal)) {
        return fatalError("Datasource failed");
    }


    /* Display and return */
    printf("%s\n", datasourceResult);
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
    printf("    snoopy-test-datasource DATASOURCE [DATASOURCE_ARG]\n");
    printf("\n");

    printf("Available datasources:\n");
    for (int i=0 ; '\0' != snoopy_datasourceregistry_names[i][0] ; i++) {
        printf("    %s\n", snoopy_datasourceregistry_names[i]);
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
