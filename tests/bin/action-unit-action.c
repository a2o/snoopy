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
#include "snoopy.h"

#include "action-common.h"

#include "action-unit-action-log-message-dispatch.h"
#include "action-unit-action-log-syscall-exec.h"

#include <stdio.h>
#include <string.h>



void snoopyTestCli_action_unit_action_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `action`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit action SUBUNIT [ARGS]\n"
        "\n"
        "Available subunits:\n"
        "    log-message-dispatch,lmd   Run a unit test on src/action/log-message-dispatch.c\n"
        "    log-syscall-exec,lse       Run a unit test on src/action/log-syscall-exec.c\n"
        "\n"
        "    --help,-h          Show this help\n"
        "    UNIT --help        Show UNIT's help\n"
        "\n";

    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_action (int argc, char ** argv)
{
    const char * subunit;

    if (argc < 1) {
        snoopyTestCli_action_unit_action_showHelp();
        fatalError("No subunit specified.");
    }
    subunit = argv[0];


    if ((0 == strcmp(subunit, "log-message-dispatch")) || (0 == strcmp(subunit, "lmd"))) {
        return snoopyTestCli_action_unit_action_log_message_dispatch(argc-1, &argv[1]);
    }

    if ((0 == strcmp(subunit, "log-syscall-exec")) || (0 == strcmp(subunit, "lse"))) {
        return snoopyTestCli_action_unit_action_log_syscall_exec(argc-1, &argv[1]);
    }


    if ((0 == strcmp(subunit, "--help")) || (0 == strcmp(subunit, "-h"))) {
        snoopyTestCli_action_unit_action_showHelp();
        return 0;
    }


    snoopyTestCli_action_unit_action_showHelp();
    fatalErrorValue("Unknown subunit", subunit);
    return 127;
}
