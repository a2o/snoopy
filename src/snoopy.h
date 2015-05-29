/*
 * SNOOPY LOGGER
 *
 * snoopy.h
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



/**
 * Include all required system headers
 */

/* This should generaly be done wherever unistd.h is required. */
/* But sysconf is needed here, and all files include snoopy.h. */
/* Needed to get getpgid and getsid on older glibc */
/* This must be the first file to be included, or implicit inclusion
 * (by i.e. <features.h>) does the wrong thing
 */
#ifdef   _XOPEN_SOURCE
#undef   _XOPEN_SOURCE
#endif
#define  _XOPEN_SOURCE   500
#include <features.h>   /* Needed for GLIBC macros here */
#include <syslog.h>     /* Needed for syslog defaults */
#include <unistd.h>     /* Needed for _SC_ARG_MAX constant */



/**
 * Include ./configured constants
 *
 * At least <syslog.h> must be included before including this file, as
 * LOG_... constants are needed.
 */
#include "config.h"



/**
 * SNOOPY_MAX_ARG_LENGTH
 *
 * Maximum length of arguments passed to execv(e) functions
 */
#define SNOOPY_SYSCONF_ARG_MAX ((-1 == sysconf(_SC_ARG_MAX)) ? 4096 : sysconf(_SC_ARG_MAX))



/**
 * SNOOPY_DATASOURCE_ARG_MAX_SIZE
 *
 * Maximum length of a string argument to each data source
 */
#define SNOOPY_DATASOURCE_ARG_MAX_SIZE 1024



/**
 * SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE
 *
 * Maximum length of a string returned from any data source function,
 * including terminating null character.
 */
#define SNOOPY_DATASOURCE_MESSAGE_MAX_SIZE 1024



/**
 * SNOOPY_DATASOURCE_(SUCCESS|FAILURE|...)
 *
 * Datasource results
 */
#define   SNOOPY_DATASOURCE_SUCCESS              1
#define   SNOOPY_DATASOURCE_FAILURE             -1
#define   SNOOPY_DATASOURCE_SUCCEEDED(retVal)   (retVal >= 0)
#define   SNOOPY_DATASOURCE_FAILED(retVal)      (retVal <  0)



/**
 * SNOOPY_FILTER_CHAIN_MAX_SIZE
 *
 * Maximum length of filter chain definition
 */
#define SNOOPY_FILTER_CHAIN_MAX_SIZE 4096



/**
 * SNOOPY_FILTER_NAME_MAX_SIZE
 *
 * Maximum length of each filter name
 */
#define SNOOPY_FILTER_NAME_MAX_SIZE 1024



/**
 * SNOOPY_FILTER_ARG_MAX_SIZE
 *
 * Maximum length of a string argument to each filter
 */
#define SNOOPY_FILTER_ARG_MAX_SIZE 1024



/**
 * SNOOPY_LOG_MESSAGE_MAX_SIZE
 *
 * Maximum length of single (whole) log message,
 * including terminating null character.
 */
#define SNOOPY_LOG_MESSAGE_MAX_SIZE 16383



/**
 * SNOOPY_LOG_MESSAGE_FORMAT_default
 *
 * Default format of Snoopy log message
 */
#define   SNOOPY_VERSION   PACKAGE_VERSION



/**
 * SNOOPY_MESSAGE_FORMAT
 *
 * Actual message format to use, unless configured otherwise in config file.
 * For format specification consult comments in etc/snoopy.ini.
 */
#define   SNOOPY_MESSAGE_FORMAT   SNOOPY_CONF_MESSAGE_FORMAT



/**
 * SNOOPY_TRUE
 * SNOOPY_FALSE
 */
#define   SNOOPY_TRUE    1
#define   SNOOPY_FALSE   0



/**
 * SNOOPY_LOG_ERROR
 * SNOOPY_LOG_MESSAGE
 */
#define   SNOOPY_LOG_ERROR     1
#define   SNOOPY_LOG_MESSAGE   2



/**
 * Filter return values
 *
 * SNOOPY_FILTER_PASS - message should be passed on
 * SNOOPY_FILTER_DROP - message should be dropped
 */
#define   SNOOPY_FILTER_PASS   1
#define   SNOOPY_FILTER_DROP   0



/**
 * SNOOPY_FILTERING_ENABLED
 *
 * Whether filtering is enabled or not
 */
#ifdef SNOOPY_CONF_FILTERING_ENABLED
#define   SNOOPY_FILTERING_ENABLED   1
#endif



/**
 * SNOOPY_FILTER_CHAIN
 *
 * Actual filter chain specification to use
 *
 * Filter chain format:
 * - example: filter1; filter2; filter3:arg1; filter3:arg2
 * - you may pass argument to filter by suffixing it with :arg
 * - spaces are ignored
 */
#ifdef SNOOPY_FILTERING_ENABLED
#define   SNOOPY_FILTER_CHAIN   SNOOPY_CONF_FILTER_CHAIN
#endif



/**
 * SNOOPY_CONFIGFILE_*
 *
 * Path to INI configuration file, if enabled
 *
 * If configuration file is enabled, this constant is defined
 * and holds absolute path to it
 */
#ifdef SNOOPY_CONF_CONFIGFILE_ENABLED
#define   SNOOPY_CONFIGFILE_ENABLED   1
#define   SNOOPY_CONFIGFILE_PATH      SNOOPY_CONF_CONFIGFILE_PATH
#endif



/**
 * SNOOPY_OUTPUT
 *
 * Where is the outlet of Snoopy messages
 *
 * By default, messages get sent to syslog. Groundwork for other outputs
 * is provided to facilitate unforseen uses.
 */
#ifdef SNOOPY_CONF_OUTPUT_DEFAULT

#define   SNOOPY_OUTPUT_DEFAULT          SNOOPY_CONF_OUTPUT_DEFAULT
#define   SNOOPY_OUTPUT_DEFAULT_ARG      SNOOPY_CONF_OUTPUT_DEFAULT_ARG

#else   /* SNOOPY_CONF_OUTPUT_DEFAULT */

#if (defined(__GLIBC__) && (2 == __GLIBC__) && (__GLIBC_MINOR__ < 9))
/* Use 'syslog' on older linuxes that od not support SOCK_CLOEXEC and SOCK_NONBLOCK */
#define   SNOOPY_OUTPUT_DEFAULT          "syslog"
#else
/* Otherwise do not use 'syslog' (was default before), because systemd is funny (blocks the syslog() call */
#define   SNOOPY_OUTPUT_DEFAULT          "devlog"
#endif
#define   SNOOPY_OUTPUT_DEFAULT_ARG      ""

#endif   /* SNOOPY_CONF_OUTPUT_DEFAULT */



/**
 * Output return values/macro
 *
 * SNOOPY_OUTPUT_SUCCESS - message was sent successfully
 * SNOOPY_OUTPUT_FAILURE - message was NOT sent
 */
#define   SNOOPY_OUTPUT_SUCCESS              1
#define   SNOOPY_OUTPUT_GRACEFUL_DISCARD     0
#define   SNOOPY_OUTPUT_FAILURE             -1
#define   SNOOPY_OUTPUT_SUCCEEDED(retVal)   (retVal >= 0)
#define   SNOOPY_OUTPUT_FAILED(retVal)      (retVal <  0)



/**
 * SNOOPY_LOG_ERRORS
 *
 * Whether errors are logged or not?
 *
 * If error logging is enabled, then all errors that occur when using Snoopy
 * will be sent to syslog. This is to aid developers and integrators with
 * their endeavours.
 */
#ifdef SNOOPY_CONF_ERROR_LOGGING_ENABLED
#define   SNOOPY_ERROR_LOGGING_ENABLED   SNOOPY_CONF_ERROR_LOGGING_ENABLED
#endif



/**
 * SNOOPY_SYSLOG_*
 *
 * Default syslog configuration settings
 */
#define   SNOOPY_SYSLOG_FACILITY   SNOOPY_CONF_SYSLOG_FACILITY
#define   SNOOPY_SYSLOG_IDENT      SNOOPY_CONF_SYSLOG_IDENT
#define   SNOOPY_SYSLOG_LEVEL      SNOOPY_CONF_SYSLOG_LEVEL
