/*
 * SNOOPY LOGGER
 *
 * File: snoopy/input/cmdline.c
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
 * SNOOPY INPUT: cmdline
 *
 * Description:
 *     Returns command line of current process.
 *
 * Params:
 *     input: pointer to string, to write result into
 *
 * Return:
 *     number of characters in the returned string
 */
#include "snoopy.h"
#include "inputdatastorage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* Helper */
#define min(a,b) a<b ? a : b



int snoopy_input_cmdline (char *input, char *arg)
{
    char   *cmdLine = NULL;
    int     cmdLineSize;
    int     argc;
    int     i;
    int     n;

    /* Count number of arguments */
    for (argc=0 ; *(snoopy_inputdatastorage_argv+argc) != '\0' ; argc++);

    /* Allocate memory for cmdLine */
    cmdLineSize = 0;
    for (i=0 ; i<argc ; i++) {
        /* Argument length + space */
        cmdLineSize += sizeof(cmdLine[0]) * (strlen(snoopy_inputdatastorage_argv[i]) + 1);
    }

    /* +1 for last \0 */
    cmdLineSize = min(SNOOPY_SYSCONF_ARG_MAX, cmdLineSize+1);
    cmdLine     = malloc(sizeof *cmdLine * cmdLineSize);

    /* Create cmdLine, and protect against overflows */
    cmdLine[0] = '\0';
    for (i = n = 0 ; i<argc ; i++) {
        n += snprintf(cmdLine+n, cmdLineSize-n, "%s", snoopy_inputdatastorage_argv[i]);
        if (n >= cmdLineSize) {
            break;
        }
        cmdLine[n++] = ' ';
    }

    /* Remove last space */
    cmdLineSize--;
    cmdLine[cmdLineSize-1] = '\0';

    /* Copy the result to the string pointed by return pointer */
    snprintf(input, SNOOPY_INPUT_MESSAGE_MAX_SIZE, "%s", cmdLine);

    free(cmdLine);
    return cmdLineSize;
}
