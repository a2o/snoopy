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
 * Includes order: from local to global
 */
#include "datasourceregistry.h"

#include "snoopy.h"
#include "genericregistry.h"

#include <string.h>



/*
 * Include headers of all datasource functions
 */
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cmdline
#include "datasource/cmdline.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cwd
#include "datasource/cwd.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_datetime
#include "datasource/datetime.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_domain
#include "datasource/domain.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_egid
#include "datasource/egid.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_egroup
#include "datasource/egroup.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_env
#include "datasource/env.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_env_all
#include "datasource/env_all.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_euid
#include "datasource/euid.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_eusername
#include "datasource/eusername.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_filename
#include "datasource/filename.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_gid
#include "datasource/gid.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_group
#include "datasource/group.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_hostname
#include "datasource/hostname.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_login
#include "datasource/login.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_pid
#include "datasource/pid.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_ppid
#include "datasource/ppid.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_rpname
#include "datasource/rpname.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_sid
#include "datasource/sid.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_literal
#include "datasource/snoopy_literal.h"
#endif
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_threads
#include "datasource/snoopy_threads.h"
#endif
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_version
#include "datasource/snoopy_version.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tid
#include "datasource/tid.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tid_kernel
#include "datasource/tid_kernel.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_timestamp
#include "datasource/timestamp.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_timestamp_ms
#include "datasource/timestamp_ms.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_timestamp_us
#include "datasource/timestamp_us.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tty
#include "datasource/tty.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tty_uid
#include "datasource/tty_uid.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tty_username
#include "datasource/tty_username.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_uid
#include "datasource/uid.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_username
#include "datasource/username.h"
#endif

/* This prevents "ISO C forbids empty initializer braces" error */
#include "datasource/noop.h"



/*
 * Two arrays holding data about all data sources
 */
char* snoopy_datasourceregistry_names[] = {
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cmdline
    "cmdline",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cwd
    "cwd",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_datetime
    "datetime",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_domain
    "domain",
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
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_hostname
    "hostname",
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
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_literal
    "snoopy_literal",
#endif
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_threads
    "snoopy_threads",
#endif
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_version
    "snoopy_version",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tid
    "tid",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tid_kernel
    "tid_kernel",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_timestamp
    "timestamp",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_timestamp_ms
    "timestamp_ms",
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_timestamp_us
    "timestamp_us",
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

    /* This prevents "ISO C forbids empty initializer braces" error */
    "noop",
    "",
};

int (*snoopy_datasourceregistry_ptrs []) (char * const result, char const * const arg) = {
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cmdline
    snoopy_datasource_cmdline,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cwd
    snoopy_datasource_cwd,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_datetime
    snoopy_datasource_datetime,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_domain
    snoopy_datasource_domain,
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
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_hostname
    snoopy_datasource_hostname,
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
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_literal
    snoopy_datasource_snoopy_literal,
#endif
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_threads
    snoopy_datasource_snoopy_threads,
#endif
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_version
    snoopy_datasource_snoopy_version,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tid
    snoopy_datasource_tid,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_tid_kernel
    snoopy_datasource_tid_kernel,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_timestamp
    snoopy_datasource_timestamp,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_timestamp_ms
    snoopy_datasource_timestamp_ms,
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_timestamp_us
    snoopy_datasource_timestamp_us,
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

    /* This prevents "ISO C forbids empty initializer braces" error */
    snoopy_datasource_noop,
};



/*
 * getCount()
 *
 * Return number of available datasources
 */
int snoopy_datasourceregistry_getCount ()
{
    return snoopy_genericregistry_getCount(snoopy_datasourceregistry_names);
}



/*
 * doesIdExist()
 *
 * Return true if datasource exists (by id), otherwise return false
 */
int snoopy_datasourceregistry_doesIdExist (int datasourceId)
{
    return snoopy_genericregistry_doesIdExist(snoopy_datasourceregistry_names, datasourceId);
}



/*
 * doesNameExist()
 *
 * Return true if datasource exists (by name), otherwise return false
 */
int snoopy_datasourceregistry_doesNameExist (char const * const datasourceName)
{
    return snoopy_genericregistry_doesNameExist(snoopy_datasourceregistry_names, datasourceName);
}



/*
 * getIdFromName()
 *
 * Return index of given datasource, or -1 if not found
 */
int snoopy_datasourceregistry_getIdFromName (char const * const datasourceName)
{
    return snoopy_genericregistry_getIdFromName(snoopy_datasourceregistry_names, datasourceName);
}



/*
 * getName()
 *
 * Return name of given datasource, or NULL
 */
char* snoopy_datasourceregistry_getName (int datasourceId)
{
    return snoopy_genericregistry_getName(snoopy_datasourceregistry_names, datasourceId);
}



/*
 * callById()
 *
 * Call the given datasource by id and return its output
 */
int snoopy_datasourceregistry_callById (int datasourceId, char * const result, char const * const datasourceArg)
{
    if (SNOOPY_FALSE == snoopy_datasourceregistry_doesIdExist(datasourceId)) {
        return -1;
    }

    return snoopy_datasourceregistry_ptrs[datasourceId](result, datasourceArg);
}



/*
 * callByName()
 *
 * Call the given datasource by name and return its output
 */
int snoopy_datasourceregistry_callByName (char const * const datasourceName, char * const result, char const * const datasourceArg)
{
    int datasourceId;

    datasourceId = snoopy_datasourceregistry_getIdFromName(datasourceName);
    if (datasourceId == -1) {
        return -1;
    }

    return snoopy_datasourceregistry_ptrs[datasourceId](result, datasourceArg);
}
