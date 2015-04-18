/*
 * SNOOPY LOGGER
 *
 * File: snoopy/input/rpname.c
 *
 * Copyright (c) 2015 ajzach@gmail.com
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

#define _GNU_SOURCE
#include "snoopy.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ROOT_PID        1
#define EMPTY_PID       0

#define PROC_NAME       "Name"
#define PROC_PPID       "PPid"
#define UNKNOWN_STR     "(unknown)"

/* Remove white spaces on the property */
char* trim_whitespace(char *str) {
    char *end;

    while (isspace(*str))
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
        end--;

    *(end + 1) = 0;

    return str;
}

/* Read /proc/{pid}/status file and extract the property */
char* read_proc_property(int pid, char* prop_name) {
    FILE   *fp;
    char   *line = NULL;
    size_t  len = 0;
    char   *k;
    char   *v;
    char   *tmp;
    char    pid_file[50];

    sprintf(pid_file, "/proc/%d/status", pid);
    fp = fopen(pid_file, "r");
    if (fp != NULL) {
        while (getline(&line, &len, fp) != -1) {

            /* The format must be prop_name:value */
            if (strstr(line, ":") == NULL) {
                fclose(fp);
                return NULL;
            }

            k = strtok(line, ":");
            v = strtok(NULL, ":");
            if (v == NULL) {
                fclose(fp);
                return NULL;
            }

            if (strcmp(prop_name, k) == 0) {
                fclose(fp);
                tmp = v+1;
                tmp[strlen(tmp)-1] = 0;
                return tmp;
            }
        }
        fclose(fp);
    }
    return NULL;
}

/* Get parent pid */
int get_parent_pid(int pid) {
    char* ppid;
    ppid = read_proc_property(pid, PROC_PPID);
    if (ppid != NULL)
        return atoi(ppid);
    return EMPTY_PID;
}

/* Find root process name */
int get_rpname(int pid, char *input) {
    int parent;
    char* name;

    parent = get_parent_pid(pid);
    if (parent == ROOT_PID) {
        name = read_proc_property(pid, PROC_NAME);
        if (name != NULL)
            return snprintf(input, SNOOPY_INPUT_MESSAGE_MAX_SIZE, "%s", name);
        else
            return snprintf(input, SNOOPY_INPUT_MESSAGE_MAX_SIZE, "%s",    UNKNOWN_STR);
    } else if (parent == EMPTY_PID)
        return snprintf(input, SNOOPY_INPUT_MESSAGE_MAX_SIZE, "%s", UNKNOWN_STR);
    else {
        return get_rpname(parent, input);
    }
}

int snoopy_input_rpname(char *input, char *arg) {
    return get_rpname(getpid(), input);
}
