/*
 * SNOOPY LOGGER
 *
 * File: snoopy/input/env_all.c
 *
 * Copyright (c) 2015 bostjan@a2o.si
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
 * SNOOPY INPUT: env_all
 *
 * Description:
 *     Returns all environmental variables
 *
 * Params:
 *     input: pointer to string, to write result into
 *     arg:   (ignored) - TODO which environmental variables to ignore
 *
 * Return:
 *     number of characters in the returned string
 */
#include "snoopy.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>


/*
 * Use this extenal variable
 */
extern char **environ;



int snoopy_input_env_all (char *input, char *arg)
{
    int inputSize = 0; // Current size of message to be returned back - does not include trailing null character

    // Loop through all environmental variables
    char *envItem = *environ; // Get first environmental variable
    int i = 1;
    for ( ; NULL!=envItem ; i++) {
        int   remInputSize    = 0;
        remInputSize = SNOOPY_INPUT_MESSAGE_MAX_SIZE - inputSize;

        // Add comma if required - must be still 5 characters available for this (,...\0)
        if ((i > 1) && (remInputSize >= 5)) {
            input[inputSize] = ',';
            input[inputSize+1] = '\0';
            inputSize++;
            remInputSize--;
        }

        // TODO Add support for excluding certain environmental variables via *arg

        // Do we append whole environmental variable, or just part of it?
        // +3 to account for ... and +1 for null character
        if (strlen(envItem) + 3 + 1 < remInputSize) {
            int strSizeCopied = 0;

            // Append whole ENV variable
            strSizeCopied = snprintf(&input[inputSize], remInputSize, "%s", envItem);
            inputSize += strSizeCopied;

        } else {
            int strSizeToCopy = 0;

            // This one includes null char, therefore the actual string length that will be copied is X-1 characters + \0
            strSizeToCopy = remInputSize - 3; // -4 to account for "...\0"
            snprintf(&input[inputSize], strSizeToCopy, "%s", envItem);
            inputSize += strSizeToCopy - 1;   // Account for added \0 at the end - we do not use strSizeCopied here, which already includes it

            strSizeToCopy = 4; // -4 to account for "...\0"
            snprintf(&input[inputSize], strSizeToCopy, "...");
            inputSize += strSizeToCopy-1;
            break;
        }

        // Get next environmental variable
        envItem = *(environ+i);
    }

    return inputSize;
}
