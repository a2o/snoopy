/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/filter/exclude_spawns_of.c
 *
 * Copyright (c) 2015 Datto, Inc. All rights reserved.
 * Author: Fred Mora - fmora@datto.com
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
#include "exclude_spawns_of.h"

#include "snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>



/*
 * Local defines
 */
#define   PROGLISTSEP               ','
// Max bytes for command, including null
#define   ST_COMM_SIZE_MAX           32
// Max bytes needed from /proc/<pid>/stat, including null:
// 20 digit PID, space, command in parens, space, 1 char state, space,
// 20 digit PPID, null
#define   ST_BUF_SIZE               (46 + ST_COMM_SIZE_MAX)
// Min valid bytes from /proc/<pid>/stat, excluding null:
// 1 digit PID, space, empty parens, space, 1 char state, space,
// 1 digit PPID
#define   ST_SIZE_MIN                 8
// Path "/proc/nnnn/stat" where nnnn = some PID
#define   ST_PATH_SIZE_MAX           32


/*
 * Non-public function prototypes
 */
static int find_ancestor_in_list(char **name_list);
static int find_string_in_array(const char *str, char **str_array);
static char **string_to_token_array(char *str);



/*
 * SNOOPY FILTER: exclude_spawns_of
 *
 * Description:
 *     Excludes all log messages for executables that have the specified program name in their ancestors.
 *     Strategy: We parse arg to create the "list of specified programs" (LoSP).
 *     Then, we walk up the parent process ID (PPID) chain and we check if each executable name is part
 *     of the LoSP.
 *
 * Params:
 *     logMessage: Pointer to string that contains formatted log message (may be manipulated)
 *     arg:        Comma-separated list of program names for the spawns of which log messages are dropped.
 *
 * Return:
 *     SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP
 */
int snoopy_filter_exclude_spawns_of (char const * const arg)
{
    char  *argDup;   // Must not alter arg
    char **losp; // List of specified programs derived from arg
    int is_ancestor_in_list = 0;

    // Turn comma-separated arg into array of program name strings
    argDup = strdup(arg);
    losp = string_to_token_array(argDup);
    if (losp == NULL) {
        // If failure, we cannot filter anything, just pass the message
        return SNOOPY_FILTER_PASS;
    }

    // Check if one of the program names in losp is an ancestor
    is_ancestor_in_list = find_ancestor_in_list(losp);
    free(losp);
    free(argDup);
    return (is_ancestor_in_list == 1) ? SNOOPY_FILTER_DROP : SNOOPY_FILTER_PASS; // Error means pass
}

// Helper functions

/**
 * Description:
 *    Walks the /proc tree from /proc/currentPID and iterate parent PIDs up to PID 1. For each PID, check if the
 *    executable name is in name_list.
 * Params:
 *    name_list: Ptr to array of char ptr. Each element point to the name of an executable.
 * Return:
 *    1 if the executable name of an ancestor process is found in name_list
 *    0 if there are no ancestor that have a name found in name_list
 *   -1 if error.
 */
static int find_ancestor_in_list(char ** name_list)
{
    pid_t ppid;
    char stat_path[ST_PATH_SIZE_MAX];
    FILE * statf;
    int rc;
    int found;
    const char * left;
    const char * right;
    size_t len;

    /*
     * The following vars are read from /proc/PID/stat
     *
     * st_buf:
     * Buffer for PID, command, state, and PPID
     * st_comm:
     * Buffer for command. In kernel/include/linux/sched.h this is
     * defined as 16 byte string, but let's keep a bit of spare room if
     * something suddenly changes in the kernel.
     */
    char st_buf[ST_BUF_SIZE];
    char st_comm_buf[ST_COMM_SIZE_MAX];
    char st_state;

    if (name_list == NULL) {
        return -1;
    }
    ppid = getppid(); // We start with the parent
    while (ppid != 0) {
        // Create the path to /proc/<ppid>/stat
        snprintf(stat_path, ST_PATH_SIZE_MAX, "/proc/%d/stat", ppid);
        statf = fopen(stat_path, "r");
        if (statf == NULL) {
            return -1;
        }

        // Grab the first few elements from the stat pseudo-file
        rc = (int) fread(st_buf, 1, ST_BUF_SIZE - 1, statf);
        st_buf[rc] = '\0';
        fclose(statf);
        if (rc < ST_SIZE_MIN) {
            return -1;
        }

        // Find the first opening paren and the last closing paren
        left = strchr(st_buf, '(');
        right = strrchr(st_buf, ')');
        if (left == NULL || right == NULL) {
            return -1;
        }
        len = right - left - 1;
        if (len <= 0 || len >= ST_COMM_SIZE_MAX) {
            return -1;
        }

        // Copy the command
        memcpy(st_comm_buf, left + 1, len);
        st_comm_buf[len] = '\0';

        // Parse the PPID
        rc = sscanf(right + 1, " %c %d", &st_state, &ppid);
        if (rc != 2) {
            return -1;
        }

        found = find_string_in_array(st_comm_buf, name_list);

        if (found) {
            return 1;
        }
    }

    return 0; // Nothing found
}

/**
 * Description:
 *    Searches for a string in an array of strings. All strings are zero-terminated.
 * Params:
 *    str: Ptr to the string we are looking for in str_array.
 *    str_array: Ptr to array of char ptr, each of them pointing to a string to compare to str.
 * Return:
 *    1 if str matches one of the strings in str_array
 *    0 if there are no matches or if either argument is NULL.
 */
static int find_string_in_array(const char *str, char **str_array)
{
    if ((str == NULL) || (str_array == NULL)) {
        return 0;
    }
    char **p = str_array;
    while (*p != NULL) {
        if (strcmp(str, *p) == 0) {
            return 1;
        }
        p++;
    }
    return 0;
}

/**
 * Description:
 *    Tokenizes string str using comma as a separator. Returns an array of individual tokens between delimiters.
 *
 * Params:
 *    str: string to be tokenized. The string is modified as per strtok (delimiters replaced with \0)..
 * Returns:
 *    Ptr to array of char ptr. Each element point to an individual substring between delimiters.
 *    The last element is the NULL pointer to act as a delimiter.
 *    The returned pointer should be freed by the caller after use.
 *    If str is NULL or empty, or if error, returns NULL.
 */

static char **string_to_token_array(char *str)
{
    char *p;
    int sepcount = 0;
    int token_count;
    char **token_array; // Return value
    char *saveptr = NULL; // For strtok_r()

    if ((str == NULL) || (*str == '\0')) {
        return NULL;
    }

    // Count the occurences of PROGLISTSEP
    p = strchr(str, PROGLISTSEP);
    while (p != NULL) {
        sepcount++;
        p=strchr(p+1, PROGLISTSEP);
    }

    // Allocate storage for token_array: one more than the separator count, and one more for the NULL delimiter.
    token_count = sepcount +1;
    token_array = calloc(token_count + 1, sizeof(p));
    if (token_array == NULL) {
        return NULL;
    }

    // Fill in token_array with ptrs to individual tokens
    char delim[] = { PROGLISTSEP, '\0'}; // Make a string of delimiters
    p = str;
    for (int i = 0; i < token_count; i++) {
        token_array[i] = strtok_r(p, delim, &saveptr);
        p = NULL;
    }
    token_array[token_count] = NULL;
    return token_array;
}

