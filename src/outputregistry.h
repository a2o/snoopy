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
 * Functions to manage and utilise outputs
 */
int   snoopy_outputregistry_getCount      ();
int   snoopy_outputregistry_doesIdExist   (int   outputId);
int   snoopy_outputregistry_doesNameExist (char const * const outputName);
int   snoopy_outputregistry_getIdFromName (char const * const outputName);
char* snoopy_outputregistry_getName       (int   outputId);

int   snoopy_outputregistry_callById      (int                outputId,   char const * const logMessage, int errorOrMessage, char const * const outputArg);
int   snoopy_outputregistry_callByName    (char const * const outputName, char const * const logMessage, int errorOrMessage, char const * const outputArg);

int   snoopy_outputregistry_dispatch                                     (char const * const logMessage, int errorOrMessage);
