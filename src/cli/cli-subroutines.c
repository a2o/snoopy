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
#include "cli-subroutines.h"

#include "snoopy.h"

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <limits.h>


const char * g_etcLdSoPreloadPath;
const char * g_libsnoopySoPath;
void * g_libsnoopySoHandle;


void printMessage (const char * const message)
{
    printf("%s\n", message);
}


void printDiag (const char * const message)
{
    printf("[DIAG] %s\n", message);
}
void printDiagValue (const char * const message, const char * const value)
{
    printf("[DIAG] %s: '%s'\n", message, value);
}


void printInfo (const char * const message)
{
    printf("[INFO] %s\n", message);
}
void printInfoValue (const char * const message, const char * const value)
{
    printf("[INFO] %s: %s\n", message, value);
}


void printNotice (const char * const message)
{
    printf("[NOTICE] %s\n", message);
}
void printNoticeValue (const char * const message, const char * const value)
{
    printf("[NOTICE] %s: %s\n", message, value);
}


void printSuccess (const char * const message)
{
    printf("[SUCCESS] %s\n", message);
}


void printWarning (const char * const message)
{
    fprintf(stderr, "[WARNING] %s\n", message);
}
void printWarningValue (const char * const message, const char * const value)
{
    fprintf(stderr, "[WARNING] %s: %s\n", message, value);
}


void printError (const char * const message)
{
    fprintf(stderr, "[ERROR] %s\n", message);
}
void printErrorValue (const char * const message, const char * const value)
{
    fprintf(stderr, "[ERROR] %s: %s\n", message, value);
}


void fatalError (const char * const message)
{
    printError(message);
    exit(127);
}
void fatalErrorValue (const char * const message, const char * const value)
{
    printErrorValue(message, value);
    exit(127);
}


char * libsnoopySo_getFilePath ()
{
    char * filePath;

    filePath = libsnoopySo_getFilePathNoCheck();

    if (access(filePath, R_OK) != 0) {
        printDiagValue("libsnoopy.so path", filePath);
        fatalError("Unable to access libsnoopy.so file");
    }

    return filePath;
}


char * libsnoopySo_getFilePathNoCheck ()
{
    char * filePath;
    char * envValue;

    envValue = getenv("SNOOPY_TEST_LIBSNOOPY_SO_PATH");
    if (envValue != NULL) {
        printWarningValue("Using non-default path to libsnoopy.so", envValue);
        filePath = envValue;
    } else {
        filePath = SNOOPY_SO_LIBRARY_PATH;
    }

    return filePath;
}


void libsnoopySo_load ()
{
    g_libsnoopySoPath = libsnoopySo_getFilePath();

    // Load the library
    g_libsnoopySoHandle = dlopen(g_libsnoopySoPath, RTLD_LAZY);
    if (!g_libsnoopySoHandle) {
        printErrorValue("Dynamic linker error message", dlerror());
        fatalError("Unable to load shared library");
    }
    dlerror();    /* Clear any existing error */
}


void * libsnoopySo_dlsym (char const * const functionName)
{
    void * functionPtr;
    const char * error;

    functionPtr = dlsym(g_libsnoopySoHandle, functionName);

    error = dlerror();
    if (error != NULL) {
        printDiagValue("libsnoopy.so path", g_libsnoopySoPath);
        fatalError(error);
    }

    return functionPtr;
}


const char * etcLdSoPreload_getFilePath ()
{
    const char * envValue = NULL;

    envValue = getenv("SNOOPY_TEST_LD_SO_PRELOAD_PATH");
    if (envValue != NULL) {
        printWarningValue("Using non-standard path to ld.so.preload", envValue);
        return envValue;
    } else {
        return SNOOPY_ETC_LD_SO_PRELOAD_PATH;
    }
}


char * etcLdSoPreload_readFile ()
{
    const char * filePath;
    int fileContentLen;
    char * fileContentBuf = 0;

    filePath = etcLdSoPreload_getFilePath();

    FILE * fileHandle = fopen(filePath, "r");
    if (fileHandle == NULL) {
        if (errno == ENOENT) {
            // For now, handle a non-existing file as an empty one
            fileContentBuf = malloc(1);
            fileContentBuf[0] = '\0';
            return fileContentBuf;
        } else {
            printDiagValue("ld.so.preload path", filePath);
            printDiagValue("Error message", strerror(errno));
            fatalError("Unable to read ld.so.preload file.");
        }
    }

    if (fseek(fileHandle, 0, SEEK_END) != 0) {
        printDiagValue("ld.so.preload path", filePath);
        printDiagValue("function called", "fseek(fh, 0, SEEK_END)");
        printDiagValue("Error message", strerror(errno));
        fatalError("Unable to determine the size of the ld.so.preload file.");
    }
    fileContentLen = (int) ftell(fileHandle);
    if (fileContentLen == -1) {
        printDiagValue("ld.so.preload path", filePath);
        printDiagValue("function called", "ftell(fh)");
        printDiagValue("Error message", strerror(errno));
        fatalError("Unable to determine the size of the ld.so.preload file.");
    }
    fseek(fileHandle, 0, SEEK_SET);

    fileContentBuf = malloc(fileContentLen+1);
    if (fileContentBuf == NULL) {
        printDiagValue("ld.so.preload path", filePath);
        printDiagValue("Error message", strerror(errno));
        fatalError("Unable to malloc() for reading the file content.");
    }

    if (fileContentLen == 0) {
        fileContentBuf[0] = '\0';
    } else {
        long freadRes = fread(fileContentBuf, 1, fileContentLen, fileHandle);
        if (freadRes < fileContentLen) {
            fileContentBuf[freadRes] = '\0';
            printDiagValue("ld.so.preload path", filePath);
            fatalError("Unable to read the whole content of the file.");
        } else {
            fileContentBuf[fileContentLen] = '\0';
        }
    }

    fclose(fileHandle);

    g_etcLdSoPreloadPath = filePath;
    return fileContentBuf;
}


void etcLdSoPreload_writeFile (char * newContent)
{
    const char * filePath;

    filePath = etcLdSoPreload_getFilePath();

    FILE * fileHandle = fopen(filePath, "w+");
    if (fileHandle == NULL) {
        printDiagValue("ld.so.preload path", filePath);
        printDiagValue("Error message", strerror(errno));
        fatalError("Unable to open file for writing (missing sudo, maybe?).");
    }

    if (fprintf(fileHandle, "%s", newContent) < 0) {
        printDiagValue("ld.so.preload path", filePath);
        printDiagValue("Error message", strerror(errno));
        fatalError("Unable to write to file.");
    }

    fclose(fileHandle);
}


const char * etcLdSoPreload_findEntry (const char * content, const char * entry)
{
    const char * contentPos = NULL;
    const char * entryPos = NULL;

    contentPos = content;

    while ((entryPos = strstr(contentPos, entry)) != NULL) {

        // Check the start of the line
        if (
                (
                    // Start of the content
                    (entryPos == content)
                    ||
                    // The preceding character is a newline
                    ((entryPos > content) && (entryPos[-1] == '\n'))
                )
                &&
                (
                    // End of content
                    (entryPos[strlen(entry)] == '\0')
                    ||
                    // Next character is a newline
                    (entryPos[strlen(entry)] == '\n')
                    ||
                    // Next character is a hash (start of a comment)
                    (entryPos[strlen(entry)] == '#')
                    ||
                    // Next character is a space (potentially followed by more space and/or a comment - a bit naive assumption, but hey!)
                    (entryPos[strlen(entry)] == ' ')
                    ||
                    // Next character is a tab (potentially followed by more tabs and/or a comment - a bit naive assumption, but hey!)
                    (entryPos[strlen(entry)] == '\t')
                )
        ) {
            return entryPos;
        }

        contentPos = entryPos + strlen(entry);
    }

    return NULL;
}


const char * etcLdSoPreload_findNonCommentLineContainingString (const char * content, const char * searchString)
{
    const char * contentPos = NULL;
    const char * foundStringPos = NULL;
    const char * lineStartPtr = NULL;

    contentPos = content;

    while ((foundStringPos = strstr(contentPos, searchString)) != NULL) {

        // Search in reverse for a newline character, or start of the buffer
        for (
            lineStartPtr = foundStringPos;
            (lineStartPtr > contentPos) && (*lineStartPtr != '\n');
            lineStartPtr--
        );

        if (*lineStartPtr == '\n') {
            lineStartPtr++;
        }

        if (*lineStartPtr != '#') {
            // This is not a commented-out line, therefore a valid search string has been found
            return lineStartPtr;
        }

        // If the found search string above is in a commented-out line, continue the search from where we left off
        contentPos = foundStringPos + strlen(searchString);
    }

    return NULL;
}
