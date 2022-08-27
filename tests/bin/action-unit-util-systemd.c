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
#include "action-unit-util-systemd.h"
#include "action-common.h"

#include "util/systemd-snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * Local helper functions
 */
static void mockCgroupEntryConversion(char const * const cgroupEntry, char const * const expectedUnitName);



void snoopyTestCli_action_unit_util_systemd_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `util` :: Subunit 'systemd'\n"
        "\n"
        "Description:\n"
        "    Mocks src/action/systemd.c implementation code paths (mainly for the coverage of code parts/paths not covered by the test suite).\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit util systemd\n"
        "    snoopy-test unit util systemd --help\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_util_systemd (int argc, char ** argv)
{
    const char *arg1;

    if (argc > 0) {
        arg1 = argv[0];
    } else {
        arg1 = "";
    }

    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_unit_util_systemd_showHelp();
        return 0;
    }


    char         cgroupEntry1[]    = "1:name=systemd:/init.scope";
    const char * expectedUnitName1 = "init";
    mockCgroupEntryConversion(cgroupEntry1, expectedUnitName1);

    char         cgroupEntry2[]    = "1:name=systemd:/system.slice/dbus.service";
    const char * expectedUnitName2 = "dbus";
    mockCgroupEntryConversion(cgroupEntry2, expectedUnitName2);

    char         cgroupEntry3[]    = "1:name=systemd:/user.slice/user-0.slice/session-3177.scope";
    const char * expectedUnitName3 = "root"; // Expected to exist
    mockCgroupEntryConversion(cgroupEntry3, expectedUnitName3);

    char         cgroupEntry4[]    = "1:name=systemd:/user.slice/user-57123.slice/session-3177.scope";
    const char * expectedUnitName4 = "user-57123"; // Expected to not exist on the system
    mockCgroupEntryConversion(cgroupEntry4, expectedUnitName4);

    char         cgroupEntry5[]    = "1:name=systemd:/";
    const char * expectedUnitName5 = "-";
    mockCgroupEntryConversion(cgroupEntry5, expectedUnitName5);

    char         cgroupEntry6[]    = "0::/whatever";
    const char * expectedUnitName6 = NULL;
    mockCgroupEntryConversion(cgroupEntry6, expectedUnitName6);

    char         cgroupEntry7[]    = "1:name=systemd:/actions_job/44e2bfe9d6f3ece192a9f762d91c9514131b8cfbbd0469b1722fd8637ef6feac";
    const char * expectedUnitName7 = NULL;
    mockCgroupEntryConversion(cgroupEntry7, expectedUnitName7);


    printSuccess("Mocking src/unit/systemd.c complete.");
    return 0;
}



static void mockCgroupEntryConversion(char const * const cgroupEntry, char const * const expectedUnitName)
{
    char * unitName = snoopy_util_systemd_convertCgroupEntryToUnitName(cgroupEntry);

    if (!unitName && !expectedUnitName) {
        return; // Both are NULL, so it's expected
    }

    if (!unitName && expectedUnitName) {
        printDiagValue("cgroupEntry", cgroupEntry);
        printDiagValue("Expected unit name", expectedUnitName);
        fatalError("Unable to convert cgroup entry to Systemd unit name");
        return; // Fake for Sonar CI, fatalError() exits
    }

    if (unitName && !expectedUnitName) {
        printDiagValue("cgroupEntry", cgroupEntry);
        printDiagValue("Converted unit name", unitName);
        fatalError("Cgroup entry not converted to NULL");
        return; // Fake for Sonar CI, fatalError() exits
    }

    if (0 != strcmp(expectedUnitName, unitName)) {
        printDiagValue("cgroupEntry", cgroupEntry);
        printDiagValue("Expected  unit name", expectedUnitName);
        printDiagValue("Converted unit name", unitName);
        fatalError("Unexpected unit name conversion");
        return; // Fake for Sonar CI, fatalError() exits
    }

    free(unitName);
    return;
}
