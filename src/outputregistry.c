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
 * Includes order: from local to global
 */
#include "outputregistry.h"

#include "snoopy.h"
#include "configuration.h"
#include "genericregistry.h"

#include <string.h>



/*
 * Include headers of all output functions
 *
 * Please maintain alphabetical order, equal to what `ls` would do.
 */
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devlog
#include "output/devlogoutput.h"
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devnull
#include "output/devnulloutput.h"
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devtty
#include "output/devttyoutput.h"
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
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_stderr
#include "output/stderroutput.h"
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_stdout
#include "output/stdoutoutput.h"
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_syslog
#include "output/syslogoutput.h"
#endif

/* This prevents "ISO C forbids empty initializer braces" error */
#include "output/noopoutput.h"



/*
 * Two arrays holding data about output functions
 *
 * Please maintain alphabetical order, equal to what `ls` would do.
 */
char *snoopy_outputregistry_names[] = {
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devlog
    "devlog",
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devnull
    "devnull",
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devtty
    "devtty",
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
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_stderr
    "stderr",
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_stdout
    "stdout",
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_syslog
    "syslog",
#endif

    /* This prevents "ISO C forbids empty initializer braces" error */
    "noop",
    "",
};

int (*snoopy_outputregistry_ptrs []) (char const * const logMessage, int errorOrMessage, char const * const arg) = {
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devlog
    snoopy_output_devlogoutput,
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devnull
    snoopy_output_devnulloutput,
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_devtty
    snoopy_output_devttyoutput,
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
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_stderr
    snoopy_output_stderroutput,
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_stdout
    snoopy_output_stdoutoutput,
#endif
#ifdef SNOOPY_CONF_OUTPUT_ENABLED_syslog
    snoopy_output_syslogoutput,
#endif

    /* This prevents "ISO C forbids empty initializer braces" error */
    snoopy_output_noopoutput,
};



/*
 * getCount()
 *
 * Return number of available outputs
 */
int snoopy_outputregistry_getCount ()
{
    return snoopy_genericregistry_getCount(snoopy_outputregistry_names);
}



/*
 * doesIdExist()
 *
 * Return true if output exists (by id), otherwise return false
 */
int snoopy_outputregistry_doesIdExist (int outputId)
{
    return snoopy_genericregistry_doesIdExist(snoopy_outputregistry_names, outputId);
}



/*
 * doesNameExist()
 *
 * Return true if output exists (by name), otherwise return false
 */
int snoopy_outputregistry_doesNameExist (char const * const outputName)
{
    return snoopy_genericregistry_doesNameExist(snoopy_outputregistry_names, outputName);
}



/*
 * getIdFromName()
 *
 * Return index of given output, or -1 if not found
 */
int snoopy_outputregistry_getIdFromName (char const * const outputName)
{
    return snoopy_genericregistry_getIdFromName(snoopy_outputregistry_names, outputName);
}



/*
 * getName()
 *
 * Return name of given output, or NULL
 */
char* snoopy_outputregistry_getName (int outputId)
{
    return snoopy_genericregistry_getName(snoopy_outputregistry_names, outputId);
}



/*
 * callById()
 *
 * Call the given output by id and return its output
 */
int snoopy_outputregistry_callById (int outputId, char const * const logMessage, int errorOrMessage, char const * const outputArg)
{
    if (SNOOPY_FALSE == snoopy_outputregistry_doesIdExist(outputId)) {
        return -1;
    }

    return snoopy_outputregistry_ptrs[outputId](logMessage, errorOrMessage, outputArg);
}



/*
 * callByName()
 *
 * Call the given output by name and return its output
 */
int snoopy_outputregistry_callByName (char const * const outputName, char const * const logMessage, int errorOrMessage, char const * const outputArg)
{
    int outputId;

    outputId = snoopy_outputregistry_getIdFromName(outputName);
    if (outputId == -1) {
        return -1;
    }

    return snoopy_outputregistry_ptrs[outputId](logMessage, errorOrMessage, outputArg);
}



/*
 * dispatch()
 *
 * Dispatch the message to configured outputProvider
 */
int snoopy_outputregistry_dispatch (char const * const logMessage, int errorOrMessage)
{
    snoopy_configuration_t *CFG;

    /* Get config pointer */
    CFG = snoopy_configuration_get();

    /* Dispatch */
    return snoopy_outputregistry_callByName(CFG->output, logMessage, errorOrMessage, CFG->output_arg);
}
