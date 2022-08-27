/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2020 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "action-enable.h"

#include "cli-subroutines.h"

#include "snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int snoopy_cli_action_enable ()
{
    const char * libsnoopySoPath;
    char * curEtcLdSoPreloadContent = 0;
    char * newEtcLdSoPreloadContent = 0;
    size_t newEtcLdSoPreloadContentLength;
    size_t newEtcLdSoPreloadContentLengthBuf;
    char * strPosPtr = 0;


    // Get file we're working with (prod or test)
    libsnoopySoPath = libsnoopySo_getFilePath();

    // Make sure the target SO library file exists and is readable
    if (access(libsnoopySoPath, F_OK) != 0) {
        printDiagValue("libsnoopy.so path", libsnoopySoPath);
        fatalError("File not found");
    }
    if (access(libsnoopySoPath, R_OK) != 0) {
        printDiagValue("libsnoopy.so path", libsnoopySoPath);
        fatalError("File not readable");
    }


    // Read current /etc/ld.so.preload content
    curEtcLdSoPreloadContent = etcLdSoPreload_readFile();


    // Check if OUR Snoopy is already enabled
    if (etcLdSoPreload_findEntry(curEtcLdSoPreloadContent, libsnoopySoPath)) {
        free(curEtcLdSoPreloadContent);
        printDiagValue("ld.so.preload path", g_etcLdSoPreloadPath);
        printDiagValue("Search string", libsnoopySoPath);
        printNotice("Snoopy is already enabled in /etc/ld.so.preload.");
        return 0;
    }


    // Check if ANOTHER Snoopy instance is already enabled
    if (etcLdSoPreload_findNonCommentLineContainingString(curEtcLdSoPreloadContent, SNOOPY_SO_LIBRARY_NAME) != NULL) {
        printDiagValue("ld.so.preload path", g_etcLdSoPreloadPath);
        printDiagValue("Search string", SNOOPY_SO_LIBRARY_NAME);
        fatalError("Another Snoopy instance encountered.");
    }


    // Prepare the new content
    newEtcLdSoPreloadContentLength    = strlen(curEtcLdSoPreloadContent) + strlen(libsnoopySoPath) + 2; // 2 = first (potential) newline (if the orig content doesn't contain the final newline) + last newline
    newEtcLdSoPreloadContentLengthBuf = newEtcLdSoPreloadContentLength + 1; // +1 for the final \0
    newEtcLdSoPreloadContent          = malloc(newEtcLdSoPreloadContentLengthBuf);
    /*
     * If the following line is missing, GCC throws the following error:
     *
     *     error: ‘__builtin_strncpy’ specified bound depends on the length of the source argument [-Werror=stringop-overflow=]
     *
     * The added code is bogus, as the final \0 is added at the end of this code section anyway.
     */
    newEtcLdSoPreloadContent[newEtcLdSoPreloadContentLength] = '\0';

    // Copy the existing content (and add a newline at the end if missing)
    if (strlen(curEtcLdSoPreloadContent) == 0) {
        strPosPtr = newEtcLdSoPreloadContent;
    } else {
        strncpy(newEtcLdSoPreloadContent, curEtcLdSoPreloadContent, newEtcLdSoPreloadContentLengthBuf);

        strPosPtr = newEtcLdSoPreloadContent + strlen(curEtcLdSoPreloadContent)-1; // -1 Moves the pointer from the last \0 character to the penultimate one
        if (*strPosPtr != '\n') {
            strPosPtr++;
            *strPosPtr = '\n';
        }
        strPosPtr++;
    }

    // Add our entry + the final newline
    /*
     * For the following `strncpy` call we had originally this implementation:
     *
     *     strncpy(strPosPtr, libsnoopySoPath, strlen(libsnoopySoPath)+1);
     *
     * However, GCC (v9+, maybe even v8+) complains:
     *
     *     error: ‘__builtin_strncpy’ specified bound depends on the length of the source argument [-Werror=stringop-overflow=]
     *
     * Reworking the statement to the current version makes the warning go away,
     * despite having the exactly same effect (length of target buffer is calculated
     * from the length of the source buffer anyway).
     */
    strncpy(strPosPtr, libsnoopySoPath, newEtcLdSoPreloadContentLengthBuf-(strPosPtr-newEtcLdSoPreloadContent));
    strPosPtr += strlen(libsnoopySoPath);
    *strPosPtr = '\n';
    strPosPtr++;
    *strPosPtr = '\0';


    // Store new content
    etcLdSoPreload_writeFile(newEtcLdSoPreloadContent);
    printDiagValue("ld.so.preload path", g_etcLdSoPreloadPath);
    printDiagValue("Snoopy library path", libsnoopySoPath);
    printMessage("SUCCESS: Snoopy has been enabled.");


    free(curEtcLdSoPreloadContent);
    free(newEtcLdSoPreloadContent);
    return 0;
}
