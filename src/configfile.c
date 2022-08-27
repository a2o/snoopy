/*
 * SNOOPY COMMAND LOGGER
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
#include "configuration.h"
#include "outputregistry.h"
#include "util/string-snoopy.h"
#include "util/syslog-snoopy.h"

#include "lib/inih/src/ini.h"

#ifndef   _XOPEN_SOURCE   // For strdup
#define   _XOPEN_SOURCE   500
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>





/*
 * Local defines
 */
#define SNOOPY_CONFIGFILE_OPTION_NOT_SUPPORTED -1

#define SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS    1
#define SNOOPY_CONFIGFILE_PARSEVALUE_ERROR      0



/*
 * Define supported config options
 */
snoopy_configfile_option_t snoopy_configfile_optionRegistry[] = {
    { "error_logging",   { SNOOPY_CONFIGFILE_OPTION_TYPE_BOOL,   &snoopy_configfile_parseValue_error_logging,   &snoopy_configfile_getOptionValueAsString_error_logging   } },
#ifdef SNOOPY_FILTERING_ENABLED
    { "filter_chain",    { SNOOPY_CONFIGFILE_OPTION_TYPE_STRING, &snoopy_configfile_parseValue_filter_chain,    &snoopy_configfile_getOptionValueAsString_filter_chain    } },
#endif
    { "message_format",  { SNOOPY_CONFIGFILE_OPTION_TYPE_STRING, &snoopy_configfile_parseValue_message_format,  &snoopy_configfile_getOptionValueAsString_message_format  } },
    { "output",          { SNOOPY_CONFIGFILE_OPTION_TYPE_STRING, &snoopy_configfile_parseValue_output,          &snoopy_configfile_getOptionValueAsString_output          } },
    { "syslog_facility", { SNOOPY_CONFIGFILE_OPTION_TYPE_STRING, &snoopy_configfile_parseValue_syslog_facility, &snoopy_configfile_getOptionValueAsString_syslog_facility } },
    { "syslog_ident",    { SNOOPY_CONFIGFILE_OPTION_TYPE_STRING, &snoopy_configfile_parseValue_syslog_ident,    &snoopy_configfile_getOptionValueAsString_syslog_ident    } },
    { "syslog_level",    { SNOOPY_CONFIGFILE_OPTION_TYPE_STRING, &snoopy_configfile_parseValue_syslog_level,    &snoopy_configfile_getOptionValueAsString_syslog_level    } },
    { "",                { SNOOPY_CONFIGFILE_OPTION_TYPE_NONE,    NULL,                                          NULL                                               } },
};



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
    int         iniParseStatus;
    snoopy_configuration_t *CFG;


    /* Get config pointer */
    CFG = snoopy_configuration_get();


    /* Tell Snoopy we are using configuration file */
    CFG->configfile_path = iniFilePath;

    /* Parse the INI configuration file first */
    iniParseStatus = ini_parse(iniFilePath, snoopy_configfile_iniParser_callback, CFG);
    if (0 != iniParseStatus) {
        return -1;
    }
    CFG->configfile_found = SNOOPY_TRUE;


    /* Housekeeping */
    CFG->configfile_parsed = SNOOPY_TRUE;   // We have successfully parsed configuration file
    return 0;
}




/*
 * snoopy_configfile_parser_callback
 *
 * Description:
 *     Callback function for each found ini value in parsed config file.
 *
 * Params:
 *     ...
 *
 * Return:
 *     ...
 */
int snoopy_configfile_iniParser_callback (
    void* sth,
    const char* section,
    const char* name,
    const char* confValString
) {
    /* Qualify pointer? */
    snoopy_configuration_t* CFG = (snoopy_configuration_t*)sth;

    /* Skip unknown sections */
    if (0 != strcmp(section, "snoopy")) {
        return 1;
    }

    /* Handle a (supported) option */
    int optionId = snoopy_configfile_optionRegistry_getIdFromName(name);
    if (optionId != SNOOPY_CONFIGFILE_OPTION_NOT_SUPPORTED) {
        return snoopy_configfile_optionRegistry[optionId].data.valueParserPtr(confValString, CFG);
    }

    /* Why are we returning 1 instead of zero everywhere? */
    return 1;
}



/*
 * Parse 'error_logging' config option
 *
 * Params:
 *     confValString:   Value from configuration file
 *     CFG:             Snoopy configuration struct
 *
 * Return:
 *     int              SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS or
 *                      SNOOPY_CONFIGFILE_PARSEVALUE_ERROR
 */
int snoopy_configfile_parseValue_error_logging (
    const char *confValString,
    snoopy_configuration_t* CFG
) {
    int confValInt = snoopy_configfile_getboolean(confValString, -1);
    if (-1 != confValInt) {
        CFG->error_logging_enabled = confValInt;
    }

    return SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS;
}



char * snoopy_configfile_getOptionValueAsString_error_logging ()
{
    const snoopy_configuration_t * CFG = snoopy_configuration_get();

    if (CFG->error_logging_enabled == SNOOPY_TRUE) {
        return strdup("yes");
    } else {
        return strdup("no");
    }
}



#ifdef SNOOPY_FILTERING_ENABLED
/*
 * Parse 'filter_chain' config option
 *
 * Params:
 *     confValString:   Value from configuration file
 *     CFG:             Snoopy configuration struct
 *
 * Return:
 *     int              SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS or
 *                      SNOOPY_CONFIGFILE_PARSEVALUE_ERROR
 */
int snoopy_configfile_parseValue_filter_chain (
    const char *confValString,
    snoopy_configuration_t* CFG
) {
    CFG->filter_chain          = strdup(confValString);
    CFG->filter_chain_malloced = SNOOPY_TRUE;

    return SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS;
}



char * snoopy_configfile_getOptionValueAsString_filter_chain ()
{
    const snoopy_configuration_t * CFG = snoopy_configuration_get();

    return strdup(CFG->filter_chain);
}
#endif



/*
 * Parse 'message_format' config option
 *
 * Params:
 *     confValString:   Value from configuration file
 *     CFG:             Snoopy configuration struct
 *
 * Return:
 *     int              SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS or
 *                      SNOOPY_CONFIGFILE_PARSEVALUE_ERROR
 */
int snoopy_configfile_parseValue_message_format (
    const char *confValString,
    snoopy_configuration_t* CFG
) {
    CFG->message_format          = strdup(confValString);
    CFG->message_format_malloced = SNOOPY_TRUE;

    return SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS;
}



char * snoopy_configfile_getOptionValueAsString_message_format ()
{
    const snoopy_configuration_t * CFG = snoopy_configuration_get();

    return strdup(CFG->message_format);
}



/*
 * Parse 'output' config option
 *
 * Params:
 *     confValString:   Value from configuration file
 *     CFG:             Snoopy configuration struct
 *
 * Return:
 *     int              SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS or
 *                      SNOOPY_CONFIGFILE_PARSEVALUE_ERROR
 */
int snoopy_configfile_parseValue_output (
    const char *confValString,
    snoopy_configuration_t* CFG
) {
    char  *confVal;
    const char * outputName;
    const char * outputArg;
    int    outputArgFound = SNOOPY_FALSE;


    // Do not assign null to it explicitly, as you get "Explicit null dereference" Coverity error.
    // If you do not assign it, Coverity complains with "Uninitialized pointer read".
    char  *saveptr1 = "";

    // First clone the config value, as it gets freed by ini parsing library
    confVal = strdup(confValString);

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


    return SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS;
}



char * snoopy_configfile_getOptionValueAsString_output ()
{
    const snoopy_configuration_t * CFG = snoopy_configuration_get();
    char * outputString = NULL;

    if (0 == strcmp("", CFG->output_arg)) {
        outputString = strdup(CFG->output);

    } else {
        size_t outputStringBufSize = strlen(CFG->output) + 1 + strlen(CFG->output_arg) + 1; // First +1 for ':', second +1 for '\0'
        outputString = malloc(outputStringBufSize);

        snprintf(outputString, outputStringBufSize, "%s:%s", CFG->output, CFG->output_arg);
        outputString[outputStringBufSize-1] = '\0';
    }
    return outputString;
}



/*
 * Parse 'syslog_facility' config option
 *
 * Params:
 *     confValString:   Value from configuration file
 *     CFG:             Snoopy configuration struct
 *
 * Return:
 *     int              SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS or
 *                      SNOOPY_CONFIGFILE_PARSEVALUE_ERROR
 */
int snoopy_configfile_parseValue_syslog_facility (
    const char *confValString,
    snoopy_configuration_t* CFG
) {
    char *confVal;
    const char *confValCleaned;
    int   facilityInt;


    // Duplicate the ini value, as we need to modify it
    confVal = strdup(confValString);

    // First cleanup the value
    confValCleaned = snoopy_configfile_syslog_value_cleanup(confVal);

    // Evaluate and set configuration flag
    facilityInt = snoopy_util_syslog_convertFacilityToInt(confValCleaned);
    if (-1 == facilityInt) {
        CFG->syslog_facility = SNOOPY_SYSLOG_FACILITY;
    } else {
        CFG->syslog_facility = facilityInt;
    }

    /* Housekeeping */
    free(confVal);


    return SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS;
}



char * snoopy_configfile_getOptionValueAsString_syslog_facility ()
{
    const snoopy_configuration_t * CFG = snoopy_configuration_get();

    return strdup(snoopy_util_syslog_convertFacilityToStr(CFG->syslog_facility));
}



/*
 * Parse 'syslog_ident' config option
 *
 * Params:
 *     confValString:   Value from configuration file
 *     CFG:             Snoopy configuration struct
 *
 * Return:
 *     int              SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS or
 *                      SNOOPY_CONFIGFILE_PARSEVALUE_ERROR
 */
int snoopy_configfile_parseValue_syslog_ident (
    const char *confValString,
    snoopy_configuration_t* CFG
) {
    CFG->syslog_ident_format          = strdup(confValString);
    CFG->syslog_ident_format_malloced = SNOOPY_TRUE;

    return SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS;
}



char * snoopy_configfile_getOptionValueAsString_syslog_ident ()
{
    const snoopy_configuration_t * CFG = snoopy_configuration_get();

    return strdup(CFG->syslog_ident_format);
}



/*
 * Parse 'syslog_level' config option
 *
 * Params:
 *     confValString:   Value from configuration file
 *     CFG:             Snoopy configuration struct
 *
 * Return:
 *     int              SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS or
 *                      SNOOPY_CONFIGFILE_PARSEVALUE_ERROR
 */
int snoopy_configfile_parseValue_syslog_level (
    const char *confValString,
    snoopy_configuration_t* CFG
) {
    char *confVal;
    const char *confValCleaned;
    int   levelInt;


    // Duplicate the ini value, as we need to modify it
    confVal = strdup(confValString);

    // First cleanup the value
    confValCleaned = snoopy_configfile_syslog_value_cleanup(confVal);

    // Evaluate and set configuration flag
    levelInt = snoopy_util_syslog_convertLevelToInt(confValCleaned);
    if (-1 == levelInt) {
        CFG->syslog_level = SNOOPY_SYSLOG_LEVEL;
    } else {
        CFG->syslog_level = levelInt;
    }

    /* Housekeeping */
    free(confVal);


    return SNOOPY_CONFIGFILE_PARSEVALUE_SUCCESS;
}



char * snoopy_configfile_getOptionValueAsString_syslog_level ()
{
    const snoopy_configuration_t * CFG = snoopy_configuration_get();

    return strdup(snoopy_util_syslog_convertLevelToStr(CFG->syslog_level));
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

    // Convert to upper case
    snoopy_util_string_toUpper(confVal);

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



/*-------------------------------------------------------------------------*/
/**
  @origin   Literally copy-pasted from ndevilla's iniparser


  @brief    Get the string associated to a key, convert to a boolean
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   integer

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.

  A true boolean is found if one of the following is matched:

  - A string starting with 'y'
  - A string starting with 'Y'
  - A string starting with 't'
  - A string starting with 'T'
  - A string starting with '1'

  A false boolean is found if one of the following is matched:

  - A string starting with 'n'
  - A string starting with 'N'
  - A string starting with 'f'
  - A string starting with 'F'
  - A string starting with '0'

  The notfound value returned if no boolean is identified, does not
  necessarily have to be 0 or 1.
 */
/*--------------------------------------------------------------------------*/
int snoopy_configfile_getboolean (const char *c, int notfound)
{
    int   ret;

    if (c[0]=='y' || c[0]=='Y' || c[0]=='1' || c[0]=='t' || c[0]=='T') {
        ret = SNOOPY_TRUE;
    } else if (c[0]=='n' || c[0]=='N' || c[0]=='0' || c[0]=='f' || c[0]=='F') {
        ret = SNOOPY_FALSE;
    } else {
        ret = notfound ;
    }
    return ret;
}



/*
 * optionRegistry :: getIdFromName()
 *
 * Return:
 *      int:    Id of a given option, or
 *              SNOOPY_CONFIGFILE_OPTION_NOT_SUPPORTED (-1) when not found.
 */
int snoopy_configfile_optionRegistry_getIdFromName (char const * const optionName)
{
    for (int i=0 ; 0 != strcmp(snoopy_configfile_optionRegistry[i].name, "") ; i++) {
        if (strcmp(snoopy_configfile_optionRegistry[i].name, optionName) == 0) {
            return i;
        }
    }

    /* Not found */
    return SNOOPY_CONFIGFILE_OPTION_NOT_SUPPORTED;
}



/*
 * optionRegistry :: getAll()
 *
 * Returns all supported config options.
 */
__attribute__((visibility("default"))) snoopy_configfile_option_t * snoopy_configfile_optionRegistry_getAll ()
{
    return snoopy_configfile_optionRegistry;
}



/*
 * optionRegistry :: getOptionValueAsString()
 *
 * Returns a malloc()-ed string representation of option value (that can be (re)used in a config file).
 */
__attribute__((visibility("default"))) char * snoopy_configfile_optionRegistry_getOptionValueAsString (char const * const optionName)
{

    for (int i=0 ; 0 != strcmp(snoopy_configfile_optionRegistry[i].name, "") ; i++) {
        if (strcmp(snoopy_configfile_optionRegistry[i].name, optionName) == 0) {
            return snoopy_configfile_optionRegistry[i].data.getValueAsStringPtr();
        }
    }
    return NULL; // Option not found
}
