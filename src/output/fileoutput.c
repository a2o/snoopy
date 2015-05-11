/*
 * SNOOPY LOGGER
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
 * SNOOPY OUTPUT: fileoutput (called like this because <socket.h> is system library
 *
 * Description:
 *     Sends given message to socket
 *
 * Params:
 *     message: message to send
 *
 * Return:
 *     void
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snoopy.h"
#include "configuration.h"



int snoopy_output_fileoutput (char *logMessage, int errorOrMessage, char *arg)
{
    FILE  *fp;
    int    charCount;

    // Check if output file is properly configured
    if (0 == strcmp(arg, "")) {
        return -1;
    }

    // Try to open file in append mode
    fp = fopen(arg, "a");
    if (NULL == fp) {
        return -2;
    }

    // Try to print to file
    charCount = fprintf(fp, "%s\n", logMessage);
    fclose(fp);
    return charCount;
}

