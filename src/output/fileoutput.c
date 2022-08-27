/*
 * SNOOPY COMMAND LOGGER
 *
 * File: snoopy/output/fileoutput.c
 *
 * Copyright (c) 2015 Bostjan Skufca (bostjan _A_T_ a2o.si)
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
#include "fileoutput.h"

#include "snoopy.h"
#include "configuration.h"
#include "message.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * SNOOPY OUTPUT: fileoutput (called like this because <socket.h> is system library
 *
 * Description:
 *     Sends given message to socket
 *
 * Params:
 *     message:    message to send
 *     arg:        output argument(s)
 *
 * Return:
 *     int:        See snoopy.h (SNOOPY_OUTPUT_*) for details.
 */
int snoopy_output_fileoutput (char const * const logMessage, char const * const arg)
{
    char   filePathBuf[PATH_MAX] = {'\0'};
    char * filePath = filePathBuf;
    FILE  *fp;
    int    charCount;

    // Check if output file is properly configured
    if (0 == strcmp(arg, "")) {
        return SNOOPY_OUTPUT_FAILURE;
    }

    // Parse the output file specification (i.e. for %{datetime} or similar tags)
    snoopy_message_generateFromFormat(filePath, PATH_MAX, arg);

    // Try to open file in append mode
    fp = fopen(filePath, "a");
    if (NULL == fp) {
        return SNOOPY_OUTPUT_FAILURE;
    }

    // Try to print to file
    charCount = fprintf(fp, "%s\n", logMessage);
    fclose(fp);
    return charCount;
}
