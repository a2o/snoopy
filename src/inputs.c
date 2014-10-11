/*
 * SNOOPY LOGGER
 *
 * snoopy_inputs.c
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



/*
 * Include all input functions
 */
#include "inputs.h"



/*
 * Two arrays holding data about input functions
 */
const char *inputNames[] = {
    "cmdline",
    "cwd",
    "egid",
    "euid",
    "filename",
    "gid",
    "sid",
    "tty",
    "uid",
    "",
};
int (*inputPtrs []) (char *input) = {
    snoopy_input_cmdline,
    snoopy_input_cwd,
    snoopy_input_egid,
    snoopy_input_euid,
    snoopy_input_filename,
    snoopy_input_gid,
    snoopy_input_sid,
    snoopy_input_tty,
    snoopy_input_uid,
};
int inputCount = 9;
