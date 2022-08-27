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

#include "action-unit-util-syslog.h"
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_systemd_unit_name
#include "action-unit-util-systemd.h"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_ipaddr
#include "action-unit-util-utmp.h"
#endif

#include <stdio.h>
#include <string.h>



void snoopyTestCli_action_unit_util_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `util`\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit util SUBUNIT [ARGS]\n"
        "\n"
        "Available subunits:\n"
        "    syslog             Run a unit test on src/util/syslog.c\n"
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_systemd_unit_name
        "    systemd            Run a unit test on src/util/systemd.c\n"
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_ipaddr
        "    utmp               Unit test helper for src/util/utmp.c\n"
#endif
        "\n"
        "    --help,-h          Show this help\n"
        "    UNIT --help        Show UNIT's help\n"
        "\n";

    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_util (int argc, char ** argv)
{
    const char * subunit;

    if (argc < 1) {
        snoopyTestCli_action_unit_util_showHelp();
        fatalError("No subunit specified.");
    }
    subunit = argv[0];


    if (0 == strcmp(subunit, "syslog")) {
        return snoopyTestCli_action_unit_util_syslog(argc-1, &argv[1]);
    }
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_systemd_unit_name
    if (0 == strcmp(subunit, "systemd")) {
        return snoopyTestCli_action_unit_util_systemd(argc-1, &argv[1]);
    }
#endif
#ifdef SNOOPY_CONF_DATASOURCE_ENABLED_ipaddr
    if (0 == strcmp(subunit, "utmp")) {
        return snoopyTestCli_action_unit_util_utmp(argc-1, &argv[1]);
    }
#endif


    if ((0 == strcmp(subunit, "--help")) || (0 == strcmp(subunit, "-h"))) {
        snoopyTestCli_action_unit_util_showHelp();
        return 0;
    }


    snoopyTestCli_action_unit_util_showHelp();
    fatalErrorValue("Unknown subunit", subunit);
    return 127;
}
