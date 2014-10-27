/*
 * SNOOPY LOGGER
 *
 * snoopy.h
 * Copyright (c) 2014 bostjan@a2o.si
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
 * Include ./configured constants
 */
#include "config.h"



/**
 * SNOOPY_MAX_ARG_LENGTH
 *
 * Maximum length of arguments passed to execv(e) functions
 */
#include <unistd.h>
#define SNOOPY_SYSCONF_ARG_MAX sysconf(_SC_ARG_MAX)



/**
 * SNOOPY_INPUT_ARG_MAX_SIZE
 *
 * Maximum length of a string argument to each input provider
 */
#define SNOOPY_INPUT_ARG_MAX_SIZE 1024



/**
 * SNOOPY_INPUT_MESSAGE_MAX_SIZE
 *
 * Maximum length of a string returned from any input function,
 * without terminating null character.
 */
#define SNOOPY_INPUT_MESSAGE_MAX_SIZE 1024



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
 * without terminating null character.
 */
#define SNOOPY_LOG_MESSAGE_MAX_SIZE 16383



/**
 * SNOOPY_LOG_MESSAGE_FORMAT_default
 *
 * Default format of snoopy log message
 */
#define SNOOPY_LOG_MESSAGE_FORMAT_default \
    "[uid:%{uid} sid:%{sid} tty:%{tty} cwd:%{cwd} filename:%{filename}]: %{cmdline}"



/**
 * SNOOPY_LOG_MESSAGE_FORMAT
 *
 * Actual log message format to use
 *
 * Message format may contain:
 * - any arbitrary text is copied litaraly
 * - text between "%{" and "}" is considered special - it calls input provider
 * - %{input_provider}     calls input provider 'input_provider' without argument
 * - %{input_provider:arg} calls input provider 'input_provider', and passed the given argument to it
 */
#ifdef SNOOPY_CONF_LOG_MESSAGE_FORMAT_custom
#define   SNOOPY_LOG_MESSAGE_FORMAT   SNOOPY_CONF_LOG_MESSAGE_FORMAT_custom
#else
#define   SNOOPY_LOG_MESSAGE_FORMAT   SNOOPY_LOG_MESSAGE_FORMAT_default
#endif



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
 * FIlter return values
 *
 * SNOOPY_FILTER_PASS - message should be passed on
 * SNOOPY_FILTER_DROP - message should be dropped
 */
#define   SNOOPY_FILTER_PASS   1
#define   SNOOPY_FILTER_DROP   0



/**
 * SNOOPY_FILTER_ENABLED
 *
 * Whether filtering is enabled or not
 */
#ifdef SNOOPY_CONF_FILTER_ENABLED
#define   SNOOPY_FILTER_ENABLED   1
#endif



/**
 * SNOOPY_FILTER_CHAIN_default
 *
 * Default snoopy filter chain specification
 */
#define   SNOOPY_FILTER_CHAIN_default   ""



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
#ifdef SNOOPY_CONF_FILTER_CHAIN_custom
#define   SNOOPY_FILTER_CHAIN   SNOOPY_CONF_FILTER_CHAIN_custom
#else
#define   SNOOPY_FILTER_CHAIN   SNOOPY_FILTER_CHAIN_default
#endif



/**
 * SNOOPY_CONFIG_FILE
 *
 * Path to INI configuration file, if enabled
 *
 * If configuration file is enabled, this constant is defined
 * and holds absolute path to it
 */
#ifdef SNOOPY_CONF_CONFIG_FILE
#define   SNOOPY_CONFIG_FILE   SNOOPY_CONF_CONFIG_FILE
#endif



/**
 * SNOOPY_LOG_ERRORS
 *
 * Whether errors are logged or not?
 *
 * If error logging is enabled, then all errors that occur when using snoopy
 * will be sent to syslog. This is to aid developers and integrators with
 * their endeavours.
 */
#ifdef SNOOPY_CONF_ERROR_LOGGING_ENABLED
#define   SNOOPY_ERROR_LOGGING_ENABLED   SNOOPY_CONF_ERROR_LOGGING_ENABLED
#endif
