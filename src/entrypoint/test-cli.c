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
#include "test-cli.h"

#include "snoopy.h"

#include "init-deinit.h"

#include "configuration.h"
#include "inputdatastorage.h"

#include <stddef.h>



void snoopy_entrypoint_test_cli_init (const char *filename, char *const argv[], char * const configFilePath)
{
    if (configFilePath != NULL) {
        snoopy_configuration_preinit_enableAltConfigFileParsing(configFilePath);
    } else {
        snoopy_configuration_preinit_disableConfigFileParsing();
    }

    snoopy_init();

    snoopy_inputdatastorage_store_filename(filename);
    snoopy_inputdatastorage_store_argv(argv);
    char *envp[] = { NULL };
    snoopy_inputdatastorage_store_envp(envp);
}



void snoopy_entrypoint_test_cli_threads_init ()
{
    snoopy_init();
}



void snoopy_entrypoint_test_cli_exit ()
{
    snoopy_cleanup();
}
