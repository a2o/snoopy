/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2022 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "cgroup.h"

#include "snoopy.h"

#include "util/file-snoopy.h"
#include "util/string-snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



/*
 * Local defines
 */
#define PROC_PID_CGROUP_PATH_SIZE_MAX 32



static int doesCgroupEntryContainController (char const * const cgroupEntry, char const * const controllerName);



int snoopy_datasource_cgroup (char * const result, char const * const arg)
{
    int myPid;
    char   procPidCgroupFilePath[PROC_PID_CGROUP_PATH_SIZE_MAX];
    char * procPidCgroupContent = NULL;

    char * cgroupEntry = NULL;
    int    retMsgLen;


    // Verify the argument
    if (0 == strcmp(arg, "")) {
        snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "Missing cgroup selection argument");
        return SNOOPY_DATASOURCE_FAILURE;
    }


    // Generate /proc/PID/cgroup path
    myPid = getpid();
    snprintf(procPidCgroupFilePath, PROC_PID_CGROUP_PATH_SIZE_MAX, "/proc/%d/cgroup", myPid);


    // Get the cgroup info content
    if (snoopy_util_file_getSmallTextFileContent(procPidCgroupFilePath, &procPidCgroupContent) < 0) {
        snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "Unable to read file %s, reason: %s", procPidCgroupFilePath, procPidCgroupContent);
        free(procPidCgroupContent);
        return SNOOPY_DATASOURCE_FAILURE;
    }


    // Search
    if (SNOOPY_TRUE == snoopy_util_string_containsOnlyDigits(arg)) {

        // Search mode: ^MYNUMBER:.*
        size_t searchStringLen;
        char * searchString = NULL;

        searchStringLen = strlen(arg) + 2; // +1 for the ':' and +1 for the '\0'
        searchString = malloc(searchStringLen);
        snprintf(searchString, searchStringLen, "%s:", arg);

        cgroupEntry = snoopy_util_string_findLineStartingWith(procPidCgroupContent, searchString);

        free(searchString);

        if (cgroupEntry) {
            snoopy_util_string_nullTerminateLine(cgroupEntry);
        }

    } else {

        // Search mode: ^[^:]+:MYTEXT:
        // This search mangles the procPidCgroupContent, but it leaves a null-terminated
        // cgroupEntry ready for subsequent use.
        char * nextEntry = NULL;

        cgroupEntry = strtok_r(procPidCgroupContent, "\n", &nextEntry);
        while (cgroupEntry) {

            if (SNOOPY_TRUE == doesCgroupEntryContainController(cgroupEntry, arg)) {
                break;
            }

            cgroupEntry = strtok_r(NULL, "\n", &nextEntry);
        }
    }


    // Not found?
    if (NULL == cgroupEntry) {
        free(procPidCgroupContent);
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", "(none)");
    }


    // Found
    retMsgLen = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", cgroupEntry);
    free(procPidCgroupContent);
    return retMsgLen;
}



static int doesCgroupEntryContainController (char const * const cgroupEntry, char const * const controllerName)
{
    const char * firstColon;
    const char * controllerList;
    char       * secondColon;


    // Find relevant colon characters to isolate the controller list
    firstColon = strchr(cgroupEntry, ':');
    if (!firstColon) {
        return SNOOPY_ERROR;
    }
    controllerList = firstColon+1;

    secondColon = strchr(controllerList, ':');
    if (!secondColon) {
        return SNOOPY_ERROR;
    }


    // If controller list is empty, we can't match then
    if (controllerList == secondColon) {
        return SNOOPY_FALSE;
    }


    // Change second colon to \0, to enable subsequent search
    *secondColon = '\0';


    // Check whole literal match
    if (0 == strcmp(controllerList, controllerName)) {
        *secondColon = ':';
        return SNOOPY_TRUE;
    }


    // Is there even a ',' character in controller list - if not, cut the execution short
    if (!strchr(controllerList, ',')) {
        *secondColon = ':';
        return SNOOPY_FALSE;
    }


    // Parse the comma-separated list of controllers
    const char * tokenPtr = controllerList;
    while (tokenPtr) {

        char * commaPtr = strchr(tokenPtr, ',');
        if (commaPtr) {
            *commaPtr = '\0';
        }

        // Token matches
        if (0 == strcmp(tokenPtr, controllerName)) {
            if (commaPtr) *commaPtr = ',';
            *secondColon = ':';
            return SNOOPY_TRUE;
        }

        // Last iteration
        if (commaPtr) {
            // Not found, still stuff to search, so search on
            *commaPtr = ',';
            tokenPtr = commaPtr + 1;
        } else {
            // Last iteration
            tokenPtr = NULL;
            break;
        }
    }


    *secondColon = ':';
    return SNOOPY_FALSE;
}
