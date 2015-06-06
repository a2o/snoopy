/*
 * SNOOPY LOGGER
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



/* Helper */
#define min(a,b) a<b ? a : b



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
int snoopy_datasource_cmdline (char * const result, char const * const arg)
{
    char   *cmdLine = NULL;
    int     cmdLineArgCount;
    int     cmdLineSizeSum;   // Size sum of all arguments and spaces in between
    int     cmdLineSizeRet;   // Size that will be returned
    int     i;
    int     n;
    snoopy_inputdatastorage_t   *snoopy_inputdatastorage;

    /* Get argument data of execv/e() call */
    snoopy_inputdatastorage = snoopy_inputdatastorage_get();

    /* Count number of arguments */
    for (cmdLineArgCount=0 ; *(snoopy_inputdatastorage->argv+cmdLineArgCount) != '\0' ; cmdLineArgCount++);

    /* Calculate memory requirement for cmdLine */
    cmdLineSizeSum = 0;
    for (i=0 ; i<cmdLineArgCount ; i++) {
        /* Argument length + space */
        cmdLineSizeSum += strlen(snoopy_inputdatastorage->argv[i]) + 1;
    }
    /* Last space will be converted to \0 */
    cmdLineSizeRet = min(SNOOPY_SYSCONF_ARG_MAX, cmdLineSizeSum);

    /* Initialize cmdLine */
    cmdLine    = malloc(cmdLineSizeRet);
    cmdLine[0] = '\0';

    for (i = n = 0 ; i<cmdLineArgCount ; i++) {
        /* Did adding space in previous iteration cause this? */
        if (n >= cmdLineSizeRet) {
            break;
        }
        n += snprintf(cmdLine+n, cmdLineSizeRet-n, "%s", snoopy_inputdatastorage->argv[i]);

        if (n >= cmdLineSizeRet) {
            break;
        }
        cmdLine[n] = ' ';
        n++;
    }

    /*
     * Conclude string - add \0 at the end
     * - if last character is space
     * - or if last character is ordinary character from incompletely copied argument
     */
    n--;
    cmdLine[n] = '\0';

    /* Copy the result to the string pointed by return pointer */
    snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", cmdLine);

    free(cmdLine);
    return cmdLineSizeRet;
}
