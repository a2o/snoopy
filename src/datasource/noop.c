/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/datasource/noop.c
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
#include "noop.h"



/*
 * SNOOPY DATA SOURCE: noop
 *
 * Description:
 *     Does nothing.
 *
 * Params:
 *     result: (ignored)
 *     arg:    (ignored)
 *
 * Return:
 *     number of characters in the returned string, or SNOOPY_DATASOURCE_FAILURE
 */
int snoopy_datasource_noop (__attribute__((unused)) char * const resultBuf, __attribute__((unused)) size_t resultBufSize, __attribute__((unused)) char const * const arg)
{
    return 0;
}
