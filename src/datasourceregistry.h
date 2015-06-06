/*
 * SNOOPY LOGGER
 *
 * File: datasourceregistry.h
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
 * Functions to manage and utilise datasources
 */
int   snoopy_datasourceregistry_getCount      ();
int   snoopy_datasourceregistry_doesIdExist   (int   datasourceId);
int   snoopy_datasourceregistry_doesNameExist (char const * const datasourceName);
int   snoopy_datasourceregistry_getIdFromName (char const * const datasourceName);
char* snoopy_datasourceregistry_getName       (int   datasourceId);

int   snoopy_datasourceregistry_callById      (int                datasourceId,   char * const result, char const * const datasourceArg);
int   snoopy_datasourceregistry_callByName    (char const * const datasourceName, char * const result, char const * const datasourceArg);
