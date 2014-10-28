/*
 * SNOOPY LOGGER
 *
 * File: configuration.c
 *
 * Copyright (c) 2014 bostjan@a2o.si
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
 * Include all required C resources
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>



/*
 * Include all snoopy-related resources
 */
#include "snoopy.h"
#include "configuration.h"



/*
 * Include iniparser-related resources
 */
#include "lib/iniparser/src/iniparser.h"



/*
 * Storage of snoopy configuration, with default values
 */
snoopy_configuration_type   snoopy_configuration = {
    .initialized             = SNOOPY_TRUE,

    .config_file_enabled     = SNOOPY_FALSE,
    .config_file_path        = "",
    .config_file_parsed      = SNOOPY_FALSE,

#ifdef SNOOPY_ERROR_LOGGING_ENABLED
    .error_logging_enabled   = SNOOPY_TRUE,
#else
    .error_logging_enabled   = SNOOPY_FALSE,
#endif

    .message_format          = SNOOPY_LOG_MESSAGE_FORMAT,
    .message_format_malloced = SNOOPY_FALSE,

#ifdef SNOOPY_FILTER_ENABLED
    .filter_enabled          = SNOOPY_TRUE,
#else
    .filter_enabled          = SNOOPY_FALSE,
#endif
    .filter_chain            = SNOOPY_FILTER_CHAIN,
    .filter_chain_malloced   = SNOOPY_FALSE,
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
#ifdef SNOOPY_CONFIG_FILE
    snoopy_configuration_load_file(SNOOPY_CONFIG_FILE);
#endif
}



/*
 * snoopy_configuration_dtor
 *
 * Description:
 *     Frees all configuration-related malloced resources.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_dtor ()
{
    if (SNOOPY_TRUE == snoopy_configuration.message_format_malloced) {
        free(snoopy_configuration.message_format);
    }
    if (SNOOPY_TRUE == snoopy_configuration.filter_chain_malloced) {
        free(snoopy_configuration.filter_chain);
    }
}



/*
 * snoopy_configuration_load_file
 *
 * Description:
 *     Parses INI configuration file and overrides snoopy
 *     configuration with changed values.
 *
 * Params:
 *     file   Path log INI configuration file
 *
 * Return:
 *     int    0 on success, -1 on error openinf file, other int for other errors
 */
int snoopy_configuration_load_file (
    char *iniFilePath
) {
    dictionary *ini ;
    char       *confValString;   // Temporary query result space
    int         confValInt;      // Temporary query result space

    /* Tell snoopy we are using configuration file */
    snoopy_configuration.config_file_enabled = SNOOPY_TRUE;
    snoopy_configuration.config_file_path    = iniFilePath;

    /* Parse the INI configuration file first */
    ini = iniparser_load(iniFilePath);
    if (NULL == ini) {
        // TODO snoopy error handling
        return -1;
    }


    /* Pick out snoopy configuration variables */
    confValInt = iniparser_getboolean(ini, "snoopy:error_logging", -1);
    if (-1 != confValInt) {
        snoopy_configuration.error_logging_enabled = confValInt;
    }

    confValString = iniparser_getstring(ini, "snoopy:message_format", NULL);
    if (NULL != confValString) {
        snoopy_configuration.message_format          = strdup(confValString);
        snoopy_configuration.message_format_malloced = SNOOPY_TRUE;
    }

    confValString = iniparser_getstring(ini, "snoopy:filter_chain", NULL);
    if (NULL != confValString) {
        snoopy_configuration.filter_chain          = strdup(confValString);
        snoopy_configuration.filter_chain_malloced = SNOOPY_TRUE;
    }


    /* Housekeeping */
    snoopy_configuration.config_file_parsed = SNOOPY_TRUE;   // We have sucessfully parsed configuration file
    iniparser_freedict(ini);
    return 0;
}
