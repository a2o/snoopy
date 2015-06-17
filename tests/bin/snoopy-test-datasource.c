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
#include "configuration.h"
#include "datasourceregistry.h"
#include "inputdatastorage.h"
#include "misc.h"

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
    char *datasourceName;
    char *datasourceArg;
    char  datasourceResult[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE];
    int   retVal;


    /* Initialize Snoopy */
    snoopy_configuration_preinit_disableConfigFileParsing();
    snoopy_init();
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);


    /* Check if there is a data source name passed as an argument */
    if (argc < 2) {
        displayHelp();
        return fatalError("Missing argument: datasource name or '--list'");
    }
    datasourceName = argv[1];


    /* Is second argument --list? */
    if (0 == strcmp(argv[1], "--list")) {

        /* Loop throught all data sources and just append the output */
        int dCount = snoopy_datasourceregistry_getCount();
        for (int i=0 ; i<dCount ; i++) {
            printf("%s\n", snoopy_datasourceregistry_getName(i));
        }
        return 0;
    }


    /* Check if what we got is a valid datasource name */
    if (SNOOPY_FALSE == snoopy_datasourceregistry_doesNameExist(datasourceName)) {
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
    retVal = snoopy_datasourceregistry_callByName(datasourceName, datasourceResult, datasourceArg);
    if (SNOOPY_DATASOURCE_FAILED(retVal)) {
        return fatalError("Datasource failed");
    }


    /* Display */
    printf("%s\n", datasourceResult);


    /* Housekeeping and return */
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
    printf("    snoopy-test-datasource DATASOURCE [DATASOURCE_ARG]\n");
    printf("    snoopy-test-datasource --list\n");
    printf("\n");

    printf("Available datasources:\n");
    int dCount = snoopy_datasourceregistry_getCount();
    for (int i=0 ; i<dCount ; i++) {
        printf("    %s\n", snoopy_datasourceregistry_getName(i));
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
