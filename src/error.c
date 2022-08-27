/*
 * SNOOPY COMMAND LOGGER
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
 * Includes order: from local to global
 */
#include "error.h"

#include "snoopy.h"
#include "configuration.h"
#include "action/log-message-dispatch.h"

#include <stdio.h>



/*
 * Local defines
 */
#define SNOOPY_ERROR_MSG_BUF_SIZE 4096



/*
 * snoopy_error_handler
 *
 * Description:
 *     Does the actual error handling. If error logging is enabled, the error
 *     message is sent to the configured output.
 *
 * Params:
 *     errorMsg:        Error message, withouth the leading "ERROR:" prefix.
 *
 * Return:
 *     void
 */
void snoopy_error_handler (char const * const errorMsg)
{
    const snoopy_configuration_t * CFG;
    char errorMsgFormatted[SNOOPY_ERROR_MSG_BUF_SIZE];
    errorMsgFormatted[0] = '\0';

    CFG = snoopy_configuration_get();

    if (SNOOPY_TRUE != CFG->error_logging_enabled) {
        return;
    }

    snprintf(errorMsgFormatted, SNOOPY_ERROR_MSG_BUF_SIZE, "SNOOPY ERROR: %s", errorMsg);
    errorMsgFormatted[SNOOPY_ERROR_MSG_BUF_SIZE-1] = '\0';

    snoopy_action_log_message_dispatch(errorMsg);
}
