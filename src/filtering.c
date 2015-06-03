/*
 * SNOOPY LOGGER
 *
 * File: filtering.c
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
#include "filtering.h"

#include "snoopy.h"
#include "configuration.h"
#include "filterregistry.h"
#include "message.h"

#ifndef _POSIX_SOURCE   // For strtok_r
#define _POSIX_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * snoopy_filtering_check_chain
 *
 * Description:
 *     Determines whether given message should be send to syslog or not
 *
 * Params:
 *     logMessage:   message about to be sent to syslog
 *     chain:        filter chain to check
 *
 * Return:
 *     SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP
 */
int snoopy_filtering_check_chain (
    char *logMessage,
    char *filterChain
) {
    char  filterChainCopy[SNOOPY_FILTER_CHAIN_MAX_SIZE];   // Must be here, or strtok_r segfaults
    int   filterChainCopySize;
    int   j;
    char *str;
    char *rest;
    char *filterSpec;            // Single filter specification from defined filter chain
    char *fcPos_filterSpecArg;   // Pointer to argument part of single filter specification in a filter chain

    // Copy the filter chain specification to separate string, to be used in strtok_r
    filterChainCopySize = strlen(filterChain);
    if (filterChainCopySize > SNOOPY_FILTER_CHAIN_MAX_SIZE - 1) {
        filterChainCopySize = SNOOPY_FILTER_CHAIN_MAX_SIZE - 1;
    }
    strncpy(filterChainCopy, filterChain, filterChainCopySize);
    filterChainCopy[filterChainCopySize] = '\0';

    // Loop through all filters
    for (j=1, str=filterChainCopy;  ; j++, str=NULL) {
        char    filterName[SNOOPY_FILTER_NAME_MAX_SIZE];
        char   *filterNamePtr;
        size_t  filterNameSize;
        char    filterArg[SNOOPY_FILTER_ARG_MAX_SIZE];
        char   *filterArgPtr;

        // Parse the remaining filter chain specification for next filterSpec
        filterSpec = strtok_r(str, ";", &rest);
        if (NULL == filterSpec) {
            // We are at the end of filtering chain
            break;
        }

        // If filter tag contains ":", then split it into filter name and filter argument
        fcPos_filterSpecArg  = strstr(filterSpec, ":");
        if (NULL == fcPos_filterSpecArg) {
            // filterSpec == filterName, there is no argument
            filterName[0] = '\0';
            filterNamePtr = filterSpec;
            filterArg[0]  = '\0';
            filterArgPtr  = filterArg;
        } else {
            // Change the colon to null character, which effectively splits the string in two parts.
            // Then point to first and second part with corresponding variables.
            filterNameSize = fcPos_filterSpecArg - filterSpec;
            filterName[0] = '\0';
            strncpy(filterName, filterSpec, filterNameSize);
            filterName[filterNameSize] = '\0';
            filterNamePtr = filterName;
            filterArgPtr  = fcPos_filterSpecArg + 1;
        }

        // Check if filter actually exists
        if (SNOOPY_FALSE == snoopy_filterregistry_doesNameExist(filterNamePtr)) {
            snoopy_message_append(logMessage, "ERROR(Filter not found - ");
            snoopy_message_append(logMessage, filterNamePtr);
            snoopy_message_append(logMessage, ")");
            break;
        }

        // Consult the filter, and return immediately if message should be dropped
        if (SNOOPY_FILTER_DROP == snoopy_filterregistry_callByName(filterNamePtr, logMessage, filterArgPtr)) {
            return SNOOPY_FILTER_DROP;
        }
    }
    return SNOOPY_FILTER_PASS;
}
