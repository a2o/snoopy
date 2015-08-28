/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/rpname.c
 *
 * Copyright (c) 2015 Ariel Zach <ajzach@gmail.com>
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
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "rpname.h"

#include "snoopy.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>



/*
 * Local defines
 */
#define PID_ROOT                1
#define PID_EMPTY               0

#define PROC_PID_STATUS_KEY_NAME        "Name"
#define PROC_PID_STATUS_KEY_PPID        "PPid"

#define PROC_PID_STATUS_VAL_MAX_LENGTH          NAME_MAX      // Pid is max 2^2 (7-digit number), name can be max 255 bytes
#define PROC_PID_STATUS_VAL_MAX_LENGTH_STR      PROC_PID_STATUS_VAL_MAX_LENGTH + 1   // +1 for null termination

#define UNKNOWN_STR             "(unknown)"



/*
 * Non-public function prototypes
 */
int   get_parent_pid (int pid);
int   get_rpname (int pid, char *result);
char* read_proc_property (int pid, char* prop_name);



/*
 * SNOOPY DATA SOURCE: rpname
 *
 * Description:
 *     Returns root process name of current process tree.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_rpname (char * const result, char const * const arg)
{
    return get_rpname(getpid(), result);
}



/* Read /proc/{pid}/status file and extract the property */
char* read_proc_property (int pid, char* prop_name)
{
    char    pid_file[50];
    FILE   *fp;
    char   *line = NULL;
    size_t  lineLen = 0;
    char   *k;
    char   *v;
    size_t  vLen = 0;
    char    returnValue[PROC_PID_STATUS_VAL_MAX_LENGTH_STR] = "";

    /* Open file or return */
    sprintf(pid_file, "/proc/%d/status", pid);
    fp = fopen(pid_file, "r");
    if (NULL == fp) {
        return NULL;
    }

    /* Read line by line */
    while (getline(&line, &lineLen, fp) != -1) {

        /* If line is empty, bail out - no such thing in /proc/PID/status */
        if (0 == lineLen) {
            goto RETURN_FREE_LINE_AND_CLOSE_FILE;
        }

        /*
         * The format must be "prop_name: value".
         * Otherwise bail out altogether - something must be wrong with this /proc/PID/status file
         */
        if (NULL == strstr(line, ":")) {
            goto RETURN_FREE_LINE_AND_CLOSE_FILE;
        }

        /*
         * Separate line content into two tokens: key and value
         * If separation fails, continue to the next line ("Groups:" key is one such example)
         *
         * Explicitly initialized as pointer to "", otherwise coverity complains with
         * "Uninitialized pointer read (UNINIT)".
         */
        char *savePtr = "";
        k = strtok_r(line, ":", &savePtr);
        v = strtok_r(NULL, ":", &savePtr);
        if (NULL == v) {
            continue;
        }

        /* The key we are looking for? */
        if (strcmp(prop_name, k) == 0) {
            /* Yes! */
            v++;                  // There is one tab in front of PID number
            vLen = strlen(v);
            v[vLen-1] = 0;        // Terminate the newline at the end of value
            vLen--;               // Length is now shorter for 1 character

            /*
             * Choose string copy mode depending on length of PID
             * - prevent segfault if sth happens to MAX PID in future
             */
            if (vLen > PROC_PID_STATUS_VAL_MAX_LENGTH) {
                strncpy(returnValue, v, PROC_PID_STATUS_VAL_MAX_LENGTH);
                returnValue[PROC_PID_STATUS_VAL_MAX_LENGTH_STR-1] = 0; // Change newline into null character
            } else {
                strncpy(returnValue, v, PROC_PID_STATUS_VAL_MAX_LENGTH_STR);
            }

            // Do a cleanup and return a string duplicate, which should be freed by the caller
            free(line);
            fclose(fp);
            return strdup(returnValue);
        }

        /*
         * Line is not freed between subsequent iteration as the same buffer is reused
         * (and realloc()-ed if required)
         */
    }

    RETURN_FREE_LINE_AND_CLOSE_FILE:
    /* Only free if this was actually allocated */
    if (NULL != line) {
        free(line);
    }
    fclose(fp);
    return NULL;
}



/* Get parent pid */
int get_parent_pid (int pid)
{
    char *ppid_str;
    int   ppid_int;

    ppid_str = read_proc_property(pid, PROC_PID_STATUS_KEY_PPID);
    if (NULL != ppid_str) {
        ppid_int = atoi(ppid_str);
        free(ppid_str);
        return ppid_int;
    }

    return PID_EMPTY;
}



/* Find root process name */
int get_rpname (int pid, char *result)
{
    int     parentPid;
    char   *name;
    size_t  nameLen;

    parentPid = get_parent_pid(pid);
    if (PID_ROOT == parentPid) {
        name = read_proc_property(pid, PROC_PID_STATUS_KEY_NAME);
        if (NULL != name) {
            nameLen = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", name);
            free(name);
        } else {
            nameLen = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", UNKNOWN_STR);
        }
        return nameLen;
    } else if (PID_EMPTY == parentPid) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", UNKNOWN_STR);
    } else {
        return get_rpname(parentPid, result);
    }
}
