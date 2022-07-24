/*
 * SNOOPY LOGGER
 *
 * File: snoopy/output/syslogoutput.c
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
#include "syslogoutput.h"

#include "snoopy.h"
#include "configuration.h"
#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>



/*
 * SNOOPY OUTPUT: syslogouput (called like this because <syslog.h> is system library
 *
 * Description:
 *     Sends given message to syslog
 *
 * Params:
 *     message:    message to send
 *     errOrMsg:   is message and error message or ordinary Snoopy log message
 *     arg:        output argument(s)
 *
 * Return:
 *     int:        See snoopy.h (SNOOPY_OUTPUT_*) for details.
 */
int snoopy_output_syslogoutput (char const * const logMessage, int errorOrMessage, char const * const arg)
{
    /* Dispatch only if non-zero size */
    if (0 == strlen(logMessage)) {
        return SNOOPY_OUTPUT_GRACEFUL_DISCARD;
    }

    /* Get config pointer */
    snoopy_configuration_t *CFG = snoopy_configuration_get();

    /* Generate syslog ident string */
    char syslogIdent[SNOOPY_SYSLOG_IDENT_FORMAT_BUF_SIZE] = {'\0'};
    snoopy_message_generateFromFormat(syslogIdent, SNOOPY_SYSLOG_IDENT_FORMAT_BUF_SIZE, CFG->syslog_ident_format);

    /* Prepare logging stuff */
    openlog(syslogIdent, LOG_PID, CFG->syslog_facility);

    /* Log error or ordinary message */
    if (SNOOPY_LOG_ERROR == errorOrMessage) {
        syslog(LOG_ERR, "ERROR: %s", logMessage);
    } else {
        syslog(CFG->syslog_level, "%s", logMessage);
    }

    /* Close the syslog file descriptor */
    closelog();

    return (int) strlen(logMessage);
}
