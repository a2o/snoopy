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
#include "datasource/cmdline.h"
#include "datasource/cwd.h"
#include "datasource/egid.h"
#include "datasource/egroup.h"
#include "datasource/env.h"
#include "datasource/env_all.h"
#include "datasource/euid.h"
#include "datasource/eusername.h"
#include "datasource/filename.h"
#include "datasource/gid.h"
#include "datasource/group.h"
#include "datasource/login.h"
#include "datasource/pid.h"
#include "datasource/ppid.h"
#include "datasource/rpname.h"
#include "datasource/sid.h"
#include "datasource/snoopy_version.h"
#include "datasource/tty.h"
#include "datasource/tty_uid.h"
#include "datasource/tty_username.h"
#include "datasource/uid.h"
#include "datasource/username.h"



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
