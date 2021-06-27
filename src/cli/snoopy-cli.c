/*
 * SNOOPY LOGGER
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
#include "snoopy-action-detect.h"
#include "snoopy-action-disable.h"
#include "snoopy-action-enable.h"
#include "snoopy-action-version.h"

#include "snoopy-cli-subroutines.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>



void showAbout () {
    char * aboutContent =
        "Snoopy is a small library that logs all program executions on your Linux/BSD system (a.k.a. Snoopy Logger). "
        "This command line utility that you're currently running is Snoopy's tiny management tool.\n"
        "\n"
        "More information about Snoopy is available at the following URL:\n"
        "    https://github.com/a2o/snoopy/\n"
        "\n";

    printf("%s", aboutContent);
}



void showHelp () {
    char * helpContent =
        "Snoopy CLI tool usage:\n"
        "    snoopy ACTION [ARGS]\n"
        "\n"
        "Available actions:\n"
        "    detect         Detect whether Snoopy is already enabled and/or loaded\n"
        "    disable        Disable Snoopy\n"
        "    enable         Enable Snoopy\n"
        "\n"
        "    version        Show Snoopy version\n"
        "    about          Show general information\n"
        "    help           Show this help\n"
        "    ACTION help    Show action-specific help\n"
        "\n";

    printf("%s", helpContent);
}



int main (int argc, char *argv[]) {

    if (argc < 2) {
        showHelp();
        showError("Action not specified. See above for available options.");
        return 1;
    }

    if (0 == strcmp(argv[1], "detect")) {
        return snoopy_action_detect(argc, argv);
    } else if (0 == strcmp(argv[1], "disable")) {
        return snoopy_action_disable(argc, argv);
    } else if (0 == strcmp(argv[1], "enable")) {
        return snoopy_action_enable(argc, argv);

    } else if (0 == strcmp(argv[1], "version")) {
        return snoopy_action_version(argc, argv);
    } else if (0 == strcmp(argv[1], "about")) {
        showAbout();
        return 0;
    } else if (0 == strcmp(argv[1], "help")) {
        showAbout();
        return 0;

    } else {
        showHelp();
        showErrorValue("Unknown action", argv[1]);
        return 1;
    }
}
