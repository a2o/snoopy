/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-all-data-sources.c
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
#include <string.h>



/*
 * Include all snoopy-related resources
 */
#include "snoopy.h"
#include "configuration.h"
#include "error.h"
#include "inputdatastorage.h"
#include "datasourceregistry.h"
#include "misc.h"



/*
 * We do not use separate .h file here
 */
int  main (int argc, char **argv);
void snoopy_test_all_datasources ();
int  snoopy_get_datasource_name_length_max ();



int main (int argc, char **argv)
{
    /* Initialize snoopy */
    snoopy_init();
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);

    /* Run the main function */
    snoopy_test_all_datasources();

    /* Housekeeping */
    snoopy_cleanup();
    return 0;
}



/*
 * snoopy_test_all_datasources
 *
 * Description:
 *     Loops through all data source and prints what they return
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_test_all_datasources ()
{
    int   datasourceNameLengthMax;
    char *datasourceResult = NULL;
    char *datasourceArgs    = NULL;
    int   i;
    int   j;

    /* Initialize variables and spaces */
    datasourceResult       = malloc(SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE + 1);
    datasourceNameLengthMax = snoopy_get_datasource_name_length_max();

    /* Loop throught all data sources and just append the output */
    i = 0;
    while (strcmp(snoopy_datasourceregistry_names[i], "") != 0) {
        datasourceResult[0]  = '\0';
        int datasourceResultSize = -1;

        /* Which arguments to pass to data source */
        if (strcmp(snoopy_datasourceregistry_names[i], "env") == 0) {
            datasourceArgs = "HOME";
        } else {
            datasourceArgs = "";
        }

        /* Execute the data source function */
        datasourceResultSize = snoopy_datasourceregistry_ptrs[i](datasourceResult, datasourceArgs);
        if (datasourceResultSize > SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE) {
            snoopy_error_handler("Maximum data source message size exceeded");
        }

        /* Copy content, append */
        printf("%s:", snoopy_datasourceregistry_names[i]);
        for (j=0 ; j<=(datasourceNameLengthMax-strlen(snoopy_datasourceregistry_names[i])) ; j++) {
            printf(" ");
        }
        printf("%s\n", datasourceResult);

        /* Go to next data source */
        i++;
    }

    /* Memory housekeeping */
    free(datasourceResult);
}



/*
 * snoopy_get_max_datasource_name_length
 *
 * Description:
 *     Loops through all data sources and returns length of the longest data source name
 *
 * Params:
 *     (none)
 *
 * Return:
 *     int   max data source name length
 */
int snoopy_get_datasource_name_length_max ()
{
    int   datasourceNameLengthMax;
    int   i;

    datasourceNameLengthMax = 0;
    i = 0;
    while (strcmp(snoopy_datasourceregistry_names[i], "") != 0) {
        if (strlen(snoopy_datasourceregistry_names[i]) > datasourceNameLengthMax) {
            datasourceNameLengthMax = strlen(snoopy_datasourceregistry_names[i]);
        }

        /* Go to next data source provider */
        i++;
    }

    return datasourceNameLengthMax;
}
