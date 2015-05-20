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
#ifdef SNOOPY_CONFIG_FILE_ENABLED
#include "configfile.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>



/*
 * Storage of snoopy configuration, with default values
 */
snoopy_configuration_type   snoopy_configuration = {
    .initialized             = SNOOPY_TRUE,

#ifdef SNOOPY_CONFIG_FILE_ENABLED
    .config_file_enabled     = SNOOPY_TRUE,
    .config_file_path        = SNOOPY_CONFIG_FILE_PATH,
#else
    .config_file_enabled     = SNOOPY_FALSE,
    .config_file_path        = "",
#endif
    .config_file_found       = SNOOPY_FALSE,
    .config_file_parsed      = SNOOPY_FALSE,

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

    .syslog_facility         = SNOOPY_CONF_SYSLOG_FACILITY,
    .syslog_level            = SNOOPY_CONF_SYSLOG_LEVEL,
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
    /* Parse INI file if enabled */
#ifdef SNOOPY_CONFIG_FILE_ENABLED
    snoopy_configfile_load(snoopy_configuration.config_file_path);
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
    /*
     * Reset config setting: message_format
     */
    if (SNOOPY_TRUE == snoopy_configuration.message_format_malloced) {
        free(snoopy_configuration.message_format);

        /*
         * Set this to false - REQUIRED
         *
         * This needs to be done as a special condition can occur at boot/shutdown:
         * - snoopy is loaded when snoopy.ini is visible (mounted, present)
         * - snoopy parses it, and sets message_format and ..._malloced to TRUE
         * - on shutdown, snoopy.ini might disappear
         * - snoopy_configuration_ctor() tries to parse config file, and as it is
         *     not found, it does no alteration of snoopy_configuraton struct
         * - snoopy_configuration.message_format_malloced is left set to TRUE
         * - when snoopy_configuration_dtor() is called, it tries to free the
         *     const char[] that contains the compiled-in message format
         */
        snoopy_configuration.message_format_malloced = SNOOPY_FALSE;

        /*
         * Set this to default value - REQUIRED
         *
         * Otherwise on next Snoopy run there will be no message format defined,
         * which would in best-case scenario cause no snoopy output, but in
         * worst-case scenarion there would be a segfault and possible system
         * crash.
         */
        snoopy_configuration.message_format = SNOOPY_MESSAGE_FORMAT;
    }


    /*
     * Reset config setting: filter_chain
     */
    if (SNOOPY_TRUE == snoopy_configuration.filter_chain_malloced) {
        free(snoopy_configuration.filter_chain);

        /* Set this to false - REQUIRED (see above) */
        snoopy_configuration.filter_chain_malloced = SNOOPY_FALSE;

        /* Set this to default value - REQUIRED (see above) */
#ifdef SNOOPY_FILTERING_ENABLED
        snoopy_configuration.filter_chain = SNOOPY_FILTER_CHAIN;
#else
        snoopy_configuration.filter_chain = "";
#endif
    }


    /*
     * Reset config setting: output
     */
    if (SNOOPY_TRUE == snoopy_configuration.output_malloced) {
        free(snoopy_configuration.output);

        /* Set this to false - REQUIRED (see above) */
        snoopy_configuration.output_malloced = SNOOPY_FALSE;

        /* Set this to default value - REQUIRED (see above) */
        snoopy_configuration.output = SNOOPY_OUTPUT_DEFAULT;
    }


    /*
     * Reset config setting: output_arg
     */
    if (SNOOPY_TRUE == snoopy_configuration.output_arg_malloced) {
        free(snoopy_configuration.output_arg);

        /* Set this to false - REQUIRED (see above) */
        snoopy_configuration.output_arg_malloced = SNOOPY_FALSE;

        /* Set this to default value - REQUIRED (see above) */
        snoopy_configuration.output_arg = SNOOPY_OUTPUT_DEFAULT_ARG;
    }
}
