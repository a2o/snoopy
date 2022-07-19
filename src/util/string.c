/*
 * SNOOPY LOGGER
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



/*
 * Includes order: from local to global (local, this-subsystem, snoopy.h, other-subsystems, global)
 *
 * Must be named "string-header.h" and not "string.h" in order to not conflict with the global <string.h>
 */
#include "string-header.h"

#include "snoopy.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * Verify whether given string only contains characters 0-9
 *
 * Params:
 *     str:     String to verify
 *
 * Return:
 *     int:     SNOOPY_TRUE or SNOOPY_FALSE
 */
int snoopy_util_string_containsOnlyDigits (const char * str)
{
    while (*str) {
        if (isdigit(*str++) == 0) {
            return SNOOPY_FALSE;
        }
    }

    return SNOOPY_TRUE;
}



/*
 * Copy the pointed-to text line from content into a new buffer
 *
 * Params:
 *     lineStartPtr:    Pointer to the start of the line. Can be a pointer to a
 *                      random location inside a larger (multiline) string. Must
 *                      point to the actual beginning of the line, or else the
 *                      preceding characters on the line will be ignored.
 *
 * Return:
 *     char *:          Pointer to malloc()-ed buffer containing the line copy (not containing the (potentially) terminating \n character)
 */
char * snoopy_util_string_copyLineFromContent (char const * const lineStartPtr)
{
    int lineLen = 0;
    char * copiedLine = NULL;

    lineLen = snoopy_util_string_getLineLength(lineStartPtr);

    copiedLine = malloc(lineLen+1);
    strncpy(copiedLine, lineStartPtr, lineLen);
    copiedLine[lineLen] = '\0';

    return copiedLine;
}



/*
 * Find a line in a multiline content that start with searchString
 *
 * Params:
 *      content:        Multi-line content to scan through
 *      searchString:   String to search for at the beginning of each line
 *
 * Return:
 *     const char *:    Pointer to the line starting with searchString, or NULL if not found
 */
char * snoopy_util_string_findLineStartingWith (char const * const content, char const * const searchString)
{
    const char * contentPos = NULL;
    char * foundStringPos = NULL;

    contentPos = content;

    while ((foundStringPos = strstr(contentPos, searchString)) != NULL) {

        // Check the start of the line
        if (
            (foundStringPos == content) // Start of the content
            ||
            ((foundStringPos > content) && (foundStringPos[-1] == '\n')) // The preceding character is a newline
        ) {
            return foundStringPos;
        }

        contentPos = foundStringPos + strlen(searchString);
    }

    return NULL;
}



/*
 * Get the length of a pointed-to text line
 *
 * Params:
 *     lineStartPtr:    Pointer to the start of the line. Can be a pointer to a
 *                      random location inside a larger (multiline) string. Must
 *                      point to the actual beginning of the line, or else the
 *                      preceding characters on the line will be ignored.
 *
 * Return:
 *     int:             Number of characters (bytes) in the pointed-to line (excluding the potential \n character)
 */
int snoopy_util_string_getLineLength (char const * const lineStartPtr)
{
    long lineLen = 0;
    const char * newlineCharPos = NULL;

    newlineCharPos = strchr(lineStartPtr, '\n');

    if (newlineCharPos == NULL) {
        lineLen = strlen(lineStartPtr);
    } else {
        lineLen = newlineCharPos - lineStartPtr;
    }

    return (int) lineLen;
}



/*
 * Null-terminate a line in content ("destroys" content in-place by replacing \n with \0)
 *
 * Params:
 *      lineInContent:  Pointer to a line in multiline content
 *
 * Return:
 *     void
 */
void snoopy_util_string_nullTerminateLine (char const * const lineInContent)
{
    char * newlineCharPos = NULL;

    newlineCharPos = strchr(lineInContent, '\n');

    if (newlineCharPos != NULL) {
        *newlineCharPos = '\0';
    }
}
