/*
 * SNOOPY LOGGER
 *
 * File: snoopy/output/devlog.c
 *
 * Copyright (c) 2015 Bostjan Skufca (bostjan _A_T_ a2o.si)
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
 * SNOOPY OUTPUT: socketouput (called like this because <socket.h> is system library
 *
 * Description:
 *     Sends given message to socket
 *
 * Params:
 *     message: message to send
 *
 * Return:
 *     void
 */
#include <stdio.h>
#include <stdlib.h>

#include "snoopy.h"
#include "configuration.h"
#include "outputregistry.h"



int snoopy_output_devlog (char *logMessage, int errorOrMessage)
{
    char  *logMessageWithPrefix = NULL;

    /* Generate final message - add prefix which is otherwise added by syslog() syscall */
    logMessageWithPrefix    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE + 100);   // +100 ought to be enough
    logMessageWithPrefix[0] = '\0';
    sprintf(logMessageWithPrefix, "snoopy[%d]: %s", getpid(), logMessage);

    /* Pass execution to another output provider */
    snoopy_configuration.output_provider = SNOOPY_OUTPUT_PROVIDER_SOCKET;
    snoopy_configuration.output_path     = "/dev/log";
    snoopy_outputregistry_dispatch(logMessageWithPrefix, errorOrMessage);

    free(logMessageWithPrefix);
    return 1;
}
