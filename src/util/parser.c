/*
 * SNOOPY COMMAND LOGGER
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
#include "util/parser-snoopy.h"

#include "util/string-snoopy.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>



/*
 * Parses CSV list into an array of char pointers (modifies original CSV string)
 *
 * Last array entry contains only '\0' character.
 *
 * NOTE 1: argListRaw will be modified (commas will be replaced with \0s,
 *         argListParsed array will contain pointers to parts of this newly
 *         mangled argListRaw.
 * NOTE 2: argListParsed is malloc()-ed and must be freed after use.
 *
 * Params:
 *     argListRaw:      String containing CSV to parse
 *     argListParsed:   A pointer that will get malloc()-ed array of char pointers to individual CSV entries
 *
 * Return:
 *     int:             Number of arguments parsed out.
 */
int snoopy_util_parser_csvToArgList (char *argListRaw, char ***argListParsed)
{
    int    commaCount;
    int    argCount;
    char **argListParsedPtr;
    int    argListRaw_charCount;   // excluding trailing \0
    char  *argListRaw_pos;
    char  *nextCommaPtr;
    int    i;

    // Get size of raw argument list
    argListRaw_charCount = (int) strlen(argListRaw);

    // Allocate memory for array of string pointers:
    // - 1st +1 is because of common sense (X commas =~ X-1 arguments)
    // - 2nd +1 is for "" at the end or array
    commaCount       = snoopy_util_string_countChars(argListRaw, ',');
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
    // Optimize: do not count characters 2 times (1st in snoopy_util_string_charCount() function)
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



/*
 * Parses a textual representation of an integer into an actual integer
 *
 * Params:
 *     textInt:         String containing CSV to parse
 *     argListParsed:   A pointer that will get malloc()-ed array of char pointers to individual CSV entries
 *
 * Return:
 *     int:             Number of arguments parsed out.
 */
int snoopy_util_parser_strByteLength (char const * const numberAsText, const int valMin, const int valMax, const int valDefault)
{
    char const *numberAsTextPtr       = numberAsText;
    int  numbersBufLength             = 20; // 20 characters are needed to store max long long int in decimal representation + \0.
    char numbersBuf[numbersBufLength];
    int  numberInt;
    int  factor = 1;
    int  result;

    // Extract numbers
    while ((*numberAsTextPtr != '\0') && isdigit(*numberAsTextPtr) && (numberAsTextPtr-numberAsText < numbersBufLength-2)) {
        numbersBuf[numberAsTextPtr - numberAsText] = *numberAsTextPtr;
        numberAsTextPtr++;
    }
    numbersBuf[numberAsTextPtr - numberAsText] = '\0';

    // Convert to int
    numberInt = atoi(numbersBuf);
    if (numberInt == 0) {
        return valDefault;
    }

    // Apply metric prefixes
    if ((*numberAsTextPtr == 'k') || (*numberAsTextPtr == 'K')) {
        factor = 1024;
    } else if ((*numberAsTextPtr == 'm') || (*numberAsTextPtr == 'M')) {
        factor = 1024*1024;
    }
    result = numberInt * factor;

    // Apply limits
    if (result < valMin) result = valMin;
    if (result > valMax) result = valMax;

    return result;
}
