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

#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
#include "action-stress-threads.h"
#include "action-stress-threadsexec.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



void snoopyTestCli_action_stress_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `stress`\n"
        "\n"
        "Usage:\n"
        "    snoopy stress SUBSYSTEM [ARGS]\n"
        "\n"
        "Available subsystems:\n"
#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
        "    threads,t          Stress internal threading implementation\n"
        "    threadsexec,te     Stress threading implementation by including execution of an external command\n"
#endif
        "\n"
        "    help,h             Show this help\n"
        "    SUBSYSTEM help     Show SUBSYSTEM's help\n"
        "\n";

    printf("%s", helpContent);
}



int snoopyTestCli_action_stress (int argc, char ** argv)
{
    const char * subsystem;

    if (argc < 1) {
        snoopyTestCli_action_stress_showHelp();
        fatalError("No subsystem specified.");
    }
    subsystem = argv[0];

    if ((0 == strcmp(subsystem, "help")) || (0 == strcmp(subsystem, "h"))) {
        snoopyTestCli_action_stress_showHelp();
        return 0;
    }

#ifdef SNOOPY_CONF_THREAD_SAFETY_ENABLED
    if ((0 == strcmp(subsystem, "threads")) || (0 == strcmp(subsystem, "t"))) {
        return snoopyTestCli_action_stress_threads(argc-1, &argv[1]);
    }
    if ((0 == strcmp(subsystem, "threadsexec")) || (0 == strcmp(subsystem, "te"))) {
        return snoopyTestCli_action_stress_threadsexec(argc-1, &argv[1]);
    }
#endif

    snoopyTestCli_action_stress_showHelp();
    fatalErrorValue("Unknown subsystem", subsystem);
    return 127;
}
