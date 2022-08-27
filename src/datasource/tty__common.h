/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/datasource/tty__common.h
 *
 * Copyright (c) 2020-2020 Bostjan Skufca <bostjan@a2o.si>
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



#include "snoopy.h"
#include <sys/types.h>



/*
 * SNOOPY DATA SOURCE: tty-related common functions
 */
int snoopy_datasource_tty__get_tty_uid (uid_t * ttyUid, char * const result);
