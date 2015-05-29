/*
 * SNOOPY LOGGER
 *
 * File: log.h
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
 * Syscall-interception-handling functions
 */
void snoopy_log_syscall_execv (
    const char  *filename,
    char *const  argv[]
);
void snoopy_log_syscall_execve (
    const char  *filename,
    char *const  argv[],
    char *const  envp[]
);
void snoopy_log_syscall_exec (
    const char  *syscallName,
    const char  *filename,
    char *const  argv[],
    char *const  envp[]
);



/*
 * Log message dispatching functions
 */
int snoopy_log_dispatch (
    char *logMessage,
    int   errorOrMessage
);
