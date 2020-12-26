/*
 * SNOOPY LOGGER
 *
 * Copyright (c) 2020 Bostjan Skufca Jese <bostjan@a2o.si>
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
#include "snoopy-action-enable.h"

#include <dlfcn.h>
#include <stdio.h>



#ifndef RTLD_DEFAULT
#  define RTLD_DEFAULT ((void *) 0)
#endif



int snoopy_action_enable (int argc, char *argv[]) {
    const char * libc = "/usr/lib/x86_64-linux-gnu/libc.so";
    void *handle = dlopen(libc, RTLD_LAZY);
    //simple test to see if the execve in memory matches libc.so.6
    if (dlsym(handle, "execve") != dlsym(RTLD_DEFAULT, "execve"))
        printf("Looks like something (Snoopy?) has overloaded the `execve()` function...\n");
    else
        printf("Looks like `execve()` is untouched...\n");
    return 0;
}
