/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2020 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "action-version.h"

#include "cli-subroutines.h"

#include "snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>



int snoopy_cli_action_version ()
{
    int (* snoopy_ds_version_ptr) (char * const result, char const * const arg);
    char ds_message_buf[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE] = "";
    int  ds_status;

    // Load the library
    libsnoopySo_load();

    // Find the desired datasource (that returns Snoopy version)
    *(void **) (&snoopy_ds_version_ptr) = libsnoopySo_dlsym("snoopy_datasource_snoopy_version");

    // Call the function
    ds_status = snoopy_ds_version_ptr(ds_message_buf, "");
    if (SNOOPY_DATASOURCE_FAILED(ds_status)) {
        fatalError("Snoopy version datasource failed");
    }

    printf("Snoopy CLI tool version: %s\n", SNOOPY_VERSION);
    printf("Snoopy library version:  %s (path: %s)\n", ds_message_buf, g_libsnoopySoPath);

    return 0;
}
