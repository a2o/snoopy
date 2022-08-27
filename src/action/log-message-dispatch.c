/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2014 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "log-message-dispatch.h"

#include "snoopy.h"
#include "outputregistry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * snoopy_action_log_message_dispatch
 *
 * Description:
 *     Dispatch given message to configured output
 *
 * Params:
 *     logMessage:       message to dispatch
 *
 * Return:
 *     int:              See snoopy.h (SNOOPY_OUTPUT_*) for details.
 */
int snoopy_action_log_message_dispatch (const char *logMessage)
{
    /* Dispatch only if non-zero size */
    if (0 == strlen(logMessage)) {
        return SNOOPY_OUTPUT_GRACEFUL_DISCARD;
    }

    // Dispatch to configured output
    return snoopy_outputregistry_dispatch(logMessage);
}
