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
 * Include configured constants
 */
#include "config.h"



/**
 * SNOOPY_MAX_ARG_LENGTH
 *
 * Maximum length of arguments passed to execv(e) functions.
 */
#include <unistd.h>
#define SNOOPY_SYSCONF_ARG_MAX sysconf(_SC_ARG_MAX)



/**
 * SNOOPY_INPUT_MESSAGE_MAX_SIZE
 *
 * Maximum length of a string returned from any input function,
 * without terminating null character.
 */
#define SNOOPY_INPUT_MESSAGE_MAX_SIZE 1024



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
 * Default format of snoopy log message.
 */
#define SNOOPY_LOG_MESSAGE_FORMAT_default \
    "[uid:%{uid} sid:%{sid} tty:%{tty} cwd:%{cwd} filename:%{filename}]: %{cmdline}"



/**
 * SNOOPY_LOG_MESSAGE_FORMAT
 *
 * Actual log message format to use
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
