/*
 * SNOOPY LOGGER
 *
 * File: datasourceregistry.h
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
 * Include headers of all datasource functions
 */
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cmdline
#include "datasource/cmdline.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_cwd
#include "datasource/cwd.h"
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
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_snoopy_version
#include "datasource/snoopy_version.h"
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



/*
 * Two arrays holding data about datasource functions
 */
extern char *snoopy_datasourceregistry_names[];
extern int (*snoopy_datasourceregistry_ptrs []) (char *datasource, char *arg);



/*
 * Functions to manage and utilise datasource providers
 */
int snoopy_datasourceregistry_call         (char *providerName, char *returnMessage, char *providerArg);
int snoopy_datasourceregistry_getIndex     (char *providerName);
int snoopy_datasourceregistry_isRegistered (char *providerName);
