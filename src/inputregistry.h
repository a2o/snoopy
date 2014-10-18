/*
 * SNOOPY LOGGER
 *
 * File: inputregistry.h
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
 * Include headers of all input functions
 */
#include "input/cmdline.h"
#include "input/cwd.h"
#include "input/egid.h"
#include "input/egroup.h"
#include "input/env.h"
#include "input/euid.h"
#include "input/eusername.h"
#include "input/filename.h"
#include "input/gid.h"
#include "input/group.h"
#include "input/login.h"
#include "input/sid.h"
#include "input/tty.h"
#include "input/tty_uid.h"
#include "input/tty_username.h"
#include "input/uid.h"
#include "input/username.h"



/*
 * Two arrays holding data about input functions
 */
extern char *snoopy_inputregistry_names[];
extern int (*snoopy_inputregistry_ptrs []) (char *input, char *arg);



/*
 * Functions to manage and utilise input providers
 */
int snoopy_inputregistry_call         (char *providerName, char *returnMessage, char *providerArg);
int snoopy_inputregistry_getIndex     (char *providerName);
int snoopy_inputregistry_isRegistered (char *providerName);
