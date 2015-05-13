/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/legacy.c
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
 * SNOOPY DATA SOURCE: legacy
 *
 * Description:
 *     Returns current date and time in ISO 8601 forma.
 *
 * Params:
 *     result: pointer to string, to write result into
 *
 * Return:
 *     number of characters in the returned string
 */
#include "snoopy.h"
#include "inputdatastorage.h"
//#include <errno.h>
//#include <stdio.h>
//#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
//#include <unistd.h>
#include <limits.h>



/* Helper */
#define legacy_min(a,b) a<b ? a : b



int snoopy_datasource_legacy (char *result, char *arg)
{
    char   *logString       = NULL;
    size_t  logStringSize   = 0;
    char    cwd[PATH_MAX+1];
    char   *getCwdRet       = NULL;

    char   *ttyPath         = NULL; 
    char    ttyPathEmpty[]  = ""; 


    int     i               = 0;
    int     n               = 0;
    int     argc            = 0;


    /* Count number of arguments */
    for (argc=0 ; *(snoopy_inputdatastorage_argv+argc) != '\0' ; argc++);


    /* Get ttyname */
    ttyPath = ttyname(0);
    if (ttyPath == NULL) {
        ttyPath = ttyPathEmpty;
    }


    /* Allocate memory for logString */
    logStringSize = 0;
    for (i=0 ; i<argc ; i++) {
        /* Argument length + space */
        logStringSize += sizeof(logString[0]) * (strlen(snoopy_inputdatastorage_argv[i]) + 1);
    }
    /* +1 for last \0 */
    logStringSize     = legacy_min(SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, logStringSize+1);
    logString         = malloc(sizeof *logString * logStringSize);


    /* Create logString, and protect against overflows */
    logString[0] = '\0';
    for (i = n = 0 ; i<argc ; i++) {
        n += snprintf(logString+n, logStringSize-n, "%s", snoopy_inputdatastorage_argv[i]);
        if (n >= logStringSize)
            break;
        logString[n++] = ' ';
    }
    logString[logStringSize-1] = '\0';

    /* Create logMessage */
    getCwdRet = getcwd(cwd, PATH_MAX+1);
    if (NULL == getCwdRet) {
        strcpy(cwd, "(error)");
    }

    // Return log message
    return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "[uid:%d sid:%d tty:%s cwd:%s filename:%s]: %s", getuid(), getsid(0), ttyPath, cwd, snoopy_inputdatastorage_filename, logString);
}
