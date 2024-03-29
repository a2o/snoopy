/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/datasource/filename.c
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
 * Includes order: from local to global
 */
#include "filename.h"

#include "snoopy.h"

#include "inputdatastorage.h"
#include <stdio.h>



/*
 * SNOOPY DATA SOURCE: filename
 *
 * Description:
 *     Returns filename of program about to be executed
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_filename (char * const resultBuf, size_t resultBufSize, __attribute__((unused)) char const * const arg)
{
    const snoopy_inputdatastorage_t * snoopy_inputdatastorage;

    /* Get argument data of execv/e() call */
    snoopy_inputdatastorage = snoopy_inputdatastorage_get();

    return snprintf(resultBuf, resultBufSize, "%s", snoopy_inputdatastorage->filename);
}
