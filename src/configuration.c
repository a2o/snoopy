/*
 * SNOOPY LOGGER
 *
 * File: configuration.c
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
#include "configuration.h"

#include "snoopy.h"
#ifdef SNOOPY_CONFIGFILE_ENABLED
#include "configfile.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>



/*
 * Storage of Snoopy configuration, with default values
 */
snoopy_configuration_t   snoopy_configuration_data = {
    .initialized             = SNOOPY_TRUE,

#ifdef SNOOPY_CONFIGFILE_ENABLED
    .configfile_enabled     = SNOOPY_TRUE,
    .configfile_path        = SNOOPY_CONFIGFILE_PATH,
#else
    .configfile_enabled     = SNOOPY_FALSE,
    .configfile_path        = "",
#endif
    .configfile_found       = SNOOPY_FALSE,
    .configfile_parsed      = SNOOPY_FALSE,

#ifdef SNOOPY_ERROR_LOGGING_ENABLED
    .error_logging_enabled   = SNOOPY_TRUE,
#else
    .error_logging_enabled   = SNOOPY_FALSE,
#endif

    .message_format          = SNOOPY_MESSAGE_FORMAT,
    .message_format_malloced = SNOOPY_FALSE,

#ifdef SNOOPY_FILTERING_ENABLED
    .filtering_enabled       = SNOOPY_TRUE,
    .filter_chain            = SNOOPY_FILTER_CHAIN,
#else
    .filtering_enabled       = SNOOPY_FALSE,
    .filter_chain            = "",
#endif
    .filter_chain_malloced   = SNOOPY_FALSE,

    .output                  = SNOOPY_OUTPUT_DEFAULT,
    .output_malloced         = SNOOPY_FALSE,
    .output_arg              = SNOOPY_OUTPUT_DEFAULT_ARG,
    .output_arg_malloced     = SNOOPY_FALSE,

    .syslog_facility         = SNOOPY_SYSLOG_FACILITY,
    .syslog_ident            = SNOOPY_SYSLOG_IDENT,
    .syslog_ident_malloced   = SNOOPY_FALSE,
    .syslog_level            = SNOOPY_SYSLOG_LEVEL,
};



/*
 * snoopy_configuration_ctor
 *
 * Description:
 *     Populates snoopy_configuration config variable storage with
 *     correct values, either from configuration file (if enabled)
 *     or from ./configure arguments, or defaults are used as last
 *     case scenario.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_ctor ()
{
    snoopy_configuration_t *CFG;


    /* Get config pointer */
    CFG = snoopy_configuration_get();


    /* Parse INI file if enabled */
#ifdef SNOOPY_CONFIGFILE_ENABLED
    snoopy_configfile_load(CFG->configfile_path);
#endif
}



/*
 * snoopy_configuration_dtor
 *
 * Description:
 *     Frees all configuration-related malloced resources, and sets
 *     corresponding config settings back to their default values.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_dtor ()
{
    snoopy_configuration_t *CFG;


    /* Get config pointer */
    CFG = snoopy_configuration_get();


    /*
     * Reset config setting: configfile_path
     *
     * This might get changed by libsnoopy-test.so library.
     */
#ifdef SNOOPY_CONFIGFILE_ENABLED
    CFG->configfile_path = SNOOPY_CONFIGFILE_PATH;
#endif


    /*
     * Reset config setting: message_format
     */
    if (SNOOPY_TRUE == CFG->message_format_malloced) {
        free(CFG->message_format);

        /*
         * Set this to false - REQUIRED
         *
         * This needs to be done as a special condition can occur at boot/shutdown:
         * - Snoopy is loaded when snoopy.ini is visible (mounted, present)
         * - Snoopy parses it, and sets message_format and ..._malloced to TRUE
         * - on shutdown, snoopy.ini might disappear
         * - snoopy_configuration_ctor() tries to parse config file, and as it is
         *     not found, it does no alteration of snoopy_configuraton struct
         * - CFG->message_format_malloced is left set to TRUE
         * - when snoopy_configuration_dtor() is called, it tries to free the
         *     const char[] that contains the compiled-in message format
         */
        CFG->message_format_malloced = SNOOPY_FALSE;

        /*
         * Set this to default value - REQUIRED
         *
         * Otherwise on next Snoopy run there will be no message format defined,
         * which would in best-case scenario cause no Snoopy output, but in
         * worst-case scenarion there would be a segfault and possible system
         * crash.
         */
        CFG->message_format = SNOOPY_MESSAGE_FORMAT;
    }


    /*
     * Reset config setting: filter_chain
     */
    if (SNOOPY_TRUE == CFG->filter_chain_malloced) {
        free(CFG->filter_chain);

        /* Set this to false - REQUIRED (see above) */
        CFG->filter_chain_malloced = SNOOPY_FALSE;

        /* Set this to default value - REQUIRED (see above) */
#ifdef SNOOPY_FILTERING_ENABLED
        CFG->filter_chain = SNOOPY_FILTER_CHAIN;
#else
        CFG->filter_chain = "";
#endif
    }


    /*
     * Reset config setting: output
     */
    if (SNOOPY_TRUE == CFG->output_malloced) {
        free(CFG->output);

        /* Set this to false - REQUIRED (see above) */
        CFG->output_malloced = SNOOPY_FALSE;

        /* Set this to default value - REQUIRED (see above) */
        CFG->output = SNOOPY_OUTPUT_DEFAULT;
    }


    /*
     * Reset config setting: output_arg
     */
    if (SNOOPY_TRUE == CFG->output_arg_malloced) {
        free(CFG->output_arg);

        /* Set this to false - REQUIRED (see above) */
        CFG->output_arg_malloced = SNOOPY_FALSE;

        /* Set this to default value - REQUIRED (see above) */
        CFG->output_arg = SNOOPY_OUTPUT_DEFAULT_ARG;
    }


    /*
     * Reset config setting: syslog_ident
     */
    if (SNOOPY_TRUE == CFG->syslog_ident_malloced) {
        free(CFG->syslog_ident);
        CFG->syslog_ident_malloced = SNOOPY_FALSE;          /* Set this to false         - REQUIRED (see above) */
        CFG->syslog_ident          = SNOOPY_SYSLOG_IDENT;   /* Set this to default value - REQUIRED (see above) */
    }
}



/*
 * snoopy_configuration_set_configfile_path_from_env
 *
 * Description:
 *     Parses environment for SNOOPY_INI and if found, checks if
 *     file exists and is readable, and sets path to snoopy.ini
 *     accordingly.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_set_configfile_path_from_env ()
{
    snoopy_configuration_t *CFG;
    char *valuePtr;


    /* Get config pointer */
    CFG = snoopy_configuration_get();


    valuePtr = getenv("SNOOPY_INI");

    /* Does environmental variable exist? */
    if (NULL == valuePtr) {
        /* Nope. */
        return;
    }

    /* Is file readable? */
    if (0 != access(valuePtr, R_OK)) {
        /* Nope. */
        return;
    }

    /* Store it */
    /* FIXME does this have to be copied to malloced local variable? */
    CFG->configfile_path = valuePtr;
}



/*
 * snoopy_configuration_get()
 *
 * Description:
 *     Retrieve pointer configuration struct.
 *
 * Params:
 *     envp:   environment array pointer to store
 *
 * Return:
 *     void
 */
snoopy_configuration_t* snoopy_configuration_get ()
{
    return &snoopy_configuration_data;
}
