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
 */
#include "file-snoopy.h"

#include "snoopy.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * Returns content of a small file
 *
 * Description:
 *     Reads the whole small* text** file and returns*** its whole content
 *
 *     *small: A file that this function reads should not be large. Its content
 *             should not exceed SNOOPY_UTIL_FILE_SMALL_FILE_MAX_SIZE bytes.
 *     **text: Only textual files should be handled using this function. They
 *             must not contain \0 byte(s).
 *             should not exceed SNOOPY_UTIL_FILE_SMALL_FILE_MAX_SIZE bytes.
 *     ***returns: A pointer to malloc()-ed memory block containing the whole
 *                file content.
 *
 * Params:
 *     filePath:     Path to the file that should be read
 *     contentPtr:   File content will be returned with this pointer (or an error message).
 *                   Must be free()-d after use.
 *
 * Return:
 *     int:        The size of the file content pointed to by the contentPtr
 *                 (excluding the last \0), or -1 on error (and contentPtr will
 *                 contain a null-terminated malloc()-ed error message).
 */
int snoopy_util_file_getSmallTextFileContent (char const * const filePath, char ** contentPtrAddr)
{
    FILE * fileHandle;
    char * contentPtr;
    char * errorMsgBuf;


    // Allocate the memory
    // Since this is mostly used for /proc reading, we can't use ftell() to get
    // the file size, hence we can't predict how much memory we'll need.
    contentPtr = malloc(SNOOPY_UTIL_FILE__SMALL_FILE_MAX_SIZE);
    if (contentPtr == NULL) {
        contentPtr = malloc(SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE);
        snprintf(
            contentPtr,
            SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE,
            "Unable to malloc() %d bytes", SNOOPY_UTIL_FILE__SMALL_FILE_MAX_SIZE
        );
        contentPtr[SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE-1] = '\0';
        *contentPtrAddr = contentPtr;
        return -1;
    }
    contentPtr[0] = '\0';


    // Open
    fileHandle = fopen(filePath, "r");
    if (fileHandle == NULL) {
        free(contentPtr);
        contentPtr = malloc(SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE);
        errorMsgBuf = malloc(SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE);
        errorMsgBuf[0] = '\0';
        strerror_r(errno, errorMsgBuf, SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE);
        errorMsgBuf[SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE-1] = '\0';
        snprintf(
            contentPtr,
            SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE,
            "Unable to open file %s for reading, reason: %s", filePath, errorMsgBuf
        );
        contentPtr[SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE-1] = '\0';
        *contentPtrAddr = contentPtr;
        free(errorMsgBuf);
        return -1;
    }


    // Read the file content
    size_t bytesReadTotal = 0;
    while (bytesReadTotal < SNOOPY_UTIL_FILE__SMALL_FILE_MAX_SIZE) {
        size_t bytesReadNow;
        bytesReadNow = fread(contentPtr+bytesReadTotal, 1, SNOOPY_UTIL_FILE__SMALL_FILE_FREAD_SIZE, fileHandle);
        bytesReadTotal += bytesReadNow;

        if (ferror(fileHandle)) {
            free(contentPtr);
            contentPtr = malloc(SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE);
            errorMsgBuf = malloc(SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE);
            errorMsgBuf[0] = '\0';
            strerror_r(errno, errorMsgBuf, SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE);
            errorMsgBuf[SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE-1] = '\0';
            snprintf(
                contentPtr,
                SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE,
                "Error reading file: %s", errorMsgBuf
            );
            contentPtr[SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE-1] = '\0';
            *contentPtrAddr = contentPtr;
            clearerr(fileHandle);
            fclose(fileHandle);
            free(errorMsgBuf);
            return -1;
        }

        if (
            feof(fileHandle) 
            ||
            // At one point, feof() didn't seem to work as expected (at least not on /proc/ files).
            // Potentially remove this section in the future.
            (bytesReadNow < SNOOPY_UTIL_FILE__SMALL_FILE_FREAD_SIZE)
        ) {
            break;
        }
    }

    if (bytesReadTotal >= SNOOPY_UTIL_FILE__SMALL_FILE_MAX_SIZE) {
        free(contentPtr);
        contentPtr = malloc(SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE);
        snprintf(
            contentPtr,
            SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE,
            "INTERNAL ERROR: File too large for getSmallTextFileContent()"
        );
        contentPtr[SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE-1] = '\0';
        *contentPtrAddr = contentPtr;
        fclose(fileHandle);
        return -1;
    }

    if (bytesReadTotal < SNOOPY_UTIL_FILE__SMALL_FILE_MAX_SIZE-1) {
        contentPtr[bytesReadTotal] = '\0';
    } else {
        contentPtr[SNOOPY_UTIL_FILE__SMALL_FILE_MAX_SIZE-1] = '\0';
    }


    // Cleanup
    fclose(fileHandle);


    // Return
    *contentPtrAddr = contentPtr;
    return (int) bytesReadTotal;
}
