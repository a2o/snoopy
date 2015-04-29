/*
 * SNOOPY LOGGER
 *
 * File: error.c
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
 * Include all required C resources
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>



/*
 * Include all snoopy-related resources
 */
#include "snoopy.h"
#include "error.h"
#include "configuration.h"
#include "log.h"



/*
 * snoopy_error_handler
 *
 * Description:
 *     Does the actual error handling. If configured, it sends it
 *     to syslog.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_error_handler (char *errorMsg)
{
    /* Only send error to syslog if configured like that */
    if (SNOOPY_TRUE == snoopy_configuration.error_logging_enabled) {
        snoopy_log_dispatch(errorMsg, SNOOPY_LOG_ERROR);
    }
}
