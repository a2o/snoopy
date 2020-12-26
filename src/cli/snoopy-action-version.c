/*
 * SNOOPY LOGGER
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
#include "snoopy-action-version.h"

#include "snoopy-cli-subroutines.h"

#include "snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>



int snoopy_action_version (int argc, char *argv[]) {
    printf("Snoopy CLI tool version: %s\n", SNOOPY_VERSION);


    void * handle;
    int (* snoopy_ds_version_ptr) (char * const result, char const * const arg);
    char * error;
    char ds_message_buf[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE] = "";
    int  ds_status;

    // Load the library
    handle = dlopen("libsnoopy.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    dlerror();    /* Clear any existing error */

    // Find the desired function
    *(void **) (&snoopy_ds_version_ptr) = dlsym(handle, "snoopy_datasource_snoopy_version");
    error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    // Call the function
    ds_status = snoopy_ds_version_ptr(ds_message_buf, "");
    if (SNOOPY_DATASOURCE_FAILED(ds_status)) {
        fprintf(stderr, "Snoopy version datasource failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Snoopy .so library version: %s\n", ds_message_buf);

    return 0;
}
