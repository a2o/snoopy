/*
 * SNOOPY LOGGER
 *
 * File: inputdatastorage.h
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
 * START: Prevent cyclic inclusions
 */
#ifndef __SNOOPY_INPUTDATASTORAGE_H
#define __SNOOPY_INPUTDATASTORAGE_H



/*
 * Store execution data for inputs to consume
 */
typedef struct {
    int          initialized;
    const char  *filename;
    char       **argv;
    char       **envp;
} snoopy_inputdatastorage_t;



/*
 * Init functions
 */
void snoopy_inputdatastorage_ctor             ();
void snoopy_inputdatastorage_dtor             ();
void snoopy_inputdatastorage_setUninitialized (snoopy_inputdatastorage_t *IDS);
void snoopy_inputdatastorage_setDefaults      (snoopy_inputdatastorage_t *IDS);



/*
 * Functions that do the actual data storing
 */
void snoopy_inputdatastorage_store_filename (const char *filename);
void snoopy_inputdatastorage_store_argv     (      char *argv[]  );
void snoopy_inputdatastorage_store_envp     (      char *envp[]  );



/*
 * Retrieval functions
 */
snoopy_inputdatastorage_t*   snoopy_inputdatastorage_get ();



/*
 * END: Prevent cyclic inclusion
 */
#endif   /* Cyclic inclusion */
