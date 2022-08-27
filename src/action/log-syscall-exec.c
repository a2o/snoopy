/*
 * SNOOPY COMMAND LOGGER
 *
 * File: log.c
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
#include "log-syscall-exec.h"

#include "log-message-dispatch.h"

#include "snoopy.h"
#include "configuration.h"
#if defined(SNOOPY_FILTERING_ENABLED)
#include "filtering.h"
#endif
#include "message.h"
#include "outputregistry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * snoopy_action_log_syscall_exec
 *
 * Description:
 *     Common routine that does execv(e)() logging
 *
 * Return:
 *     void
 */
void snoopy_action_log_syscall_exec ()
{
    const snoopy_configuration_t * CFG;
    char *logMessage = NULL;

    /* Get config pointer */
    CFG = snoopy_configuration_get();

#if defined(SNOOPY_FILTERING_ENABLED)
    /* Should message be passed to syslog or not? */
    if (
        (SNOOPY_TRUE == CFG->filtering_enabled)
        &&
        (SNOOPY_FILTER_DROP == snoopy_filtering_check_chain(CFG->filter_chain))
    ) {
        return;
    }
#endif

    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_BUF_SIZE);
    logMessage[0] = '\0';

    /* Generate log message in specified format */
    snoopy_message_generateFromFormat(logMessage, SNOOPY_LOG_MESSAGE_BUF_SIZE, CFG->message_format);

    /* Dispatch the message to configured output */
    snoopy_action_log_message_dispatch(logMessage);

    /* Housekeeping */
    free(logMessage);
}
