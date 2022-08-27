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



/*
 * Includes order: from local to global (local, this-subsystem, snoopy.h, other-subsystems, global)
 *
 * Header file must not be named "string.h" in order to not conflict with the global <string.h>.
 * Therefore we're using the "-snoopy" suffix - "string-snoopy.h".
 */
#include "string-snoopy.h"

#include "snoopy.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>



/*
 * Append content to the end of string, watching for buffer overrun
 *
 * Params:
 *     destString:              string container to append to
 *     destStringBufSize:       maximum length of dest string, including \0
 *     appendThis:              content to append to destString
 *
 * Return:
 *     int:     Appended size, or SNOOPY_ERROR (when remaining space in destination string is not big enough)
 */
int snoopy_util_string_append (char *destString, size_t destStringBufSize, const char *appendThis)
{
    size_t destStringSize          = 0;
    size_t destStringSizeRemaining = 0;
    size_t appendThisSize          = 0;

    /* Verify the limits */
    destStringSize          = strlen(destString);
    appendThisSize          = strlen(appendThis);
    destStringSizeRemaining = destStringBufSize - destStringSize;
    if (destStringSizeRemaining < appendThisSize) {
        return SNOOPY_ERROR;
    }

    /* Copy to the destination string */
    strcat(&destString[destStringSize], appendThis);
    return (int) appendThisSize;
}



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
 * Count number of occurrences of specified character in a given string
 *
 * Params:
 *     stringToSearch:   string to look into
 *     characterToCount: search for this character
 *
 * Return:
 *     int   Number of occurrences
 */
int  snoopy_util_string_countChars (const char *stringToSearch, char characterToCount)
{
    const char *strPtr = stringToSearch;
    int charCount      = 0;

    while ('\0' != *strPtr) {
        if (*strPtr == characterToCount) {
            charCount++;
        }
        strPtr++;
    }

    return charCount;
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



/*
 * Convert string to upper case
 *
 * Params:
 *     s:       Pointer to string to work on
 *
 * Return:
 *     void
 */
void snoopy_util_string_toUpper (char * s)
{
    while (*s) {
        if ((*s >= 'a' ) && (*s <= 'z')) {
            *s -= ('a'-'A');
        }
        s++;
    }
}
