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
#include "datasource/hostname.h"

#include   <stdio.h>
#include   <string.h>



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
int snoopy_datasource_domain (char *result, char *arg)
{
    FILE *fp;
    char  hostname[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE+1];   // +1 - we add "." to the end of string
    char  line[HOSTS_LINE_SIZE_MAX];
    int   retVal;
    int   tmpInt;

    /* Get my hostname first */
    retVal = snoopy_datasource_hostname(hostname, "");
    if (SNOOPY_DATASOURCE_FAILED(retVal)) {
        snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "Unable to get hostname");
        return SNOOPY_DATASOURCE_FAILURE;
    }
    if (0 == strlen(hostname)) {
        snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "Got empty hostname");
        return SNOOPY_DATASOURCE_FAILURE;
    }
    /* Add a dot at the end of hostname, that is what we are searching for */
    tmpInt = strlen(hostname);
    hostname[tmpInt] = '.';
    hostname[tmpInt+1] = '\0';


    /* Check access to hosts file */
    if (-1 == access(HOSTS_PATH, R_OK)) {
        snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "File not found or not readable: %s", HOSTS_PATH);
        return SNOOPY_DATASOURCE_FAILURE;
    }

    /* Try to open file in read mode */
    fp = fopen(HOSTS_PATH, "r");
    if (NULL == fp) {
        snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "Unable to open file for reading: %s", HOSTS_PATH);
        return SNOOPY_OUTPUT_FAILURE;
    }


    /* Read line by line */
    char *linePtr;
    char *lineEntryPtr;
    char *tokenPtr;
    char *savePtr;
    char *domainPtr;

    while (NULL != (linePtr = fgets(line, sizeof(line), fp))) {

        /* Try to find "hostname." there */
        lineEntryPtr = strcasestr(linePtr, hostname);
        if (NULL != lineEntryPtr) {
            tokenPtr = strtok_r(lineEntryPtr, " \t\n\r", &savePtr);
            if (NULL != tokenPtr) {
                /* Found. Strtok created \0 where appropriate */
                domainPtr = lineEntryPtr + strlen(hostname);
                break;
            } else {
                /* Token was not found, but this might be just the end of file. */
                domainPtr = lineEntryPtr + strlen(hostname);
                break;
            }
        }
    }


    /* Cleanup and return */
    fclose(fp);
    return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", domainPtr);
}
