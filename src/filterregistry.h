/*
 * SNOOPY LOGGER
 *
 * File: filterregistry.h
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
 * Two arrays holding data about filter functions
 */
extern char *snoopy_filterregistry_names[];
extern int (*snoopy_filterregistry_ptrs []) (char *logMessage, char *arg);



/*
 * Functions to manage and utilise filter providers
 */
int snoopy_filterregistry_call         (char *filterName, char *logMessage, char *filterArg);
int snoopy_filterregistry_getIndex     (char *filterName);
int snoopy_filterregistry_isRegistered (char *filterName);
