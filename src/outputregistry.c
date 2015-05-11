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
 * Include relevant header files
 */
#include "snoopy.h"
#include "outputregistry.h"
#include "configuration.h"
#include <string.h>



/*
 * Include headers of all output functions
 *
 * Please maintain alphabetical order, equal to what `ls` would do.
 */
//#ifdef SNOOPY_CONF_OUTPUT_ENABLED_console
//#include "output/consoleoutput.h"
//#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devlog
#include "output/devlogoutput.h"
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_file
#include "output/fileoutput.h"
#endif
//#ifdef SNOOPY_CONF_OUTPUT_ENABLED_journald
//#include "output/journaldoutput.h"
//#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_socket
#include "output/socketoutput.h"
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_syslog
#include "output/syslogoutput.h"
#endif



/*
 * Two arrays holding data about output functions
 *
 * Please maintain alphabetical order, equal to what `ls` would do.
 */
char *snoopy_outputregistry_names[] = {
//#ifdef SNOOPY_CONF_OUTPUT_ENABLED_console
//    "console",
//#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devlog
    "devlog",
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_file
    "file",
#endif
//#ifdef SNOOPY_CONF_OUTPUT_ENABLED_journald
//    "journald",
//#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_socket
    "socket",
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_syslog
    "syslog",
#endif
    "",
};

int (*snoopy_outputregistry_ptrs []) (char *logMessage, int errorOrMessage, char *arg) = {
//#ifdef SNOOPY_CONF_OUTPUT_ENABLED_console
//    snoopy_output_consoleoutput,
//#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devlog
    snoopy_output_devlogoutput,
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_file
    snoopy_output_fileoutput,
#endif
//#ifdef SNOOPY_CONF_OUTPUT_ENABLED_journald
//    snoopy_output_journaldoutput,
//#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_socket
    snoopy_output_socketoutput,
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_syslog
    snoopy_output_syslogoutput,
#endif
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
 * dispatch()
 *
 * Dispatch the message to configured outputProvider
 */
int snoopy_outputregistry_dispatch (char *logMessage, int errorOrMessage)
{
    int idx;

    idx = snoopy_outputregistry_getIndex(snoopy_configuration.output);
    if (idx == -1) {
        return -1;
    }

    return snoopy_outputregistry_ptrs[idx](logMessage, errorOrMessage, snoopy_configuration.output_arg);
}
