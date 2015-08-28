
/*
 * SNOOPY LOGGER
 *
 * File: snoopy/datasource/tty.h
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
 * Local defines
 */
#define   SNOOPY_DATASOURCE_TTY_sizeMaxWithoutNull   4096   // PATH_MAX does not work here?
#define   SNOOPY_DATASOURCE_TTY_sizeMaxWithNull      4097



/*
 * SNOOPY FILTER: only_tty
 */
int snoopy_filter_only_tty(char *msg, char const * const arg);
