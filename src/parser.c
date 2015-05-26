/*
 * SNOOPY LOGGER
 *
 * File: parser.c
 *
 * Copyright (c) 2015 Bostjan Skufca <bostjan@a2o.si>
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
#include "parser.h"

#include "misc.h"

#include <stdlib.h>
#include <string.h>



/*
 * snoopy_parser_argList_csv
 *
 * Description:
 *     Parses comma-separated list of arguments in an array of char pointers.
 *     Last array entry contains only '\0' character.
 *
 *     NOTE 1: argListRaw will be modified (commas will be replaced with \0s,
 *             argListParsed array will contain pointers to parts of this newly
 *             mangled argListRaw.
 *     NOTE 2: argListParsed is malloc()-ed and must be freed afterwards.
 *
 * Params:
 *     char*
 *
 * Return:
 *     char*   Number of arguments parsed out.
 */
int snoopy_parser_argList_csv (char *argListRaw, char ***argListParsed)
{
    int    commaCount;
    int    argCount;
    char **argListParsedPtr;
    int    argListRaw_charCount;   // excluding trailing \0
    char  *argListRaw_pos;
    char  *nextCommaPtr;
    int    i;

    // Get size of raw argument list
    argListRaw_charCount = strlen(argListRaw);

    // Allocate memory for array of string pointers:
    // - 1st +1 is because of common sense (X commas =~ X-1 arguments)
    // - 2nd +1 is for "" at the end or array
    commaCount       = snoopy_string_countChars(argListRaw, ',');
    argCount         = commaCount + 1;
    argListParsedPtr = malloc(sizeof(char*) * (argCount+1));

    // Initial assignment
    if (0 == argListRaw_charCount) {
        // Argument list is empty, assign nothing, keep argCount down
        argCount            = 0;
        i                   = 0;
    } else {
        // Assign initial pointer, to beginning of character array
        // This might be the only argument if no comma was found.
        argListParsedPtr[0] = argListRaw;
        i                   = 1;
    }


    // Parse for commas, changing commas into '\0' characters and assigning pointers
    // Optimize: do not count characters 2 times (1st in snoopy_string_charCount() function)
    if (commaCount > 0 ) {
        argListRaw_pos = argListRaw;
        while (NULL != (nextCommaPtr = strchr(argListRaw_pos, ','))) {
            *nextCommaPtr     = '\0';
            argListRaw_pos   = nextCommaPtr+1;
            argListParsedPtr[i] = argListRaw_pos;
            i++;
        }
    }

    // Assign last pointer - must point to terminating null character
    argListParsedPtr[i] = argListRaw + argListRaw_charCount+1;
    *argListParsed      = argListParsedPtr;

    return argCount;
}
