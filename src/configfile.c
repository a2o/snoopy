/*
 * SNOOPY LOGGER
 *
 * File: configfile.c
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
#include "configfile.h"

#include "snoopy.h"
#include "configfile.h"
#include "configuration.h"
#include "misc.h"
#include "outputregistry.h"

#include "lib/iniparser/src/iniparser.h"

#ifndef   _XOPEN_SOURCE   // For strdup
#define   _XOPEN_SOURCE   500
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>



/*
 * snoopy_configfile_load_file
 *
 * Description:
 *     Parses INI configuration file and overrides Snoopy
 *     configuration with changed values.
 *
 * Params:
 *     file   Path log INI configuration file
 *
 * Return:
 *     int    0 on success, -1 on error openinf file, other int for other errors
 */
int snoopy_configfile_load (
    char *iniFilePath
) {
    dictionary *ini ;
    const char *confValString;   // Temporary query result space
    int         confValInt;      // Temporary query result space
    snoopy_configuration_t *CFG;


    /* Get config pointer */
    CFG = snoopy_configuration_get();


    /* Tell Snoopy we are using configuration file */
    CFG->configfile_path = iniFilePath;

    /* Parse the INI configuration file first */
    ini = iniparser_load(iniFilePath);
    if (NULL == ini) {
        // TODO Snoopy error handling
        return -1;
    }
    CFG->configfile_found = SNOOPY_TRUE;


    /* Pick out Snoopy configuration variables */
    confValInt = iniparser_getboolean(ini, "snoopy:error_logging", -1);
    if (-1 != confValInt) {
        CFG->error_logging_enabled = confValInt;
    }

    confValString = iniparser_getstring(ini, "snoopy:message_format", NULL);
    if (NULL != confValString) {
        CFG->message_format          = strdup(confValString);
        CFG->message_format_malloced = SNOOPY_TRUE;
    }

    confValString = iniparser_getstring(ini, "snoopy:filter_chain", NULL);
    if (NULL != confValString) {
        CFG->filter_chain          = strdup(confValString);
        CFG->filter_chain_malloced = SNOOPY_TRUE;
    }

    confValString = iniparser_getstring(ini, "snoopy:output", NULL);
    if (NULL != confValString) {
        snoopy_configfile_parse_output(confValString);
    }

    confValString = iniparser_getstring(ini, "snoopy:syslog_facility", NULL);
    if (NULL != confValString) {
        snoopy_configfile_parse_syslog_facility(confValString);
    }

    confValString = iniparser_getstring(ini, "snoopy:syslog_ident", NULL);
    if (NULL != confValString) {
        CFG->syslog_ident          = strdup(confValString);
        CFG->syslog_ident_malloced = SNOOPY_TRUE;
    }

    confValString = iniparser_getstring(ini, "snoopy:syslog_level", NULL);
    if (NULL != confValString) {
        snoopy_configfile_parse_syslog_level(confValString);
    }


    /* Housekeeping */
    CFG->configfile_parsed = SNOOPY_TRUE;   // We have sucessfully parsed configuration file
    iniparser_freedict(ini);
    return 0;
}



/*
 * snoopy_configfile_parse_output
 *
 * Description:
 *     Parses configuration setting syslog_output and
 *     sets appropriate internal configuration variable(s).
 *     Uses default setting if unknown value.
 *
 * Params:
 *     confVal   Value from configuration file
 *
 * Return:
 *     void
 */
void snoopy_configfile_parse_output (
    const char *confValOrig
) {
    char  *confVal;
    char  *outputName;
    char  *outputArg;
    int    outputArgFound = SNOOPY_FALSE;
    snoopy_configuration_t *CFG;


    /* Get config pointer */
    CFG = snoopy_configuration_get();


    // Do not assign null to it explicitly, as you get "Explicit null dereference" Coverity error.
    // If you do not assign it, Coverity complains with "Uninitialized pointer read".
    char  *saveptr1 = "";

    // First clone the config value, as it gets freed by iniparser
    confVal = strdup(confValOrig);

    // Check if configured value contains argument(s)
    if (NULL == strchr(confVal, ':')) {
        outputName = confVal;
        CFG->output_arg          = "";
        CFG->output_arg_malloced = SNOOPY_FALSE;
        outputArg  = "";
    } else {
        // Separate output name from its arguments
        outputName = strtok_r(confVal, ":", &saveptr1);
        outputArg  = strtok_r(NULL   , ":", &saveptr1);
        outputArgFound = SNOOPY_TRUE;

    }

    // Determine output name
    if (SNOOPY_TRUE == snoopy_outputregistry_doesNameExist(outputName)) {
        CFG->output          = strdup(outputName);
        CFG->output_malloced = SNOOPY_TRUE;

        if (SNOOPY_TRUE == outputArgFound) {
            // THINK What if conf.output_arg was set in previous call to this function,
            // and is already malloced? We need to detect that and free previous
            // allocation.
            CFG->output_arg          = strdup(outputArg);
            CFG->output_arg_malloced = SNOOPY_TRUE;
        }
    } else {
        CFG->output              = SNOOPY_OUTPUT_DEFAULT;
        CFG->output_malloced     = SNOOPY_FALSE;
        CFG->output_arg          = SNOOPY_OUTPUT_DEFAULT_ARG;
        CFG->output_arg_malloced = SNOOPY_FALSE;
    }

    // Housekeeping
    free(confVal);
}



/*
 * snoopy_configfile_parse_syslog_facility
 *
 * Description:
 *     Parses configuration setting syslog_facility and
 *     sets appropriate config variable.
 *     Uses default setting if unknown value.
 *
 * Params:
 *     confVal   Value from configuration file
 *
 * Return:
 *     void
 */
void snoopy_configfile_parse_syslog_facility (
    const char *confValOrig
) {
    char *confVal;
    char *confValCleaned;
    int   facilityInt;
    snoopy_configuration_t *CFG;


    /* Get config pointer */
    CFG = snoopy_configuration_get();

    // Duplicate the iniparser value, as we need to modify it
    confVal = strdup(confValOrig);

    // First cleanup the value
    confValCleaned = snoopy_configfile_syslog_value_cleanup(confVal);

    // Evaluate and set configuration flag
    facilityInt = snoopy_syslog_convert_facilityToInt(confValCleaned);
    if (-1 == facilityInt) {
        CFG->syslog_facility = SNOOPY_SYSLOG_FACILITY;
    } else {
        CFG->syslog_facility = facilityInt;
    }

    /* Housekeeping */
    free(confVal);
}



/*
 * snoopy_configfile_parse_syslog_level
 *
 * Description:
 *     Parses configuration setting syslog_level and
 *     sets appropriate config variable.
 *     Uses default setting if unknown value.
 *
 * Params:
 *     confVal   Value from configuration file
 *
 * Return:
 *     void
 */
void snoopy_configfile_parse_syslog_level (
    const char *confValOrig
) {
    char *confVal;
    char *confValCleaned;
    int   levelInt;
    snoopy_configuration_t *CFG;


    /* Get config pointer */
    CFG = snoopy_configuration_get();

    // Duplicate the iniparser value, as we need to modify it
    confVal = strdup(confValOrig);

    // First cleanup the value
    confValCleaned = snoopy_configfile_syslog_value_cleanup(confVal);

    // Evaluate and set configuration flag
    levelInt = snoopy_syslog_convert_levelToInt(confValCleaned);
    if (-1 == levelInt) {
        CFG->syslog_level = SNOOPY_SYSLOG_LEVEL;
    } else {
        CFG->syslog_level = levelInt;
    }

    /* Housekeeping */
    free(confVal);
}



/*
 * snoopy_configfile_syslog_value_cleanup
 *
 * Description:
 *     Convert existing string to upper case, and remove LOG_ prefix
 *
 * Params:
 *     confVal   Pointer to string to change and to be operated on
 *
 * Return:
 *     char *    Pointer to cleaned string (either the same as initial argument,
 *               or 4 characters advanced, to remove LOG_ prefix
 */
char *snoopy_configfile_syslog_value_cleanup (char *confVal)
{
    char *confValCleaned;

    // Initialize - just in case
    confValCleaned = confVal;

    // Convert to upper case
    snoopy_configfile_strtoupper(confVal);

    // Remove LOG_ prefix
    confValCleaned = snoopy_configfile_syslog_value_remove_prefix(confVal);

    return confValCleaned;
}



/*
 * snoopy_configfile_syslog_value_remove_prefix
 *
 * Description:
 *     Remove the LOG_ prefix, return pointer to new string (either equal
 *     or +4 chars advanced)
 *
 * Params:
 *     string   Pointer to string to remove LOG_ prefix
 *
 * Return:
 *     char *   Pointer to non LOG_ part of the string
 */
char *snoopy_configfile_syslog_value_remove_prefix (char *confVal)
{
    if (0 == strncmp(confVal, "LOG_", 4)) {
        return confVal+4;
    } else {
        return confVal;
    }
}



/*
 * snoopy_configfile_strtoupper
 *
 * Description:
 *     Convert existing string to upper case
 *
 * Params:
 *     string   Pointer to string to change and to be operated on
 *
 * Return:
 *     void
 */
void snoopy_configfile_strtoupper (char *s)
{
    while (*s) {
        if ((*s >= 'a' ) && (*s <= 'z')) {
            *s -= ('a'-'A');
        }
        s++;
    }
}
