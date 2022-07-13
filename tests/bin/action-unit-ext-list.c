/*
 * SNOOPY LOGGER
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
#include "action-unit-ext-ini.h"
#include "action-common.h"

#include "lib/liblcthw/src/list.h"

#include <stdio.h>
#include <string.h>



void snoopyTestCli_action_unit_ext_list_showHelp ()
{
    char * helpContent =
        "Snoopy TEST SUITE CLI utility :: Action `unit` :: Unit `ext-list`\n"
        "\n"
        "Description:\n"
        "    Mocks external list implementation code paths (mainly for the code coverage of parts not used by Snoopy).\n"
        "\n"
        "Usage:\n"
        "    snoopy-test unit ext-list\n"
        "    snoopy-test unit ext-list --help\n"
        "\n";
    printf("%s", helpContent);
}



int snoopyTestCli_action_unit_ext_list (int argc, char ** argv)
{
    const char *arg1;

    if (argc > 0) {
        arg1 = argv[0];
    } else {
        arg1 = "";
    }

    if (0 == strcmp(arg1, "--help")) {
        snoopyTestCli_action_unit_ext_list_showHelp();
        return 0;
    }


    // Do the mocking (mainly to cover the code paths that Snoopy does not need)
    List * mockList;
    char * mockVal1;
    char * mockVal2;
    char * mockVal3;

    mockList = List_create();
    mockVal1 = malloc(8);
    mockVal2 = malloc(8);
    mockVal3 = malloc(8);
    strncpy(mockVal1, "val1", 8);
    strncpy(mockVal2, "val2", 8);
    strncpy(mockVal3, "val3", 8);

    List_unshift(mockList, mockVal1);
    List_unshift(mockList, mockVal2);
    List_unshift(mockList, mockVal3);

    List_shift(mockList);
    List_pop(mockList);

    if (mockList->count != 1) {
        printf("Current list size: %d\n", mockList->count);
        fatalError("List size is not 1");
    }

    if (0 != strcmp(mockList->first->value, "val2")) {
        printf("Current node value: %s\n", (char *)mockList->first->value);
        fatalError("Unexpected node value");
    }

    List_clear_destroy(mockList);


    printSuccess("Mocking lib/liblcthw/src/list.c complete.");
    return 0;
}
