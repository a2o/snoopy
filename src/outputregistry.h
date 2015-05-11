/*
 * SNOOPY LOGGER
 *
 * File: outputregistry.h
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
 * Two arrays holding data about output functions
 */
extern char *snoopy_outputregistry_names[];
extern int (*snoopy_outputregistry_ptrs []) (char *logMessage, int errorOrMessage, char *arg);



/*
 * Functions to manage and utilise output providers
 */
int snoopy_outputregistry_isRegistered (char *providerName);
int snoopy_outputregistry_getIndex     (char *providerName);
int snoopy_outputregistry_dispatch     (char *logMessage, int errorOrMessage);
