/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/datasource/cmdline.c
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
 * Includes order: from local to global
 */
#include "cmdline.h"

#include "snoopy.h"
#include "inputdatastorage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * SNOOPY DATA SOURCE: cmdline
 *
 * Description:
 *     Returns command line of current process.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_cmdline (char * const result, __attribute__((unused)) char const * const arg)
{
    const snoopy_inputdatastorage_t * snoopy_inputdatastorage;
    size_t bytesWrittenToResult;

    /* Get argument data of execv/e() call */
    snoopy_inputdatastorage = snoopy_inputdatastorage_get();

    /*
     * DirectAdmin does not follow the execve() convention (as described in its man page)
     * and passes NULL argv instead of what it's supposed to do. Let's work around that.
     *
     * Additionally, let's handle the `(char *[]) { NULL }` case in the same manner too.
     */
    if (
        (snoopy_inputdatastorage->argv == NULL)
        ||
        (snoopy_inputdatastorage->argv[0] == NULL)
    ) {
        if (NULL == snoopy_inputdatastorage->filename) {
            return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(unknown)");
        } else {
            return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", snoopy_inputdatastorage->filename);
        }
    }

    // Recompose the command into a result string
    bytesWrittenToResult = 0;
    for (int argId=0 ; snoopy_inputdatastorage->argv[argId] != NULL ; argId++) {

        // Add space before every non-first argument
        if ((argId > 0) && (bytesWrittenToResult < SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE)) {
            bytesWrittenToResult += snprintf(
                result+bytesWrittenToResult,
                SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE - bytesWrittenToResult,
                " "
            );
        }

        // Copy the content
        if (bytesWrittenToResult < SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE) {
            bytesWrittenToResult += snprintf(
                result+bytesWrittenToResult,
                SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE - bytesWrittenToResult,
                "%s", snoopy_inputdatastorage->argv[argId]
            );
        }
    }

    // Ensure the presence of a terminating null character
    if (bytesWrittenToResult < SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE) {
        result[bytesWrittenToResult] = '\0';
    } else {
        result[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE-1] = '\0';
    }

    return (int) bytesWrittenToResult;
}
