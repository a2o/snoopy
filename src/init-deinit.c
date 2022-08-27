/*
 * SNOOPY COMMAND LOGGER
 *
 * File: misc.c
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
#include "init-deinit.h"

#include "snoopy.h"
#include "configuration.h"
#include "error.h"
#include "inputdatastorage.h"
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
#include "tsrm.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>



/*
 * snoopy_init
 *
 * Description:
 *     Handles Snoopy initialization/startup specifics.
 *     This method must be called when initializing anything that is
 *     Snoopy-based. This is especially true for thread-safe Snoopy builds.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_init ()
{
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
    snoopy_tsrm_ctor();
#endif
    snoopy_configuration_ctor();
    snoopy_inputdatastorage_ctor();
}



/*
 * snoopy_cleanup (deinit)
 *
 * Description:
 *     Handles all Snoopy deinitialization/shutdown specifics
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_cleanup ()
{
    /* Reverse order from ctor */
    snoopy_inputdatastorage_dtor();
    snoopy_configuration_dtor();
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
    snoopy_tsrm_dtor();
#endif
}
