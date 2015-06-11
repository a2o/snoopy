/*
 * SNOOPY LOGGER
 *
 * File: libsnoopy-debug-addons.c
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
#include "libsnoopy-debug-addons.h"

#include "snoopy.h"
//#include "configuration.h"
#include "datasourceregistry.h"
#include "error.h"
#ifdef SNOOPY_FILTERING_ENABLED
#include "filterregistry.h"
#endif
#include "inputdatastorage.h"
//#include "misc.h"
#include "outputregistry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * snoopy_debug_test_all_datasources
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
void snoopy_debug_test_all_datasources ()
{
    char *itemName   = NULL;
    char *itemArgs   = NULL;
    char *itemResult = NULL;
    int   itemResultSize;
    int   i;
    int   dCount;

    /* Initialize variables and spaces */
    itemResult = malloc(SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE + 1);

    /* Loop throught all datasources and just send to output */
    dCount = snoopy_datasourceregistry_getCount();
    for (i=0 ; i<dCount ; i++) {

        itemName = snoopy_datasourceregistry_getName(i);
        printf("Datasource %15s: ", itemName);

        /* Which arguments to pass to data source */
        if (strcmp(itemName, "env") == 0) {
            itemArgs = "HOME";
        } else if (strcmp(itemName, "snoopy_literal") == 0) {
            itemArgs = "somestring";
        } else {
            itemArgs = "";
        }

        /* Execute the data source function */
        itemResultSize = snoopy_datasourceregistry_callById(i, itemResult, itemArgs);
        if (itemResultSize > SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE) {
            snoopy_error_handler("Maximum data source message size exceeded");
        }

        /* Copy content, append */
        printf("%s\n", itemResult);
    }

    /* Memory housekeeping */
    free(itemResult);
}



#ifdef SNOOPY_FILTERING_ENABLED
/*
 * snoopy_debug_test_all_filters
 *
 * Description:
 *     Loops through all filters and runs them, with bogus data.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_debug_test_all_filters ()
{
    char *message    = NULL;
    char *itemName   = NULL;
    char *itemArgs   = NULL;
    int   itemResult;
    int   fCount;
    int   i;


    /* Initialize variables and spaces */
    message  = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE + 1);
    snprintf(message, SNOOPY_LOG_MESSAGE_MAX_SIZE, "bad message here");

    /* Loop throught all filters and run them with bogus arguments */
    fCount = snoopy_filterregistry_getCount();
    for (i=0 ; i<fCount ; i++) {

        itemName = snoopy_filterregistry_getName(i);
        printf("Filter %19s: ", itemName);

        /* Which arguments to pass */
        if (strcmp(itemName, "exclude_spawns_of") == 0) {
            itemArgs = "asdf,bsdf";
        } else if (strcmp(itemName, "exclude_uid") == 0) {
            itemArgs = "0";
        } else if (strcmp(itemName, "only_root") == 0) {
            itemArgs = "";
        } else if (strcmp(itemName, "only_uid") == 0) {
            itemArgs = "0";
        } else {
            itemArgs = "";
        }

        /* Execute the filter function */
        itemResult = snoopy_filterregistry_callById(i, message, itemArgs);

        /* Evaluate */
        if (SNOOPY_FILTER_PASS == itemResult) {
            printf("PASS");
        } else {
            printf("DROP");
        }
        printf("\n");
    }

    /* Memory housekeeping */
    free(message);
}
#endif



/*
 * snoopy_debug_test_all_outputs
 *
 * Description:
 *     Loops through all applicable outputs and runs them, with bogus message.
 *
 * Params:
 *     message:   Content to pass to outputs
 *
 * Return:
 *     void
 */
void snoopy_debug_test_all_outputs ()
{
    char *message    = NULL;
    char *itemName   = NULL;
    char *itemArgs   = NULL;
    int   itemResult;
    int   iCount;
    int   i;


    /* Initialize variables and spaces */
    message  = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE + 1);
    snprintf(message, SNOOPY_LOG_MESSAGE_MAX_SIZE, "Snoopy output debugging");

    /* Loop throught all outputs and run them with bogus arguments */
    iCount = snoopy_outputregistry_getCount();
    for (i=0 ; i<iCount ; i++) {

        itemName = snoopy_outputregistry_getName(i);
        printf("Output %19s: ", itemName);

        /* Which arguments to pass */
        if (strcmp(itemName, "file") == 0) {
            itemArgs = "./fileoutput.out";
        } else if (strcmp(itemName, "socket") == 0) {
            itemArgs = "/dev/log";
        } else {
            itemArgs = "";
        }

        /* Execute the output function */
        itemResult = snoopy_outputregistry_callById(i, message, SNOOPY_LOG_MESSAGE, itemArgs);

        /* Evaluate */
        printf("%d chars transmitted. (output arg:%s)\n", itemResult, itemArgs);

        /* Cleanup */
        if (strcmp(itemName, "file") == 0) {
            unlink(itemArgs);
        }
    }

    /* Memory housekeeping */
    free(message);
}
