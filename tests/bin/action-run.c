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

#ifdef SNOOPY_CONFIGFILE_ENABLED
#include "action-run-configfile.h"
#endif
#include "action-run-datasource.h"
#ifdef SNOOPY_FILTERING_ENABLED
#include "action-run-filter.h"
#include "action-run-filterchain.h"
#endif
#include "action-run-messageformat.h"
#include "action-run-output.h"

#include "action-run-everything.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



void snoopyTestCli_action_run_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `run`\n"
        "\n"
        "Usage:\n"
        "    snoopy run SUBSYSTEM [ARGS]\n"
        "\n"
        "Available subsystems:\n"
#ifdef SNOOPY_CONFIGFILE_ENABLED
        "    configfile,cf      Run a configfile (.ini) parser\n"
#endif
        "    datasource,ds      Run a data source\n"
#ifdef SNOOPY_FILTERING_ENABLED
        "    filter,f           Run a filter\n"
        "    filterchain,fc     Run a filter chain (as if it would be configured in snoopy.ini)\n"
#endif
        "    messageformat,mf   Run the message formatter\n"
        "    output,o           Run an output\n"
        "\n"
        "    everything         Runs every subsystem, as much as possible (for Valgrind)\n"
        "\n"
        "    help,h             Show this help\n"
        "    SUBSYSTEM help     Show SUBSYSTEM's help\n"
        "\n";

    printf("%s", helpContent);
}



int snoopyTestCli_action_run (int argc, char ** argv)
{
    if (argc < 1) {
        snoopyTestCli_action_run_showHelp();
        fatalError("No subsystem specified.");
    }


#ifdef SNOOPY_CONFIGFILE_ENABLED
    if ((0 == strcmp(argv[0], "configfile")) || (0 == strcmp(argv[0], "cf"))) {
        return snoopyTestCli_action_run_configfile(argc-1, &argv[1]);
    }
#endif

    if ((0 == strcmp(argv[0], "datasource")) || (0 == strcmp(argv[0], "ds"))) {
        return snoopyTestCli_action_run_datasource(argc-1, &(argv[1]));
    }

#ifdef SNOOPY_FILTERING_ENABLED
    if ((0 == strcmp(argv[0], "filter")) || (0 == strcmp(argv[0], "f"))) {
        return snoopyTestCli_action_run_filter(argc-1, &argv[1]);
    }
    if ((0 == strcmp(argv[0], "filterchain")) || (0 == strcmp(argv[0], "fc"))) {
        return snoopyTestCli_action_run_filterchain(argc-1, &argv[1]);
    }
#endif

    if ((0 == strcmp(argv[0], "messageformat")) || (0 == strcmp(argv[0], "mf"))) {
        return snoopyTestCli_action_run_messageformat(argc-1, &argv[1]);
    }

    if ((0 == strcmp(argv[0], "output")) || (0 == strcmp(argv[0], "o"))) {
        return snoopyTestCli_action_run_output(argc-1, &argv[1]);
    }

    if (0 == strcmp(argv[0], "everything")) {
        return snoopyTestCli_action_run_everything();
    }


    if ((0 == strcmp(argv[0], "help")) || (0 == strcmp(argv[0], "h"))) {
        snoopyTestCli_action_run_showHelp();
        return 0;
    }


    snoopyTestCli_action_run_showHelp();
    fatalErrorValue("Unknown subsystem", argv[0]);
    return 127;
}
