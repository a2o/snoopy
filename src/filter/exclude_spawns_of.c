/*
 * SNOOPY LOGGER
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
#define   ST_COMM_SIZE_MAX           32   // Keep these 3 in sync
#define   ST_COMM_SCANF_PRECISION_S "31"  // Keep these 3 in sync
#define   ST_COMM_LAST_CHAR_POS      31   // Keep these 3 in sync



/*
 * Non-public function prototypes
 */
int find_ancestor_in_list(char **name_list);
int find_string_in_array(char *str, char **str_array);
char **string_to_token_array(char *str);



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
int snoopy_filter_exclude_spawns_of(char *msg, char const * const arg)
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
int find_ancestor_in_list(char **name_list)
{
    pid_t ppid;
    char stat_path[32]; // Path "/proc/nnnn/stat" where nnnn = some PID
    FILE *statf;
    int rc, found;
    char *ancestor_name;

    /*
     * The following vars are read from /proc/PID/stat
     *
     * st_comm:
     * Buffer for exec file name. In kernel/include/linux/sched.h is this
     * defined as 16 byte string, but let's keep a bit of spare room if
     * something suddenly changes in the kernel.
     */
    char st_commBuff[ST_COMM_SIZE_MAX];
    char *st_comm;
    pid_t st_pid;
    char st_state;

    if (name_list == NULL) {
        return -1;
    }
    ppid = getppid(); // We start with the parent
    while (ppid != 1) {
        // Create the path to /proc/<ppid>/stat
        sprintf(stat_path, "/proc/%d/stat", ppid);
        statf = fopen(stat_path, "r");
        if (statf == NULL) {
            return -1;
        }

        // Grab the first few elements from the stat pseudo-file. Format from man 5 proc.
        st_comm = st_commBuff;
        rc = fscanf(statf, "%d %"ST_COMM_SCANF_PRECISION_S"s %c %d", &st_pid, st_comm, &st_state, &ppid);
        if (rc == EOF) {
            fclose(statf);
            return -1;
        }
        /*
         * Do not worry about %31s not being enough if command name in
         * /proc/PID/stat exceeds it. fscanf() returns OK, it just shortens
         * the resulting string to 31+\0.
         */

        // Secure string ending, just in case (this should be done by fscanf())
        st_comm[ST_COMM_LAST_CHAR_POS] = '\0';

        // stat provides st_comm as the name between parentheses. Get rid of the parens.
        ancestor_name = st_comm + 1;
        st_comm[strlen(st_comm) - 1] = '\0';
        found = find_string_in_array(ancestor_name, name_list);

        if (found) {
            fclose(statf);
            return 1;
        }

        fclose(statf);
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
int find_string_in_array(char *str, char **str_array)
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

char **string_to_token_array(char *str)
{
    char *p;
    int i;
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
    for (i = 0; i < token_count; i++) {
        token_array[i] = strtok_r(p, delim, &saveptr);
        p = NULL;
    }
    token_array[token_count] = NULL;
    return(token_array);
}

