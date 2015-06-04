/*
 * SNOOPY LOGGER
 *
 * File: snoopy-test-all-data-sources.c
 *
 * Copyright (c) 2014-2015 Bostjan Skufca <bostjan@a2o.si>
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
 * Include all required C resources
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * Include all snoopy-related resources
 */
#include "snoopy.h"
#include "configuration.h"
#include "error.h"
#include "inputdatastorage.h"
#include "libsnoopy-debug-addons.h"
#include "misc.h"



/*
 * We do not use separate .h file here
 */
int  main (int argc, char **argv);
void snoopy_test_all_datasources ();
int  snoopy_get_datasource_name_length_max ();



int main (int argc, char **argv)
{
    /* Initialize Snoopy */
    snoopy_init();
    snoopy_inputdatastorage_store_filename(argv[0]);
    snoopy_inputdatastorage_store_argv(argv);

    /* Run the main function */
    snoopy_debug_test_all_datasources();

    /* Housekeeping */
    snoopy_cleanup();
    return 0;
}
