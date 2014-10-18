/*
 * SNOOPY LOGGER
 *
 * File: inputregistry.c
 *
 * Copyright (c) 2014 bostjan@a2o.si
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
 * Include all input functions
 */
#include "snoopy.h"
#include "inputregistry.h"
#include <string.h>



/*
 * Two arrays holding data about input functions
 */
char *snoopy_inputregistry_names[] = {
    "cmdline",
    "cwd",
    "egid",
    "egroup",
    "env",
    "euid",
    "eusername",
    "filename",
    "gid",
    "group",
    "login",
    "sid",
    "tty",
    "tty_uid",
    "tty_username",
    "uid",
    "username",
    "",
};

int (*snoopy_inputregistry_ptrs []) (char *input, char *arg) = {
    snoopy_input_cmdline,
    snoopy_input_cwd,
    snoopy_input_egid,
    snoopy_input_egroup,
    snoopy_input_env,
    snoopy_input_euid,
    snoopy_input_eusername,
    snoopy_input_filename,
    snoopy_input_gid,
    snoopy_input_group,
    snoopy_input_login,
    snoopy_input_sid,
    snoopy_input_tty,
    snoopy_input_tty_uid,
    snoopy_input_tty_username,
    snoopy_input_uid,
    snoopy_input_username,
};



/*
 * isRegistered()
 *
 * Return true if input provider exists, otherwise return false
 */
int snoopy_inputregistry_isRegistered (char *providerName)
{
    if (snoopy_inputregistry_getIndex(providerName) == -1) {
        return SNOOPY_FALSE;
    } else {
        return SNOOPY_TRUE;
    }
}



/*
 * getIndex()
 *
 * Return index of given input provider, or -1 if not found 
 */
int snoopy_inputregistry_getIndex (char *providerName)
{
    int i;

    i = 0;
    while (strcmp(snoopy_inputregistry_names[i], "") != 0) {
        if (strcmp(snoopy_inputregistry_names[i], providerName) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}



/*
 * call()
 *
 * Call the given provider and return its output
 */
int snoopy_inputregistry_call (char *providerName, char *returnMessage, char *providerArg)
{
    int idx;

    idx = snoopy_inputregistry_getIndex(providerName);
    if (idx == -1) {
        return -1;
    }

    return snoopy_inputregistry_ptrs[idx](returnMessage, providerArg);
}
