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
#include "action-unit-filterregistry.h"
#include "action-common.h"

#include "snoopy.h"

#include "filterregistry.h"

#include <stdio.h>
#include <string.h>



void snoopyTestCli_action_unit_filterregistry_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `filterregistry`\n"
        "\n"
        "Description:\n"
        "    Mocks filterregistry implementation code paths (mainly for the coverage of code parts/paths not covered by the test suite).\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit filterregistry\n"
        "    snoopy-test unit filterregistry --help\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_filterregistry (int argc, char ** argv)
{
    const char *arg1;

    if (argc > 0) {
        arg1 = argv[0];
    } else {
        arg1 = "";
    }

    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_unit_filterregistry_showHelp();
        return 0;
    }


    // Do the mocking
    const char * filterName = NULL;
    int filterCount = 0;
    int filterIdPreset = 0;
    int filterIdReceived = 0;

    filterCount = snoopy_filterregistry_getCount();
    if (filterCount < 1) {
        fatalError("No filters available, filter count is 0");
    }
    printf("[DEBUG] Number of filters available: %d\n", filterCount);

    if (snoopy_filterregistry_doesIdExist(filterIdPreset) != SNOOPY_TRUE) {
        fatalError("Filter with a preset ID (0) does not exist");
    }

    filterName = snoopy_filterregistry_getName(filterIdPreset);
    filterIdReceived = snoopy_filterregistry_getIdFromName(filterName);
    if (filterIdPreset != filterIdReceived) {
        fatalError("Filter ID returned when searching by name differs from the initially used ID to find that same filter");
    }

    filterName = "noop";
    filterIdReceived = snoopy_filterregistry_getIdFromName(filterName);
    snoopy_filterregistry_callById(filterIdReceived, "");

    if (-1 != snoopy_filterregistry_callById(-1, "")) {
        fatalError("Filter ID -1 unexpectedly exists");
    }
    if (-1 != snoopy_filterregistry_callByName("fakeFilterNameThatShouldNeverExist", "")) {
        fatalError("Filter with an unexpected name actually exists");
    }


    printSuccess("Mocking src/filterregistry.c complete.");
    return 0;
}
