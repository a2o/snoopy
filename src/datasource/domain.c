/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/domain.c
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
// This needs to be defined before including sys/utsname.h,
// or struct utsname does not return domainname member.
#define   _GNU_SOURCE

#include "domain.h"

#include "snoopy.h"

#include   <errno.h>
#include   <stdio.h>
#include   <string.h>
#include   <unistd.h>



/*
 * Local defines
 */
#define   HOSTS_PATH            "/etc/hosts"
#define   HOSTS_LINE_SIZE_MAX   1024
#define   HOSTS_LINE_POS_MAX    1023



/*
 * SNOOPY DATA SOURCE: domain
 *
 * Description:
 *     Returns domain of this system.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_domain (char * const result, char const * const arg)
{
    FILE *fp;
    char  hostname[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE+1];   // +1 - we add "." to the end of string
    char  line[HOSTS_LINE_SIZE_MAX];
    int   retVal;
    int   tmpInt;

    /*
     * START: COPY FROM datasource/hostname
     */
    /* Get my hostname first */
    retVal = gethostname(hostname, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE);
    if (0 != retVal) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(error @ gethostname(): %d)", errno);
    }

    // If hostname was something alien (longer than 1024 characters),
    // set last char to null just in case
    hostname[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE-1] = '\0';
    /*
     * END: Copy from datasource/hostname
     */

    /* Check hostname length */
    if (0 == strlen(hostname)) {
        snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "Got empty hostname");
        return SNOOPY_DATASOURCE_FAILURE;
    }
    /* Add a dot at the end of hostname, that is what we are searching for */
    tmpInt = strlen(hostname);
    hostname[tmpInt] = '.';
    hostname[tmpInt+1] = '\0';


    /* Try to open file in read mode */
    fp = fopen(HOSTS_PATH, "r");
    if (NULL == fp) {
        snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "Unable to open file for reading: %s", HOSTS_PATH);
        return SNOOPY_OUTPUT_FAILURE;
    }


    /* Read line by line */
    char *linePtr;
    char *hashPtr;
    char *lineEntryPtr;
    char *savePtr;
    char *domainPtr = NULL;

    while (NULL != (linePtr = fgets(line, sizeof(line), fp))) {

        /* Is line a comment - ignore everything after '#' character */
        if (NULL != (hashPtr = strchr(linePtr, '#'))) {
            hashPtr = '\0';
        }

        /* Try to find "hostname." there */
        lineEntryPtr = strcasestr(linePtr, hostname);
        if (NULL != lineEntryPtr) {
            strtok_r(lineEntryPtr, " \t\n\r", &savePtr);

            /* Does not matter whether token has been found or not. */
            /* If it was, strtok creates \0 at the end of it and returns pointer to it. */
            /* If it was not, we must have met the EOF and just use everything. */
            domainPtr = lineEntryPtr + strlen(hostname);
            break;
        }
    }


    /* Cleanup and return */
    fclose(fp);
    if (NULL != domainPtr) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", domainPtr);
    } else {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "(none)");
    }
}
