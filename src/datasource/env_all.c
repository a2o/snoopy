/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/env_all.c
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
#include "env_all.h"

#include "snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * Use this extenal variable
 */
extern char **environ;



/*
 * SNOOPY DATA SOURCE: env_all
 *
 * Description:
 *     Returns all environmental variables
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    which environmental variable(s) to ignore
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_env_all (char * const result, char const * const arg)
{
    int resultSize = 0; // Current size of message to be returned back - does not include trailing null character

    // Loop through all environmental variables
    char *envItem = *environ; // Get first environmental variable
    int i = 1;
    for ( ; NULL!=envItem ; i++) {
        int   remResultSize    = 0;
        remResultSize = SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE - resultSize;

        // Add comma if required - must be still 5 characters available for this (,...\0)
        if ((i > 1) && (remResultSize >= 5)) {
            result[resultSize] = ',';
            result[resultSize+1] = '\0';
            resultSize++;
            remResultSize--;
        }

        // TODO Add support for excluding certain environmental variables via *arg

        // Do we append whole environmental variable, or just part of it?
        // +3 to account for ... and +1 for null character
        if ((int)(strlen(envItem) + 3 + 1) < remResultSize) {
            int strSizeCopied = 0;

            // Append whole ENV variable
            strSizeCopied = snprintf(&result[resultSize], remResultSize, "%s", envItem);
            resultSize += strSizeCopied;

        } else {
            int strSizeToCopy = 0;

            // This one includes null char, therefore the actual string length that will be copied is X-1 characters + \0
            strSizeToCopy = remResultSize - 3; // -4 to account for "...\0"
            snprintf(&result[resultSize], strSizeToCopy, "%s", envItem);
            resultSize += strSizeToCopy - 1;   // Account for added \0 at the end - we do not use strSizeCopied here, which already includes it

            strSizeToCopy = 4; // -4 to account for "...\0"
            snprintf(&result[resultSize], strSizeToCopy, "...");
            resultSize += strSizeToCopy-1;
            break;
        }

        // Get next environmental variable
        envItem = *(environ+i);
    }

    return resultSize;
}
