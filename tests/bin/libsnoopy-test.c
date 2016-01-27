/*
 * SNOOPY LOGGER
 *
 * File: snoopy.c -- execve() logging wrapper
 *
 * Copyright (c) 2000 Marius Aamodt Eriksen <marius@linux.com>
 * Copyright (c) 2000 Mike Baker <mbm@linux.com>
 * Copyright (c) 2010-2015 Bostjan Skufca <bostjan@a2o.si>
 *
 * Part hacked on flight KL 0617, 30,000 ft or so over the Atlantic :)
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
//#include "libsnoopy-test.h"

#include "snoopy.h"
#include "configuration.h"
#include "log.h"

#include <dlfcn.h>



/*
 * Helpers to find pointer to overloaded function
 */
#if defined(RTLD_NEXT)
#  define REAL_LIBC RTLD_NEXT
#else
#  define REAL_LIBC ((void *) -1L)
#endif

#define FN(ptr, type, name, args)   ptr = (type (*)args)dlsym (REAL_LIBC, name)



/*
 * Function wrapper - execv()
 */
int execv (const char *filename, char *const argv[]) {
    static int (*func)(const char *, char **);

    FN(func, int, "execv", (const char *, char **const));
    snoopy_configuration_preinit_setConfigFilePathFromEnv();
    snoopy_log_syscall_execv(filename, argv);

    return (*func) (filename, (char **) argv);
}



/*
 * Function wrapper - execve()
 */
int execve (const char *filename, char *const argv[], char *const envp[])
{
    static int (*func)(const char *, char **, char **);

    FN(func, int, "execve", (const char *, char **const, char **const));
    snoopy_configuration_preinit_setConfigFilePathFromEnv();
    snoopy_log_syscall_execve(filename, argv, envp);

    return (*func) (filename, (char**) argv, (char **) envp);
}
