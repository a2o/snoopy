/*
 * SNOOPY LOGGER
 *
 * File: snoopy/filter/only_uid.c
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
#include "only_uid.h"

#include "snoopy.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>



/*
 * SNOOPY FILTER: only_uid
 *
 * Description:
 *     Excludes all log messages not comming from specific UID
 *
 * Params:
 *     logMessage: pointer to string that contains formatted log message (may be manipulated)
 *     arg:        Comma-separated list of UIDs for which log message is passed on, dropped for all others
 *
 * Return:
 *     SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP
 */
int snoopy_filter_only_uid (char *msg, char const * const arg)
{
    uid_t  curUid;     // Actual UID of running process
    char  *argDup    = NULL;
    char **argParsed = NULL;
    int    argCount  = 0;
    int    retVal    = -1;


    /* Get uid of current process */
    curUid = getuid();

    /* Parse arguments - values are malloc()-ed */
    argDup   = strdup(arg);
    argCount = snoopy_parser_argList_csv(argDup, &argParsed);

    /* Loop through all UIDs passed to the filter as argument */
    for (int i=0 ; i<argCount ; i++) {
        uid_t  argCurUid; // Actual UID to be used for comparison

        // Convert literal UID to numeric type
        argCurUid = atol(argParsed[i]);

        // Compare and return PASS if matches
        if (argCurUid == curUid) {
            retVal = SNOOPY_FILTER_PASS;
            goto RETURN_AND_FREE_ALL;
        }
    }
    retVal = SNOOPY_FILTER_DROP;
    goto RETURN_AND_FREE_ALL;


RETURN_AND_FREE_ALL:
    /* Cleanup */
    free(argDup);
    free(argParsed);

    // None of the UIDs matched so far, therefore we are dropping this
    return retVal;
}
