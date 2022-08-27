/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2020 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "snoopy.h"

#ifdef SNOOPY_CONFIGFILE_ENABLED
#include "action-conf.h"
#endif
#include "action-disable.h"
#include "action-enable.h"
#include "action-status.h"
#include "action-version.h"

#include "cli-subroutines.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>



void showAbout () {
    char * aboutContent =
        "Snoopy is a small library that logs all program executions on your Linux/BSD system (a.k.a. Snoopy Command Logger).\n"
        "The command line utility that you've just used to display this message is Snoopy's CLI management tool.\n"
        "\n"
        "More information about Snoopy is available at the following URL:\n"
        "    https://github.com/a2o/snoopy/\n"
        "\n";

    printf("%s", aboutContent);
}



void showHelp () {
    char * helpContent =
        "Snoopy CLI management tool usage:\n"
        "    snoopyctl ACTION [ARGS]\n"
        "\n"
        "Available actions:\n"
#ifdef SNOOPY_CONFIGFILE_ENABLED
        "    conf           Show configuration\n"
#endif
        "    disable        Remove libsnoopy.so from /etc/ld.so.preload\n"
        "    enable         Add libsnoopy.so to /etc/ld.so.preload\n"
        "    status         Detect whether Snoopy is already enabled and loaded\n"
        "\n"
        "    version        Show Snoopy version\n"
        "    about          Show general information\n"
        "    help           Show this help\n"
        "\n";

    printf("%s", helpContent);
}



int main (int argc, char *argv[]) {

    if (argc < 2) {
        showHelp();
        fatalError("No action specified.");
    }

    if (0 == strcmp(argv[1], "status")) {
        return snoopy_cli_action_status();
#ifdef SNOOPY_CONFIGFILE_ENABLED
    } else if (0 == strcmp(argv[1], "conf")) {
        return snoopy_cli_action_conf();
#endif
    } else if (0 == strcmp(argv[1], "disable")) {
        return snoopy_cli_action_disable();
    } else if (0 == strcmp(argv[1], "enable")) {
        return snoopy_cli_action_enable();

    } else if (0 == strcmp(argv[1], "version")) {
        return snoopy_cli_action_version();
    } else if (0 == strcmp(argv[1], "about")) {
        showAbout();
        return 0;
    } else if (0 == strcmp(argv[1], "help")) {
        showHelp();
        return 0;

    } else {
        showHelp();
        fatalErrorValue("Unknown action", argv[1]);
    }
}
