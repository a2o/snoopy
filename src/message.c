/*
 * SNOOPY COMMAND LOGGER
 *
 * File: message.c
 *
 * Copyright (c) 2014-2015 Bostjan Skufca <bostjan@a2o.si>
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
#include "message.h"

#include "snoopy.h"
#include "configuration.h"
#include "error.h"
#include "datasourceregistry.h"
#include "util/string-snoopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * snoopy_message_generateFromFormat
 *
 * Description:
 *     Generates log message as specified by messageFormat, using
 *     data sources where requested.
 *
 * Params:
 *     logMessage:         destination string to return message in
 *     logMessageBufSize:  size of the destination buffer
 *     logMessageFormat:   log message format to use
 *
 * Return:
 *     void
 */
void snoopy_message_generateFromFormat (
    char * const logMessage,
    size_t       logMessageBufSize,
    char const * const logMessageFormat
) {
    char const * fmtPos_cur;
    char const * fmtPos_nextFormatTag;
    char const * fmtPos_nextFormatTagClose;
    int   retVal;

    fmtPos_cur           = logMessageFormat;
    fmtPos_nextFormatTag = logMessageFormat;

    // Loop all the way to the end of log message format specification
    while (strlen(fmtPos_nextFormatTag) > 0) {
        int   lengthToCopy;
        char  fmtStaticText[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE];
        char  dataSourceTag[100];
        int   dataSourceTagLength;
        char *fmtPos_dataSourceTagArg;
        const char *dataSourceNamePtr;
        const char *dataSourceArgPtr;
        char  dataSourceArg[SNOOPY_DATASOURCE_ARG_MAX_SIZE];
        char  dataSourceMsg[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE];

        // If no data source tag is found, just copy the text and bail out
        fmtPos_nextFormatTag = strstr(fmtPos_cur, "%{");
        if (NULL == fmtPos_nextFormatTag) {
            snoopy_message_append(logMessage, logMessageBufSize, fmtPos_cur);
            return; // Should be "break;" but SonarCloud is complaining about it
        }

        // Otherwise copy text up to the next data source tag
        lengthToCopy = (int) (fmtPos_nextFormatTag - fmtPos_cur + 1); // + 1 for null termination
        if (lengthToCopy > (int)(logMessageBufSize-strlen(logMessage))) {
            lengthToCopy = (int)(logMessageBufSize-strlen(logMessage));
        }
        fmtStaticText[0] = '\0';
        snprintf(fmtStaticText, lengthToCopy, "%s", fmtPos_cur);
        snoopy_message_append(logMessage, logMessageBufSize, fmtStaticText);

        // Get data source tag
        fmtPos_nextFormatTagClose = strstr(fmtPos_nextFormatTag, "}");
        if (NULL == fmtPos_nextFormatTagClose) {
            snoopy_message_append(logMessage, logMessageBufSize, "[ERROR: Closing data source tag ('}') not found.]");
            return; // Should be "break;" but SonarCloud is complaining about it
        }
        dataSourceTag[0]    = '\0';
        dataSourceTagLength = (int)((fmtPos_nextFormatTagClose-1) - (fmtPos_nextFormatTag+2) + 2);
        snprintf(dataSourceTag, dataSourceTagLength, "%s", fmtPos_nextFormatTag + 2);

        // If data source tag contains ":", then split it into data source name and data source argument
        fmtPos_dataSourceTagArg  = strstr(dataSourceTag, ":");
        if (NULL == fmtPos_dataSourceTagArg) {
            // Format tag == data source name ATM
            dataSourceNamePtr = dataSourceTag;
            dataSourceArg[0]  = '\0';
            dataSourceArgPtr  = dataSourceArg;
        } else {
            // Change the colon to null string, and copy fist and second part to corresponding variables
            fmtPos_dataSourceTagArg[0] = '\0';
            dataSourceNamePtr = dataSourceTag;
            dataSourceArgPtr  = fmtPos_dataSourceTagArg + 1;
        }

        // Check if data source actually exists
        if (! snoopy_datasourceregistry_doesNameExist(dataSourceNamePtr)) {
            snoopy_message_append(logMessage, logMessageBufSize, "[ERROR: Data source '");
            snoopy_message_append(logMessage, logMessageBufSize, dataSourceNamePtr);
            snoopy_message_append(logMessage, logMessageBufSize, "' not found.]");
            return; // Should be "break;" but SonarCloud is complaining about it
        }

        // Call the provider, and append the results to log message
        dataSourceMsg[0] = '\0';
        retVal = snoopy_datasourceregistry_callByName(dataSourceNamePtr, dataSourceMsg, dataSourceArgPtr);
        if (SNOOPY_DATASOURCE_FAILED(retVal)) {
            snoopy_message_append(logMessage, logMessageBufSize, "[ERROR: Data source '");
            snoopy_message_append(logMessage, logMessageBufSize, dataSourceNamePtr);
            snoopy_message_append(logMessage, logMessageBufSize, "' failed with the following error message: '");
            snoopy_message_append(logMessage, logMessageBufSize, dataSourceMsg);
            snoopy_message_append(logMessage, logMessageBufSize, "']");
        } else {
            snoopy_message_append(logMessage, logMessageBufSize, dataSourceMsg);
        }

        // Where to start next iteration
        fmtPos_cur = fmtPos_nextFormatTagClose + 1;
    }
}



/*
 * snoopy_message_append
 *
 * Description:
 *     Appends content to the end of log message, watching for
 *     buffer overrun.
 *
 * Params:
 *     logMessage:   message container to append to
 *     appendThis:   content to append to logMessage
 *
 * Return:
 *     void
 */
void snoopy_message_append (
    char * logMessage,
    size_t logMessageBufSize,
    char const * const appendThis
) {
    if (SNOOPY_ERROR == snoopy_util_string_append(logMessage, logMessageBufSize, appendThis)) {
        snoopy_error_handler("Maximum destination string size exceeded");
    }
}
