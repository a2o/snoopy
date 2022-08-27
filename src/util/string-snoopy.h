/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2022 Bostjan Skufca Jese (bostjan _A_T_ a2o.si)
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



#include <stddef.h>



int             snoopy_util_string_append (char *destString, size_t destStringBufSize, const char *appendThis);
int             snoopy_util_string_containsOnlyDigits (char const * const str);
char *          snoopy_util_string_copyLineFromContent (char const * const lineStartPtr);
int             snoopy_util_string_countChars (const char *stringToSearch, char characterToCount);
char *          snoopy_util_string_findLineStartingWith (char const * const content, char const * const searchString);
int             snoopy_util_string_getLineLength (char const * const lineStartPtr);
void            snoopy_util_string_nullTerminateLine (char const * const lineInContent);
void            snoopy_util_string_toUpper (char * s);
