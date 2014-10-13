/*
 * SNOOPY LOGGER
 *
 * File: inputdatastorage.c
 *
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
 * Includes
 */
#include "snoopy.h"
#include "inputdatastorage.h"



/*
 * Create these storage locations for inputs to consume
 */
const char *snoopy_inputdatastorage_filename;
char **snoopy_inputdatastorage_argv;
char **snoopy_inputdatastorage_envp;



/*
 * snoopy_inputdatastorage_store_filename()
 *
 * Description:
 *     Store filename of execv()/execve() syscall
 *
 * Params:
 *     filename:   filename to store
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_store_filename (
    const char *filename
) {
    snoopy_inputdatastorage_filename = filename;
}



/*
 * snoopy_inputdatastorage_store_argv()
 *
 * Description:
 *     Store argv[] of execv()/execve() syscall
 *
 * Params:
 *     argv:   argv pointer to store
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_store_argv (
    char * argv[]
) {
    snoopy_inputdatastorage_argv = argv;
}



/*
 * snoopy_inputdatastorage_store_envp()
 *
 * Description:
 *     Store envp[] of execve() syscall
 *
 * Params:
 *     envp:   environment array pointer to store
 *
 * Return:
 *     void
 */
void snoopy_inputdatastorage_store_envp (
    char * envp[]
) {
    snoopy_inputdatastorage_envp = envp;
}
