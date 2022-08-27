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
#include "action-disable.h"

#include "cli-subroutines.h"

#include "snoopy.h"
#include "util/string-snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int snoopy_cli_action_disable ()
{
    const char * libsnoopySoPath;
    char * curEtcLdSoPreloadContent = 0;
    char * newEtcLdSoPreloadContent = 0;
    size_t newEtcLdSoPreloadContentLengthMax;
    unsigned int copyLength;
    const char * entryPtr = NULL;
    char * entryLine = NULL;
    const char * srcPosPtr = 0;
    char * destPosPtr = 0;
    const char * foundStringPos1 = NULL;
    const char * foundStringPos2 = NULL;


    // Get the file we're working with (prod or test)
    libsnoopySoPath = libsnoopySo_getFilePathNoCheck();

    // Read current /etc/ld.so.preload content
    curEtcLdSoPreloadContent = etcLdSoPreload_readFile();


    // Check for duplicate active libsnoopy.so
    foundStringPos1 = etcLdSoPreload_findNonCommentLineContainingString(curEtcLdSoPreloadContent, SNOOPY_SO_LIBRARY_NAME);
    if (foundStringPos1 != NULL) {
        foundStringPos2 = etcLdSoPreload_findNonCommentLineContainingString(foundStringPos1 + snoopy_util_string_getLineLength(foundStringPos1), SNOOPY_SO_LIBRARY_NAME);
        if (foundStringPos2 != NULL) {
            printDiagValue("Search string", SNOOPY_SO_LIBRARY_NAME);
            printDiagValue("ld.so.preload path", g_etcLdSoPreloadPath);
            fatalError("Duplicate libsnoopy.so entry encountered");
        }
    }


    // Check if OUR Snoopy is already disabled
    entryPtr = etcLdSoPreload_findEntry(curEtcLdSoPreloadContent, libsnoopySoPath);
    if (entryPtr == NULL) {
        free(curEtcLdSoPreloadContent);
        printDiagValue("ld.so.preload path", g_etcLdSoPreloadPath);
        printDiagValue("libsnoopy.so path", libsnoopySoPath);
        printNotice("Snoopy library is already absent from the ld.so.preload file.");
        return 0;
    }


    newEtcLdSoPreloadContentLengthMax = strlen(curEtcLdSoPreloadContent);
    newEtcLdSoPreloadContent          = malloc(newEtcLdSoPreloadContentLengthMax+1); // +1 for terminating \0
    newEtcLdSoPreloadContent[0] = '\0';

    // Copy the initial part
    destPosPtr = newEtcLdSoPreloadContent;
    srcPosPtr  = curEtcLdSoPreloadContent;
    copyLength = (unsigned int) (entryPtr - srcPosPtr);
    strncpy(destPosPtr, srcPosPtr, copyLength);

    // Skip the entry line we're removing, copy the rest
    destPosPtr = newEtcLdSoPreloadContent + copyLength;
    entryLine  = snoopy_util_string_copyLineFromContent(entryPtr);
    srcPosPtr  = entryPtr + strlen(entryLine);
    copyLength = (unsigned int) (strlen(curEtcLdSoPreloadContent) - (entryPtr - curEtcLdSoPreloadContent) - strlen(entryLine));
    if (*srcPosPtr == '\n') {
        srcPosPtr++;
        copyLength--;
    }
    strncpy(destPosPtr, srcPosPtr, copyLength);

    destPosPtr += copyLength;
    *destPosPtr = '\0';


    // Store new content
    etcLdSoPreload_writeFile(newEtcLdSoPreloadContent);
    printMessage("[SUCCESS] Snoopy has been removed from /etc/ld.so.preload.");
    printInfo("Existing processes may still have Snoopy enabled until they are restarted.");


    // Check if ANOTHER Snoopy instance is present
    if (etcLdSoPreload_findNonCommentLineContainingString(newEtcLdSoPreloadContent, SNOOPY_SO_LIBRARY_NAME) != NULL) {
        printDiagValue("ld.so.preload path", g_etcLdSoPreloadPath);
        printDiagValue("Search string", SNOOPY_SO_LIBRARY_NAME);
        printWarning("Another Snoopy instance found in ld.so.preload file.");
    }


    free(curEtcLdSoPreloadContent);
    free(newEtcLdSoPreloadContent);
    free(entryLine);
    return 0;
}
