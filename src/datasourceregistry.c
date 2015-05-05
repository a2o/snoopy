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
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cmdline
    "cmdline",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cwd
    "cwd",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_egid
    "egid",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_egroup
    "egroup",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_env
    "env",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_env_all
    "env_all",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_euid
    "euid",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_eusername
    "eusername",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_filename
    "filename",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_gid
    "gid",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_group
    "group",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_login
    "login",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_pid
    "pid",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_ppid
    "ppid",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_rpname
    "rpname",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_sid
    "sid",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_version
    "snoopy_version",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tty
    "tty",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tty_uid
    "tty_uid",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tty_username
    "tty_username",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_uid
    "uid",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_username
    "username",
#endif
    "",
};

int (*snoopy_datasourceregistry_ptrs []) (char *result, char *arg) = {
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cmdline
    snoopy_datasource_cmdline,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cwd
    snoopy_datasource_cwd,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_egid
    snoopy_datasource_egid,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_egroup
    snoopy_datasource_egroup,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_env
    snoopy_datasource_env,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_env_all
    snoopy_datasource_env_all,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_euid
    snoopy_datasource_euid,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_eusername
    snoopy_datasource_eusername,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_filename
    snoopy_datasource_filename,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_gid
    snoopy_datasource_gid,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_group
    snoopy_datasource_group,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_login
    snoopy_datasource_login,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_pid
    snoopy_datasource_pid,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_ppid
    snoopy_datasource_ppid,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_rpname
    snoopy_datasource_rpname,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_sid
    snoopy_datasource_sid,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_version
    snoopy_datasource_snoopy_version,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tty
    snoopy_datasource_tty,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tty_uid
    snoopy_datasource_tty_uid,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tty_username
    snoopy_datasource_tty_username,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_uid
    snoopy_datasource_uid,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_username
    snoopy_datasource_username,
#endif
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
