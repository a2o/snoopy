/*
 * SNOOPY LOGGER
 *
 * File: outputregistry.c
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
 * Include all output functions
 */
#include "snoopy.h"
#include "outputregistry.h"
#include "configuration.h"
#include <string.h>



/*
 * Two arrays holding data about output functions
 */
char *snoopy_outputregistry_names[] = {
//    "console",
    "devlog",
//    "file",
//    "journald",
    "socket",
    "syslog",
    "",
};

int (*snoopy_outputregistry_ptrs []) (char *logMessage, int errorOrMessage) = {
//    snoopy_output_consoleoutput,
    snoopy_output_devlog,
//    snoopy_output_fileoutput,
//    snoopy_output_journaldoutput,
    snoopy_output_socketoutput,
    snoopy_output_syslogoutput,
};



/*
 * isRegistered()
 *
 * Return true if output provider exists, otherwise return false
 */
int snoopy_outputregistry_isRegistered (char *providerName)
{
    if (snoopy_outputregistry_getIndex(providerName) == -1) {
        return SNOOPY_FALSE;
    } else {
        return SNOOPY_TRUE;
    }
}



/*
 * getIndex()
 *
 * Return index of given output provider, or -1 if not found 
 */
int snoopy_outputregistry_getIndex (char *providerName)
{
    int i;

    i = 0;
    while (strcmp(snoopy_outputregistry_names[i], "") != 0) {
        if (strcmp(snoopy_outputregistry_names[i], providerName) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}



/*
 * call()
 *
 * Call the given ouputProvider
 */
int snoopy_outputregistry_call (char *providerName, char *logMessage, int errorOrMessage)
{
    int idx;

    idx = snoopy_outputregistry_getIndex(providerName);
    if (idx == -1) {
        return -1;
    }

    return snoopy_outputregistry_ptrs[idx](logMessage, errorOrMessage);
}



/*
 * dispatch()
 *
 * Dispatch the message to configured outputProvider
 */
int snoopy_outputregistry_dispatch (char *logMessage, int errorOrMessage)
{
    int idx;

    idx = snoopy_outputregistry_getIndex(snoopy_configuration.output_provider);
    if (idx == -1) {
        return -1;
    }

    return snoopy_outputregistry_ptrs[idx](logMessage, errorOrMessage);
}
