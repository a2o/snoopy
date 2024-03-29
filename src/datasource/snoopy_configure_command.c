/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/datasource/snoopy_configure_command.c
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
#include "snoopy_configure_command.h"

#include "snoopy.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>



/*
 * SNOOPY DATA SOURCE: snoopy_configure_command
 *
 * Description:
 *     Dummy data source that returns the configure command that was used to build Snoopy.
 *
 * Params:
 *     result: pointer to string, to write result into
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_snoopy_configure_command (char * const resultBuf, size_t resultBufSize, __attribute__((unused)) char const * const arg)
{
    return snprintf(resultBuf, resultBufSize, "%s", SNOOPY_CONFIGURE_COMMAND);
}
