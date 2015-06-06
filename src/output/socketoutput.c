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
 * Includes order: from local to global
 */
#include "socketoutput.h"

#include "snoopy.h"
#include "configuration.h"

#include <features.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>



/*
 * SNOOPY OUTPUT: socketoutput (called like this because <socket.h> is system library)
 *
 * Description:
 *     Sends given message to socket
 *
 * Params:
 *     message:    message to send
 *     errOrMsg:   is message and error message or ordinary Snoopy log message
 *     arg:        output argument(s)
 *
 * Return:
 *     int:        See snoopy.h (SNOOPY_OUTPUT_*) for details.
 */
int snoopy_output_socketoutput (char const * const logMessage, int errorOrMessage, char const * const arg)
{
    int                  s;
    struct sockaddr_un   remote;
    int                  remoteLength;

    /* Dispatch only if non-zero size */
    if (0 == strlen(logMessage)) {
        return SNOOPY_OUTPUT_GRACEFUL_DISCARD;
    }

#if (defined(__GLIBC__) && (2 == __GLIBC__) && (__GLIBC_MINOR__ < 9))
    /* Prepare socket - non-blocking sockets are not supported on older glibc */
    if ((s = socket(AF_LOCAL, SOCK_DGRAM, 0)) == -1) {
#else
    /* Prepare socket - NON BLOCKING (systemd blocks /dev/log if journald is not running) */
    if ((s = socket(AF_LOCAL, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0)) == -1) {
#endif
        return SNOOPY_OUTPUT_FAILURE;
    }

    remote.sun_family = AF_LOCAL;
    strncpy(remote.sun_path, arg, 107);   // Coverity suggests -1 here
    if (strlen(arg) > 107) {
        remote.sun_path[107] = '\0';
    }

    remoteLength      = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, remoteLength) == -1) {
        close(s);
        return SNOOPY_OUTPUT_FAILURE;
    }


    /* Send message - returns -1 on error, chars sent on success */
    if (send(s, logMessage, strlen(logMessage), MSG_DONTWAIT|MSG_NOSIGNAL) == -1) {
        close(s);
        return SNOOPY_OUTPUT_FAILURE;
    }


    /* Return success status */
    close(s);
    return strlen(logMessage);
}
