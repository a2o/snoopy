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

#include "action-unit-action.h"
#include "action-unit-datasource.h"
#include "action-unit-error.h"
#ifdef SNOOPY_CONFIGFILE_ENABLED
#include "action-unit-ext-ini.h"
#endif
#ifdef SNOOPY_FILTERING_ENABLED
#include "action-unit-filterregistry.h"
#endif
#include "action-unit-outputregistry.h"
#include "action-unit-util.h"

#include <stdio.h>
#include <string.h>



void snoopyTestCli_action_unit_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit UNIT [ARGS]\n"
        "\n"
        "Available units:\n"
        "    action,a           Run a unit test on action/*.c\n"
        "    datasource,ds      Run a unit test on datasource/*.c\n"
        "    error,e            Run a unit test on src/error.c\n"
#ifdef SNOOPY_CONFIGFILE_ENABLED
        "    ext-ini,ei         Run a unit test on lib/inih/src/ini.c\n"
#endif
#ifdef SNOOPY_FILTERING_ENABLED
        "    filterregistry,fr  Run a unit test on src/filterregistry.c\n"
#endif
        "    outputregistry,or  Run a unit test on src/outputregistry.c\n"
        "    util,u             Run a unit test on src/util/*.c\n"
        "\n"
        "    --help,-h          Show this help\n"
        "    UNIT --help        Show UNIT's help\n"
        "\n";

    printf("%s", helpContent);
}



int snoopyTestCli_action_unit (int argc, char ** argv)
{
    const char * unit;

    if (argc < 1) {
        snoopyTestCli_action_unit_showHelp();
        fatalError("No unit specified.");
    }
    unit = argv[0];


    if ((0 == strcmp(unit, "action")) || (0 == strcmp(unit, "a"))) {
        return snoopyTestCli_action_unit_action(argc-1, &argv[1]);
    }

    if ((0 == strcmp(unit, "datasource")) || (0 == strcmp(unit, "ds"))) {
        return snoopyTestCli_action_unit_datasource(argc-1, &argv[1]);
    }

    if ((0 == strcmp(unit, "error")) || (0 == strcmp(unit, "e"))) {
        return snoopyTestCli_action_unit_error(argc-1, &argv[1]);
    }

#ifdef SNOOPY_CONFIGFILE_ENABLED
    if ((0 == strcmp(unit, "ext-ini")) || (0 == strcmp(unit, "ei"))) {
        return snoopyTestCli_action_unit_ext_ini(argc-1, &argv[1]);
    }
#endif

#ifdef SNOOPY_FILTERING_ENABLED
    if ((0 == strcmp(unit, "filterregistry")) || (0 == strcmp(unit, "fr"))) {
        return snoopyTestCli_action_unit_filterregistry(argc-1, &argv[1]);
    }
#endif

    if ((0 == strcmp(unit, "outputregistry")) || (0 == strcmp(unit, "or"))) {
        return snoopyTestCli_action_unit_outputregistry(argc-1, &argv[1]);
    }

    if ((0 == strcmp(unit, "util")) || (0 == strcmp(unit, "u"))) {
        return snoopyTestCli_action_unit_util(argc-1, &argv[1]);
    }


    if ((0 == strcmp(unit, "--help")) || (0 == strcmp(unit, "-h"))) {
        snoopyTestCli_action_unit_showHelp();
        return 0;
    }


    snoopyTestCli_action_unit_showHelp();
    fatalErrorValue("Unknown unit", unit);
    return 127;
}
