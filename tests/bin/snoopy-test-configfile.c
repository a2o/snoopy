/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-filter.c
 *
 * Copyright (c) 2015 Bostjan Skufca <bostjan@a2o.si>
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
//#include "snoopy-test-filter.h"

#include "snoopy.h"
#include "configuration.h"
#include "inputdatastorage.h"
#include "misc.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>



/*
 * We do not use separate .h file here
 */
int  main (int argc, char **argv);
void displayHelp();
int  fatalError(char *errorMsg);



int main (int argc, char **argv)
{
    char *iniFilePath;
    char *showConfigVar;
    snoopy_configuration_t *CFG;


    /* Check if all arguments are present */
    if (argc < 2) {
        displayHelp();
        return fatalError("Missing argument: path to INI config file");
    }
    iniFilePath = argv[1];

    if (argc < 3) {
        displayHelp();
        return fatalError("Missing argument: configuration variable to display");
    }
    showConfigVar = argv[2];


    /* Check if config file exists and is readable */
    if (-1 == access(iniFilePath, R_OK)) {
        displayHelp();
        return fatalError("Unable to open/read given config file");
    }


    /* Initialize Snoopy, which parses alternate configuration file too */
    snoopy_configuration_preinit_enableAltConfigFileParsing(iniFilePath);
    snoopy_init();
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);


    /* Get config pointer */
    CFG = snoopy_configuration_get();


    /* Output appropriate value */
    if        (0 == strcmp(showConfigVar, "message_format")) {
        printf("%s\n", CFG->message_format);

    } else if (0 == strcmp(showConfigVar, "filter_chain")) {
        printf("%s\n", CFG->filter_chain);

    } else if (0 == strcmp(showConfigVar, "output")) {
        printf("%s", CFG->output);
        if ('\0' != CFG->output_arg[0]) {
            printf(":%s", CFG->output_arg);
        }
        printf("\n");

    } else if (0 == strcmp(showConfigVar, "syslog_facility")) {
        printf("%s\n", snoopy_syslog_convert_facilityToStr(CFG->syslog_facility));

    } else if (0 == strcmp(showConfigVar, "syslog_ident")) {
        printf("%s\n", CFG->syslog_ident);

    } else if (0 == strcmp(showConfigVar, "syslog_level")) {
        printf("%s\n", snoopy_syslog_convert_levelToStr(CFG->syslog_level));

    } else {
        return fatalError("Unknown configuration variable given");
    }


    /* Housekeeping and return */
    snoopy_cleanup();
    return 0;
}



/*
 * displayHelp()
 *
 * Description:
 *     Displays help
 *
 * Params:
 *     (none)
 *
 * Return:
 *     void
 */
void displayHelp ()
{
    printf("\n");
    printf("Usage: \n");
    printf("    snoopy-test-configfile PATH-TO-INI CONFIG-VARIABLE-TO-DISPLAY\n");
    printf("\n");

    printf("Available configfile variables:\n");
    printf("    (check etc/snoopy.ini for list of supported configuration variables)\n");
    printf("\n");
}



/*
 * fatalError()
 *
 * Description:
 *     Displays error message + help and returns non-zero exit status
 *
 * Params:
 *     errorMsg   Error message to display to user
 *
 * Return:
 *     int        Exit status to return to calling process
 */
int fatalError (char *errorMsg)
{
    printf("ERROR: %s\n", errorMsg);
    printf("\n");
    return 127;
}
