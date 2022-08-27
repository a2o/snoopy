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
#define _GNU_SOURCE // For RTDL_NEXT & dladdr()

#include "action-status.h"

#include "cli-subroutines.h"

#include "snoopy.h"
#include "util/string-snoopy.h"

#include <dlfcn.h>
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef RTLD_DEFAULT
#  define RTLD_DEFAULT ((void *) 0)
#endif

typedef struct {
    int libcPos;
    int libsnoopyPos;
} solibsOrderT;



static int dl_iterate_phdr_callback (struct dl_phdr_info *i, size_t size, void *data)
{
    static int callCount = 0;
    solibsOrderT * solibsOrder;
    const char * strPos;

    solibsOrder = (solibsOrderT *) data;

    callCount++;

    const char * libsnoopyFileName = "/libsnoopy.so";
    strPos = strstr(i->dlpi_name, libsnoopyFileName);
    if ((strPos != NULL) && (strPos[strlen(libsnoopyFileName)] == '\0')) {
        solibsOrder->libsnoopyPos = callCount;
    }

    const char * libcFileName = "/libc.so.6";
    strPos = strstr(i->dlpi_name, libcFileName);
    if ((strPos != NULL) && (strPos[strlen(libcFileName)] == '\0')) {
        solibsOrder->libcPos = callCount;
    }

    return 0;
}



int snoopy_cli_action_status ()
{
    const char * libsnoopySoPath = NULL;
    char * etcLdSoPreloadContent = NULL;
    const char * linePtr = NULL;
    const char * entryPtr = NULL;

    // Get the file we're working with (prod or test)
    libsnoopySoPath = libsnoopySo_getFilePath();

    // Check /etc/ld.so.preload
    etcLdSoPreloadContent = etcLdSoPreload_readFile();

    linePtr = etcLdSoPreload_findNonCommentLineContainingString(etcLdSoPreloadContent, SNOOPY_SO_LIBRARY_NAME);
    if (linePtr == NULL) {
        printMessage("/etc/ld.so.preload:            NOT OK - Snoopy is not enabled.");
    } else {

        // Check for multiple concurrently-enabled libsnoopy.so instances
        linePtr += snoopy_util_string_getLineLength(linePtr);
        linePtr = etcLdSoPreload_findNonCommentLineContainingString(linePtr, SNOOPY_SO_LIBRARY_NAME);
        if (linePtr != NULL) {
            printDiagValue("ld.so.preload path", g_etcLdSoPreloadPath);
            printDiagValue("Search string", SNOOPY_SO_LIBRARY_NAME);
            fatalError("Multiple Snoopy references found, aborting.");
        }

        // Check for the right Snoopy instance
        entryPtr = etcLdSoPreload_findEntry(etcLdSoPreloadContent, libsnoopySoPath);
        if (entryPtr == NULL) {
            printMessage("/etc/ld.so.preload:            NOT OK - 'libsnoopy.so' found, but not with the expected path - is another Snoopy instance enabled?");
        } else {
            printMessage("/etc/ld.so.preload:            OK - Snoopy is enabled.");
        }
    }
    free(etcLdSoPreloadContent);


    // Check LD_PRELOAD environment variable?
    const char * envValue = NULL;
    envValue = getenv("LD_PRELOAD");
    if (envValue == NULL) {
        printMessage("LD_PRELOAD environment var:    NOT OK - Not set.");
    } else {
        if (strstr(envValue, SNOOPY_SO_LIBRARY_NAME) != NULL) {
            printMessage("LD_PRELOAD environment var:    OK - Snoopy reference found.");
        } else {
            printMessage("LD_PRELOAD environment var:    NOT SET - Set, but no Snoopy reference found.");
        }
    }


    // Check the order of shared libraries
    solibsOrderT solibsOrder = {
        .libcPos = 0,
        .libsnoopyPos = 0
    };

    dl_iterate_phdr(dl_iterate_phdr_callback, &solibsOrder);
    if (solibsOrder.libcPos == 0) {
        printWarning("Current process (shared libs): Unable to find libc.so.6");
    } else if (solibsOrder.libsnoopyPos == 0) {
        printMessage("Current process (shared libs): NOT OK - Unable to find libsnoopy.so.");
    } else {
        if (solibsOrder.libsnoopyPos < solibsOrder.libcPos) {
            printMessage("Current process (shared libs): OK - libsnoopy.so is loaded in front of libc.so.6.");
        } else {
            printMessage("Current process (shared libs): NOT OK - libsnoopy.so is loaded _after_ libc.so.6.");
        }
    }


    // Check the current running process' `execve` address
    typedef int (*execveAddrT) ();
    execveAddrT execveAddrLibc;
    execveAddrT execveAddrCur;
    const char * libcPath = "libc.so.6";

    void *libcHandle = dlopen(libcPath, RTLD_LAZY);
    *(void **) (&execveAddrCur)  = dlsym(RTLD_DEFAULT, "execve");
    *(void **) (&execveAddrLibc) = dlsym(libcHandle,   "execve");

    if (execveAddrLibc == execveAddrCur) {
        printMessage("Current process (execve addr): NOT OK - execve() symbol not overloaded, Snoopy is not loaded.");
    } else {
        printMessage("Current process (execve addr): OK - execve() symbol address changed, looks like Snoopy is loaded.");
    }

    dlclose(libcHandle);


    return 0;
}
