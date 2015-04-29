/*
 * SNOOPY LOGGER
 *
 * File: datasourceregistry.c
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
 * Include all data source functions
 */
#include "snoopy.h"
#include "datasourceregistry.h"
#include <string.h>



/*
 * Two arrays holding data about all data sources
 */
char *snoopy_datasourceregistry_names[] = {
    "cmdline",
    "cwd",
    "egid",
    "egroup",
    "env",
    "env_all",
    "euid",
    "eusername",
    "filename",
    "gid",
    "group",
    "login",
    "pid",
    "ppid",
    "rpname",
    "sid",
    "snoopy_version",
    "tty",
    "tty_uid",
    "tty_username",
    "uid",
    "username",
    "",
};

int (*snoopy_datasourceregistry_ptrs []) (char *result, char *arg) = {
    snoopy_datasource_cmdline,
    snoopy_datasource_cwd,
    snoopy_datasource_egid,
    snoopy_datasource_egroup,
    snoopy_datasource_env,
    snoopy_datasource_env_all,
    snoopy_datasource_euid,
    snoopy_datasource_eusername,
    snoopy_datasource_filename,
    snoopy_datasource_gid,
    snoopy_datasource_group,
    snoopy_datasource_login,
    snoopy_datasource_pid,
    snoopy_datasource_ppid,
    snoopy_datasource_rpname,
    snoopy_datasource_sid,
    snoopy_datasource_snoopy_version,
    snoopy_datasource_tty,
    snoopy_datasource_tty_uid,
    snoopy_datasource_tty_username,
    snoopy_datasource_uid,
    snoopy_datasource_username,
};



/*
 * isRegistered()
 *
 * Return true if data source exists, otherwise return false
 */
int snoopy_datasourceregistry_isRegistered (char *providerName)
{
    if (snoopy_datasourceregistry_getIndex(providerName) == -1) {
        return SNOOPY_FALSE;
    } else {
        return SNOOPY_TRUE;
    }
}



/*
 * getIndex()
 *
 * Return index of given data source, or -1 if not found 
 */
int snoopy_datasourceregistry_getIndex (char *providerName)
{
    int i;

    i = 0;
    while (strcmp(snoopy_datasourceregistry_names[i], "") != 0) {
        if (strcmp(snoopy_datasourceregistry_names[i], providerName) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}



/*
 * call()
 *
 * Call the given data source and return its output
 */
int snoopy_datasourceregistry_call (char *providerName, char *returnMessage, char *providerArg)
{
    int idx;

    idx = snoopy_datasourceregistry_getIndex(providerName);
    if (idx == -1) {
        return -1;
    }

    return snoopy_datasourceregistry_ptrs[idx](returnMessage, providerArg);
}
