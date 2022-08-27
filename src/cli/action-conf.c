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
#include "action-conf.h"

#include "cli-subroutines.h"

#include "configfile.h"
#include "configuration.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int snoopy_cli_action_conf ()
{
    void                         (* snoopy_entrypoint_cli_init_ptr) ();
    void                         (* snoopy_entrypoint_cli_exit_ptr) ();
    snoopy_configuration_t *     (* snoopy_configuration_get_ptr) ();
    snoopy_configfile_option_t * (* snoopy_configfile_optionRegistry_getAll_ptr) ();
    char *                       (* snoopy_configfile_optionRegistry_getOptionValueAsString_ptr) (char const * const optionName);

    snoopy_configuration_t * CFG;
    const snoopy_configfile_option_t * optionRegistry;


    // Load the library
    libsnoopySo_load();

    // Find function pointers
    *(void **) (&snoopy_entrypoint_cli_init_ptr)                              = libsnoopySo_dlsym("snoopy_entrypoint_cli_init");
    *(void **) (&snoopy_entrypoint_cli_exit_ptr)                              = libsnoopySo_dlsym("snoopy_entrypoint_cli_exit");
    *(void **) (&snoopy_configuration_get_ptr)                                = libsnoopySo_dlsym("snoopy_configuration_get");
    *(void **) (&snoopy_configfile_optionRegistry_getAll_ptr)                 = libsnoopySo_dlsym("snoopy_configfile_optionRegistry_getAll");
    *(void **) (&snoopy_configfile_optionRegistry_getOptionValueAsString_ptr) = libsnoopySo_dlsym("snoopy_configfile_optionRegistry_getOptionValueAsString");

    snoopy_entrypoint_cli_init_ptr();

    // Get the configuration
    CFG = snoopy_configuration_get_ptr();
    printf("; Options from config file (or defaults): %s\n", CFG->configfile_path);
    printf("[snoopy]\n");

    // Print the configured options
    optionRegistry = snoopy_configfile_optionRegistry_getAll_ptr();
    for (int i=0 ; 0 != strcmp(optionRegistry[i].name, "") ; i++) {
        char * optionValue = snoopy_configfile_optionRegistry_getOptionValueAsString_ptr(optionRegistry[i].name);
        printf("%s = %s\n", optionRegistry[i].name, optionValue);
        free(optionValue);
    }

    // Cleanup
    snoopy_entrypoint_cli_exit_ptr();

    return 0;
}
