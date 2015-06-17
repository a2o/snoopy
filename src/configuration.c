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
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
#include "tsrm.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>




/*
 * Flag that enables/disables configuration file parsing
 *
 * This is a runtime flag, used by binaries in tests/bin/ directory.
 * ./configure flags are wrapped around this.
 */
int snoopy_configuration_configFileParsingEnabled = SNOOPY_TRUE;



/*
 * Should alternate configuration file be loaded?
 */
char * snoopy_configuration_altConfigFilePath = NULL;



/*
 * Storage of Snoopy configuration for non-thread-safe builds
 */
#ifndef SNOOPY_CONF_THREAD_SAFETY_ENABLED
snoopy_configuration_t   snoopy_configuration_data = {
    .initialized = SNOOPY_FALSE,
};
#endif



/*
 * snoopy_configuration_preinit_disableConfigFileParsing
 *
 * Description:
 *     Disables configuration file parsing at runtime.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_preinit_disableConfigFileParsing ()
{
    snoopy_configuration_configFileParsingEnabled = SNOOPY_FALSE;
}



/*
 * snoopy_configuration_preinit_enableConfigFileParsing
 *
 * Description:
 *     Enables configuration file parsing at runtime. Optionally sets
 *     alternative config file path.
 *
 * Params:
 *     altConfigFilePath:   path to alternate config file
 *
 * Return:
 *     void
 */
void snoopy_configuration_preinit_enableAltConfigFileParsing (char * const altConfigFilePath)
{
    if (NULL != altConfigFilePath) {
        snoopy_configuration_altConfigFilePath = altConfigFilePath;
    }
    snoopy_configuration_configFileParsingEnabled = SNOOPY_TRUE;
}



/*
 * snoopy_configuration_preinit_setConfigFilePathFromEnv
 *
 * Description:
 *     Parses environment for SNOOPY_INI and if found, checks if
 *     file exists and is readable, and sets path to snoopy.ini
 *     accordingly. Also it enables runtime config file parsing.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_preinit_setConfigFilePathFromEnv ()
{
    char *valuePtr;


    /* Does environmental variable exist? */
    valuePtr = getenv("SNOOPY_INI");
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
    snoopy_configuration_preinit_enableAltConfigFileParsing(valuePtr);
}



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
#ifdef SNOOPY_CONFIGFILE_ENABLED
    /* Is config file parsing disabled at runtime? */
    if (SNOOPY_FALSE == snoopy_configuration_configFileParsingEnabled) {
        return;
    }

    /* Get config pointer */
    snoopy_configuration_t *CFG = snoopy_configuration_get();

    /* Parse INI file if enabled */
    if (NULL != snoopy_configuration_altConfigFilePath) {
        // This is used by Snoopy testing suite - combined tests
        snoopy_configfile_load(snoopy_configuration_altConfigFilePath);
    } else {
        snoopy_configfile_load(CFG->configfile_path);
    }
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
 * snoopy_configuration_setUninitialized
 *
 * Description:
 *     Sets state of configuration array to uninitialized.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_setUninitialized
(
    snoopy_configuration_t *CFG
) {
    CFG->initialized = SNOOPY_FALSE;
}



/*
 * snoopy_configuration_setDefaults
 *
 * Description:
 *     Sets the default values for all configuration variables.
 *     Defaults are primarily defined by Snoopy, and possibly overridden by
 *     ./configure flags.
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void snoopy_configuration_setDefaults
(
    snoopy_configuration_t *CFG
) {
    CFG->initialized             = SNOOPY_TRUE;

#ifdef SNOOPY_CONFIGFILE_ENABLED
    CFG->configfile_enabled      = SNOOPY_TRUE;
    CFG->configfile_path         = SNOOPY_CONFIGFILE_PATH;
#else
    CFG->configfile_enabled      = SNOOPY_FALSE;
    CFG->configfile_path         = "";
#endif
    CFG->configfile_found        = SNOOPY_FALSE;
    CFG->configfile_parsed       = SNOOPY_FALSE;

#ifdef SNOOPY_ERROR_LOGGING_ENABLED
    CFG->error_logging_enabled   = SNOOPY_TRUE;
#else
    CFG->error_logging_enabled   = SNOOPY_FALSE;
#endif

    CFG->message_format          = SNOOPY_MESSAGE_FORMAT;
    CFG->message_format_malloced = SNOOPY_FALSE;

#ifdef SNOOPY_FILTERING_ENABLED
    CFG->filtering_enabled       = SNOOPY_TRUE;
    CFG->filter_chain            = SNOOPY_FILTER_CHAIN;
#else
    CFG->filtering_enabled       = SNOOPY_FALSE;
    CFG->filter_chain            = "";
#endif
    CFG->filter_chain_malloced   = SNOOPY_FALSE;

    CFG->output                  = SNOOPY_OUTPUT_DEFAULT;
    CFG->output_malloced         = SNOOPY_FALSE;
    CFG->output_arg              = SNOOPY_OUTPUT_DEFAULT_ARG;
    CFG->output_arg_malloced     = SNOOPY_FALSE;

    CFG->syslog_facility         = SNOOPY_SYSLOG_FACILITY;
    CFG->syslog_ident            = SNOOPY_SYSLOG_IDENT;
    CFG->syslog_ident_malloced   = SNOOPY_FALSE;
    CFG->syslog_level            = SNOOPY_SYSLOG_LEVEL;
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
 *     snoopy_configuration_t*
 */
snoopy_configuration_t* snoopy_configuration_get ()
{
    snoopy_configuration_t *CFG;

#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
    CFG = snoopy_tsrm_get_configuration();
#else
    CFG = &snoopy_configuration_data;
#endif

    if (SNOOPY_TRUE != CFG->initialized) {
        snoopy_configuration_setDefaults(CFG);
    }

    return CFG;
}
