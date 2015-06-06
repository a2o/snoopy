/*
 * SNOOPY LOGGER
 *
 * File: snoopy/output/stdoutoutput.c
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
 * Includes order: from local to global
 */
#include "stdoutoutput.h"

#include <stdio.h>



/*
 * SNOOPY OUTPUT: stdoutoutput
 *
 * Description:
 *     Sends given message to STDOUT
 *
 * Params:
 *     message:    message to send
 *     errOrMsg:   is message and error message or ordinary Snoopy log message
 *     arg:        output argument(s)
 *
 * Return:
 *     int:        See snoopy.h (SNOOPY_OUTPUT_*) for details.
 */
int snoopy_output_stdoutoutput (char const * const logMessage, int errorOrMessage, char const * const arg)
{
    return fprintf(stdout, "%s\n", logMessage);
}
