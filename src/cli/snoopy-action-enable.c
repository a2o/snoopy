/*
 * SNOOPY LOGGER
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
#include "snoopy-action-enable.h"
#include "snoopy-cli-subroutines.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef RTLD_DEFAULT
#  define RTLD_DEFAULT ((void *) 0)
#endif



int snoopy_action_enable (int argc, char *argv[])
{
//    char * filePath = "/etc/ld.so.preload";
    char * filePath = "/etc/ld.so.preload.TES";
    long   fileContentLen;
    char * fileContentBuf = 0;


    FILE * fileHandle = fopen(filePath, "r");
    if (fileHandle == NULL) {
        showErrorValue("Unable to open file for reading", filePath);
        showErrorValue("Reason:", strerror(errno));
        return 1;
    }


    fseek(fileHandle, 0, SEEK_END);
    fileContentLen = ftell(fileHandle);
    fseek(fileHandle, 0, SEEK_SET);
    if (fileContentLen > 0) {
        fileContentBuf = malloc(fileContentLen+1);
        if (fileContentBuf == NULL) {
            showErrorValue("Unable to malloc() for reading file content from:", filePath);
            showErrorValue("Reason:", strerror(errno));
            return 1;
        }

        long freadRes = fread(fileContentBuf, 1, fileContentLen, fileHandle);
        if (freadRes != fileContentLen) {
            showErrorValue("Unable to read the whole content of", filePath);
            printf("Expected bytes count: %ld", fileContentLen);
            printf("Obtained bytes count: %ld", freadRes);
            return 1;
        }
    }
    fclose (fileHandle);

    printf("%s content:\n", filePath);
    printf("%s", fileContentBuf);
    free(fileContentBuf);
    return 0;
}
