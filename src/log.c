/*
 * SNOOPY LOGGER
 *
 * File: log.c
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
 * Include all required C resources
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * Include all snoopy-related resources
 */
#include "snoopy.h"
#include "log.h"

#include "configuration.h"
#include "error.h"
#include "inputdatastorage.h"
#include "datasourceregistry.h"
#include "filterregistry.h"
#include "misc.h"
#include "outputregistry.h"



/*
 * snoopy_log_message_generate
 *
 * Description:
 *     Generates log message as specified by messageFormat, using
 *     data sources where requested.
 *
 * Params:
 *     logMessage:         destination string to return message in
 *     logMessageFormat:   log message format to use
 *
 * Return:
 *     void
 */
void snoopy_log_message_generate (
    char *logMessage,
    char *logMessageFormat
) {
    char *fmtPos_cur;
    char *fmtPos_nextFormatTag;
    char *fmtPos_nextFormatTagClose;
    char *msgPos_cur;

    fmtPos_cur           = logMessageFormat;
    fmtPos_nextFormatTag = logMessageFormat;
    msgPos_cur           = logMessage;

    // Loop all the way to the end of log message format specification
    while (strlen(fmtPos_nextFormatTag) > 0) {
        int   lengthToCopy;
        char  fmtStaticText[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE];
        char  dataSourceTag[100];
        int   dataSourceTagLength;
        char *fmtPos_dataSourceTagArg;
        char *dataSourceNamePtr;
        char *dataSourceArgPtr;
        char  dataSourceArg[SNOOPY_DATASOURCE_ARG_MAX_SIZE];
        char  dataSourceMsg[SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE];

        // If no data source tag is found, just copy the text and bail out
        fmtPos_nextFormatTag = strstr(fmtPos_cur, "%{");
        if (NULL == fmtPos_nextFormatTag) {
            snoopy_log_message_append(logMessage, fmtPos_cur);
            break;
        }

        // Otherwise copy text up to the next data source tag
        lengthToCopy = fmtPos_nextFormatTag - fmtPos_cur + 1; // + 1 for null termination
        if (lengthToCopy > SNOOPY_LOG_MESSAGE_MAX_SIZE-strlen(logMessage)) {
            lengthToCopy = SNOOPY_LOG_MESSAGE_MAX_SIZE-strlen(logMessage);
        }
        fmtStaticText[0] = '\0';
        snprintf(fmtStaticText, lengthToCopy, "%s", fmtPos_cur);
        snoopy_log_message_append(logMessage, fmtStaticText);
        msgPos_cur += lengthToCopy;

        // Get data source tag
        fmtPos_nextFormatTagClose = strstr(fmtPos_nextFormatTag, "}");
        if (NULL == fmtPos_nextFormatTagClose) {
            snoopy_log_message_append(logMessage, " ERROR: Closing data source tag not found: '}'");
            break;
        }
        dataSourceTag[0]    = '\0';
        dataSourceTagLength = (fmtPos_nextFormatTagClose-1) - (fmtPos_nextFormatTag+2) + 2;
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
        if (! snoopy_datasourceregistry_isRegistered(dataSourceNamePtr)) {
            snoopy_log_message_append(logMessage, "ERROR(Data source not found - ");
            snoopy_log_message_append(logMessage, dataSourceNamePtr);
            snoopy_log_message_append(logMessage, ")");
            break;
        }

        // Call the provider, and append the results to log message
        dataSourceMsg[0] = '\0';
        snoopy_datasourceregistry_call(dataSourceNamePtr, dataSourceMsg, dataSourceArgPtr);
        snoopy_log_message_append(logMessage, dataSourceMsg);

        // Where to start next iteration
        fmtPos_cur = fmtPos_nextFormatTagClose + 1;
        msgPos_cur = fmtPos_nextFormatTagClose + 1;
    }
}



/*
 * snoopy_log_message_append
 *
 * Description:
 *     Appends content to the end of log message, watching for
 *     buffer overruns
 *
 * Params:
 *     logMessage:   message container to append to
 *     appendThis:   content to append to logMessage
 *
 * Return:
 *     void
 */
void snoopy_log_message_append (
    char *logMessage,
    char *appendThis
) {
    int   logMessageSize          = -1;
    int   logMessageSizeRemaining = -1;
    int   appendThisSize          = -1;

    /* Verify the limits */
    logMessageSize          = strlen(logMessage);
    appendThisSize          = strlen(appendThis);
    logMessageSizeRemaining = SNOOPY_LOG_MESSAGE_MAX_SIZE - logMessageSize;
    if (logMessageSizeRemaining < appendThisSize) {
        snoopy_error_handler("Maximum log message size exceeded");
    }

    /* Copy to the destination string */
    strcat(&logMessage[logMessageSize], appendThis);
}



/*
 * snoopy_log_filter_check_chain
 *
 * Description:
 *     Determines whether given message should be send to syslog or not
 *
 * Params:
 *     logMessage:   message about to be sent to syslog
 *     chain:        filter chain to check
 *
 * Return:
 *     SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP
 */
int snoopy_log_filter_check_chain (
    char *logMessage,
    char *filterChain
) {
    char  filterChainCopy[SNOOPY_FILTER_CHAIN_MAX_SIZE];   // Must be here, or strtok_r segfaults
    int   filterChainCopySize;
    int   j;
    char *str;
    char *rest;
    char *filterSpec;            // Single filter specification from defined filter chain
    char *fcPos_filterSpecArg;   // Pointer to argument part of single filter specification in a filter chain

    // Copy the filter chain specification to separate string, to be used in strtok_r
    filterChainCopySize = strlen(filterChain);
    if (filterChainCopySize > SNOOPY_FILTER_CHAIN_MAX_SIZE - 1) {
        filterChainCopySize = SNOOPY_FILTER_CHAIN_MAX_SIZE - 1;
    }
    strncpy(filterChainCopy, filterChain, filterChainCopySize);
    filterChainCopy[filterChainCopySize+1] = '\0';

    // Loop through all filters
    for (j=1, str=filterChainCopy;  ; j++, str=NULL) {
        char    filterName[SNOOPY_FILTER_NAME_MAX_SIZE];
        char   *filterNamePtr;
        size_t  filterNameSize;
        char    filterArg[SNOOPY_FILTER_ARG_MAX_SIZE];
        char   *filterArgPtr;

        // Parse the remaining filter chain specification for next filterSpec
        filterSpec = strtok_r(str, ";", &rest);
        if (NULL == filterSpec) {
            // We are at the end of filtering chain
            break;
        }

        // If filter tag contains ":", then split it into filter name and filter argument
        fcPos_filterSpecArg  = strstr(filterSpec, ":");
        if (NULL == fcPos_filterSpecArg) {
            // filterSpec == filterName, there is no argument
            filterName[0] = '\0';
            filterNamePtr = filterSpec;
            filterArg[0]  = '\0';
            filterArgPtr  = filterArg;
        } else {
            // Change the colon to null character, which effectively splits the string in two parts.
            // Then point to first and second part with corresponding variables.
            filterNameSize = fcPos_filterSpecArg - filterSpec;
            filterName[0] = '\0';
            strncpy(filterName, filterSpec, filterNameSize);
            filterName[filterNameSize] = '\0';
            filterNamePtr = filterName;
            filterArgPtr  = fcPos_filterSpecArg + 1;
        }

        // Check if filter actually exists
        if (! snoopy_filterregistry_isRegistered(filterNamePtr)) {
            snoopy_log_message_append(logMessage, "ERROR(Filter not found - ");
            snoopy_log_message_append(logMessage, filterNamePtr);
            snoopy_log_message_append(logMessage, ")");
            break;
        }

        // Consult the filter, and return immediately if message should be dropped
        if (SNOOPY_FILTER_DROP == snoopy_filterregistry_call(filterNamePtr, logMessage, filterArgPtr)) {
            return SNOOPY_FILTER_DROP;
        }
    }
    return SNOOPY_FILTER_PASS;
}



/*
 * snoopy_log_message_dispatch
 *
 * Description:
 *     Dispatch given message to configured output
 *
 * Params:
 *     logMessage:       message to dispatch
 *     errorOrMessage:   is this a message or an error?
 *
 * Return:
 *     void
 */
void snoopy_log_message_dispatch (
    char *logMessage,
    int   errorOrMessage
) {
    /* Dispatch only if non-zero size */
    if (0 == strlen(logMessage)) {
        return;
    }

    // Dispatch to configured output
    snoopy_outputregistry_dispatch(logMessage, errorOrMessage);
}



/*
 * snoopy_log_syscall_execv
 *
 * Description:
 *     Log the call to syscall execv()
 *
 * Params:
 *     filename:   filename of program being executed
 *     argv:       arguments being passed to execv()
 *
 * Return:
 *     void
 */
void snoopy_log_syscall_execv (
    const char *filename,
    char *const argv[]
) {
    char *envp[] = { NULL };
    snoopy_inputdatastorage_store_filename(filename);
    snoopy_inputdatastorage_store_argv((char**)argv);
    snoopy_inputdatastorage_store_envp(envp);
    snoopy_log_syscall("execv");
}



/*
 * snoopy_log_syscall_execve
 *
 * Description:
 *     Log the call to syscall execve()
 *
 * Params:
 *     filename:   filename of program being executed
 *     argv:       arguments being passed to execve()
 *     envp:       environment being passed to execve()
 *
 * Return:
 *     void
 */
void snoopy_log_syscall_execve (
    const char *filename,
    char *const argv[],
    char *const envp[]
) {
    snoopy_inputdatastorage_store_filename(filename);
    snoopy_inputdatastorage_store_argv((char**)argv);
    snoopy_inputdatastorage_store_envp((char**)envp);
    snoopy_log_syscall("execve");
}



/*
 * snoopy_log_syscall
 *
 * Description:
 *     Log the call to one of the syscalls
 *
 * Params:
 *     syscallName:   system call name to log
 *
 * Return:
 *     void
 */
void snoopy_log_syscall (
    const char *syscallName
) {
    char *logMessage = NULL;

    /* Do not log non-root commands if this is requested */
#if defined(SNOOPY_CONF_ROOT_ONLY)
    if ((geteuid() != 0) && (getuid() != 0)) {
        return;
    }
#endif

    /* Initialize snoopy */
    snoopy_init();

    /* Initialize empty log message */
    logMessage    = malloc(SNOOPY_LOG_MESSAGE_MAX_SIZE);
    logMessage[0] = '\0';

    /* Generate log message in specified format */
    snoopy_log_message_generate(logMessage, snoopy_configuration.message_format);

    /* Should message be passed to syslog or not? */
    if (SNOOPY_TRUE == snoopy_configuration.filtering_enabled) {
        if (SNOOPY_FILTER_PASS == snoopy_log_filter_check_chain(logMessage, snoopy_configuration.filter_chain)) {
            snoopy_log_message_dispatch(logMessage, SNOOPY_LOG_MESSAGE);
        }
    } else {
        snoopy_log_message_dispatch(logMessage, SNOOPY_LOG_MESSAGE);
    }

    /* Housekeeping */
    free(logMessage);
    snoopy_cleanup();
}
