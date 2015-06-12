/*
 * SNOOPY LOGGER
 *
 * File: tsrm.h
 *
 * Copyright (c) 2015 Bostjan Skufca <bostjan@a2o.si>
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
#ifndef __SNOOPY_TSRM_H
#define __SNOOPY_TSRM_H



/*
 * Includes order: from local to global
 */
#include "configuration.h"
#include "inputdatastorage.h"

#include <pthread.h>



/*
 * Type alias: threadId
 */
typedef   pthread_t   snoopy_tsrm_threadId_t;

/*
 * Type: thread data struct
 */
typedef struct {
    snoopy_tsrm_threadId_t     threadId;
    snoopy_configuration_t    *configuration;
    snoopy_inputdatastorage_t *inputdatastorage;
} snoopy_tsrm_threadData_t;



/*
 * Init/shutdown functions
 */
void   snoopy_tsrm_ctor ();
void   snoopy_tsrm_dtor ();



/*
 * Getter functions
 */
snoopy_configuration_t*    snoopy_tsrm_get_configuration    ();
snoopy_inputdatastorage_t* snoopy_tsrm_get_inputdatastorage ();
int                        snoopy_tsrm_get_threadCount      ();



/*
 * END: Prevent cyclic inclusion
 */
#endif   /* Cyclic inclusion */
