/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2022 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "action-run.h"
#include "action-stress.h"
#include "action-unit.h"

#include "cli/cli-subroutines.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>



/*
 * Store original argc & argv in global space
 */
int     g_argc;
char ** g_argv;



void snoopyTestCli_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility\n"
        "\n"
        "Usage:\n"
        "    snoopy ACTION [SUBACTION] [ARGS]\n"
        "\n"
        "Available actions:\n"
        "    run            Run Snoopy's subsystem\n"
        "    stress         Run stress tests\n"
        "    unit           Run unit tests\n"
        "\n"
        "    --help,-h      Show this help\n"
        "    ACTION --help  Show ACTION's help\n"
        "\n";

    printf("%s", helpContent);
}



int main (int argc, char ** argv)
{
    const char * action;

    g_argc = argc;
    g_argv = argv;


    if (argc < 2) {
        snoopyTestCli_showHelp();
        fatalError("No action specified.");
    }
    action = argv[1];


    if ((0 == strcmp(action, "--help")) || (0 == strcmp(action, "-h"))) {
        snoopyTestCli_showHelp();
        return 0;
    }


    if (0 == strcmp(action, "run")) {
        return snoopyTestCli_action_run(argc-2, &argv[2]);
    }

    if (0 == strcmp(action, "stress")) {
        return snoopyTestCli_action_stress(argc-2, &argv[2]);
    }

    if (0 == strcmp(action, "unit")) {
        return snoopyTestCli_action_unit(argc-2, &argv[2]);
    }


    snoopyTestCli_showHelp();
    fatalErrorValue("Unknown action", action);
}
