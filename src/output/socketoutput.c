/*
 * SNOOPY LOGGER
 *
 * File: snoopy/output/socketoutput.c
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
#include <string.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "snoopy.h"
#include "configuration.h"



int snoopy_output_socketoutput (char *logMessage, int errorOrMessage)
{
    int                  s;
    struct sockaddr_un   remote;
    int                  remoteLength;

    /* Dispatch only if non-zero size */
    if (0 == strlen(logMessage)) {
        return 0;
    }


    /* Prepare socket - NON BLOCKING (systemd blocks /dev/log if journald is not running) */
    if ((s = socket(AF_LOCAL, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0)) == -1) {
        return -1;
    }

    remote.sun_family = AF_LOCAL;
    strcpy(remote.sun_path, snoopy_configuration.output_path);
    remoteLength      = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, remoteLength) == -1) {
        return -2;
    }


    /* Send message - returns -1 on error, chars sent on success */
    if (send(s, logMessage, strlen(logMessage), MSG_DONTWAIT|MSG_NOSIGNAL) == -1) {
        return -3;
    }


    /* Return success status */
    close(s);
    return 1;
}
