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
#include "systemd-unit-name.h"
#include "cgroup.h"

#include "snoopy.h"

#include "util/systemd-snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int snoopy_datasource_systemd_unit_name (char * const result, __attribute__((unused)) char const * const arg)
{
    char  cgroupEntry[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE] = "";
    int   cgroupDsRetVal;
    char *unitName = NULL;
    int   retMsgLen;


    // Get the cgroup entry
    cgroupDsRetVal = snoopy_datasource_cgroup(cgroupEntry, "name=systemd");
    if (
        (cgroupDsRetVal == SNOOPY_DATASOURCE_FAILURE)
        ||
        (0 == strcmp(cgroupEntry, "(none)"))
    ) {
        snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "Cgroup entry 'name=systemd' not found");
        return SNOOPY_DATASOURCE_FAILURE;
    }


    // Convert
    unitName = snoopy_util_systemd_convertCgroupEntryToUnitName(cgroupEntry);
    if (!unitName) {
        return snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", cgroupEntry+strlen("1:name=systemd:/"));
    }


    // Return
    retMsgLen = snprintf(result, SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE, "%s", unitName);
    free(unitName);
    return retMsgLen;
}
