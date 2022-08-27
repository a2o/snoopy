/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2015 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "action-common.h"

#include "snoopy.h"
#include "entrypoint/test-cli.h"
#include "configuration.h"
#include "inputdatastorage.h"
#include "util/syslog-snoopy.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>



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



void snoopyTestCli_action_run_configfile_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `run` :: Subsystem `configfile`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test run configfile INI_FILE KEY\n"
        "\n"
        "Result:\n"
        "    Prints value of the requested configuration KEY from the given INI_FILE.\n"
        "\n"
        "Supported configuration keys (check etc/snoopy.ini for more information):\n"
        "    message_format\n"
        "    filter_chain\n"
        "    output\n"
        "    syslog_facility\n"
        "    syslog_ident\n"
        "    syslog_level\n"
        "NOTICE: These keys MUST be placed in a section named [snoopy].\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_run_configfile (int argc, char **argv)
{
    char *iniFilePath;
    const char *showConfigVar;
    snoopy_configuration_t *CFG;


    /* Check if all arguments are present */
    if (argc < 1) {
        snoopyTestCli_action_run_configfile_showHelp();
        fatalError("Missing argument: path to INI config file");
    }
    if (0 == strcmp(argv[0], "--help")) {
        snoopyTestCli_action_run_configfile_showHelp();
        return 0;
    }
    iniFilePath = argv[0];

    if (argc < 2) {
        snoopyTestCli_action_run_configfile_showHelp();
        fatalError("Missing argument: configuration variable to display");
    }
    showConfigVar = argv[1];


    /* Check if config file exists and is readable */
    if (-1 == access(iniFilePath, R_OK)) {
        snoopyTestCli_action_run_configfile_showHelp();
        printErrorValue("INI file path", iniFilePath);
        fatalErrorValue("Unable to open/read given INI file", strerror(errno));
    }


    /* Initialize Snoopy, which parses alternate configuration file too */
    snoopy_entrypoint_test_cli_init((char const *)g_argv[0], g_argv, iniFilePath);


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
        printf("%s\n", snoopy_util_syslog_convertFacilityToStr(CFG->syslog_facility));

    } else if (0 == strcmp(showConfigVar, "syslog_ident")) {
        printf("%s\n", CFG->syslog_ident_format);

    } else if (0 == strcmp(showConfigVar, "syslog_level")) {
        printf("%s\n", snoopy_util_syslog_convertLevelToStr(CFG->syslog_level));

    } else if (0 == strcmp(showConfigVar, "error_logging")) {
        printf("%s\n", (CFG->error_logging_enabled == SNOOPY_TRUE ? "y" : "n"));

    } else {
        fatalErrorValue("Unknown setting given", showConfigVar);
    }


    /* Housekeeping and return */
    snoopy_entrypoint_test_cli_exit();
    return 0;
}
