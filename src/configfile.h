/*
 * SNOOPY COMMAND LOGGER
 *
 * File: configfile.h
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



#include "snoopy.h"
#include "configuration.h"



#define SNOOPY_CONFIGFILE_OPTION_TYPE_BOOL      1
#define SNOOPY_CONFIGFILE_OPTION_TYPE_STRING    2
#define SNOOPY_CONFIGFILE_OPTION_TYPE_NONE      0 // Reserved for internal use



/* Config file data types */
typedef struct {
    int type;
    int (*valueParserPtr) (char const * const confValString, snoopy_configuration_t *CFG);
    char * (*getValueAsStringPtr) ();
} snoopy_configfile_optionData_t;

typedef struct {
    char const * const                  name;
    snoopy_configfile_optionData_t      data;
} snoopy_configfile_option_t;



/* Config file loading/parsing */
int   snoopy_configfile_load (char *iniFilePath);
int   snoopy_configfile_iniParser_callback (
    void* sth,
    const char* section,
    const char* name,
    const char* confValString
);

int     snoopy_configfile_parseValue_error_logging        (const char *confValString, snoopy_configuration_t* CFG);
#ifdef SNOOPY_FILTERING_ENABLED
int     snoopy_configfile_parseValue_filter_chain         (const char *confValString, snoopy_configuration_t* CFG);
#endif
int     snoopy_configfile_parseValue_message_format       (const char *confValString, snoopy_configuration_t* CFG);
int     snoopy_configfile_parseValue_output               (const char *confValString, snoopy_configuration_t* CFG);
int     snoopy_configfile_parseValue_syslog_facility      (const char *confValString, snoopy_configuration_t* CFG);
int     snoopy_configfile_parseValue_syslog_ident         (const char *confValString, snoopy_configuration_t* CFG);
int     snoopy_configfile_parseValue_syslog_level         (const char *confValString, snoopy_configuration_t* CFG);

char *  snoopy_configfile_getOptionValueAsString_error_logging   ();
#ifdef SNOOPY_FILTERING_ENABLED
char *  snoopy_configfile_getOptionValueAsString_filter_chain    ();
#endif
char *  snoopy_configfile_getOptionValueAsString_message_format  ();
char *  snoopy_configfile_getOptionValueAsString_output          ();
char *  snoopy_configfile_getOptionValueAsString_syslog_facility ();
char *  snoopy_configfile_getOptionValueAsString_syslog_ident    ();
char *  snoopy_configfile_getOptionValueAsString_syslog_level    ();



/* Internal cleanup/conversion methods */
char *snoopy_configfile_syslog_value_cleanup       (char *confVal);
char *snoopy_configfile_syslog_value_remove_prefix (char *confVal);

int   snoopy_configfile_getboolean (const char *c, int notfound);



/* Config option registry */
int                             snoopy_configfile_optionRegistry_getIdFromName (char const * const itemName);
snoopy_configfile_option_t *    snoopy_configfile_optionRegistry_getAll ();
char *                          snoopy_configfile_optionRegistry_getOptionValueAsString (char const * const configOptionName);
