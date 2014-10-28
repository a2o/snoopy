/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-all-inputs.c
 *
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
#include "inputdatastorage.h"
#include "inputregistry.h"
#include "misc.h"



/*
 * We do not use separate .h file here
 */
int  main (int argc, char **argv);
void snoopy_test_all_inputs ();
int  snoopy_get_input_name_length_max ();



int main (int argc, char **argv)
{
    char *logMessage       = NULL;

    /* Initialize snoopy */
    snoopy_init();
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);

    /* Run the main function */
    snoopy_test_all_inputs();

    /* Housekeeping */
    snoopy_cleanup();
    return 0;
}



/*
 * snoopy_test_all_inputs
 *
 * Description:
 *     Loops through all inputs and prints what they return
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_test_all_inputs ()
{
    int   inputNameLengthMax;
    char *inputMessage = NULL;
    char *inputArgs    = NULL;
    int   i;
    int   j;

    /* Initialize variables and spaces */
    inputMessage       = malloc(SNOOPY_INPUT_MESSAGE_MAX_SIZE + 1);
    inputNameLengthMax = snoopy_get_input_name_length_max();

    /* Loop throught all inputs and just append the output */
    i = 0;
    while (strcmp(snoopy_inputregistry_names[i], "") != 0) {
        inputMessage[0]  = '\0';
        int inputMessageSize = -1;

        /* Which arguments to pass to input provider */
        if (strcmp(snoopy_inputregistry_names[i], "env") == 0) {
            inputArgs = "HOME";
        } else {
            inputArgs = "";
        }

        /* Execute the input function */
        inputMessageSize = snoopy_inputregistry_ptrs[i](inputMessage, inputArgs);
        if (inputMessageSize > SNOOPY_INPUT_MESSAGE_MAX_SIZE) {
            snoopy_error_handler("Maximum input message size exceeded");
        }

        /* Copy content, append */
        printf("%s:", snoopy_inputregistry_names[i]);
        for (j=0 ; j<=(inputNameLengthMax-strlen(snoopy_inputregistry_names[i])) ; j++) {
            printf(" ");
        }
        printf("%s\n", inputMessage);

        /* Go to next input provider */
        i++;
    }

    /* Memory housekeeping */
    free(inputMessage);
}



/*
 * snoopy_get_max_input_name_length
 *
 * Description:
 *     Loops through all inputs and finds largest input name length
 *
 * Params:
 *     (none)
 *
 * Return:
 *     int   max input name length
 */
int snoopy_get_input_name_length_max ()
{
    int   inputNameLengthMax;
    int   i;

    inputNameLengthMax = 0;
    i = 0;
    while (strcmp(snoopy_inputregistry_names[i], "") != 0) {
        if (strlen(snoopy_inputregistry_names[i]) > inputNameLengthMax) {
            inputNameLengthMax = strlen(snoopy_inputregistry_names[i]);
        }

        /* Go to next input provider */
        i++;
    }

    return inputNameLengthMax;
}
