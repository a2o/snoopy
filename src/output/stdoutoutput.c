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
 * SNOOPY OUTPUT: stdoutoutput
 *
 * Description:
 *     Sends given message to STDOUT
 *
 * Params:
 *     message: message to send
 *
 * Return:
 *     int: number of characters printed
 */
#include <stdio.h>



int snoopy_output_stdoutoutput (char *logMessage, int errorOrMessage, char *arg)
{
    return fprintf(stdout, "%s\n", logMessage);
}
