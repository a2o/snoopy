/*
 * SNOOPY COMMAND LOGGER
 *
 * Copyright (c) 2022 Bostjan Skufca Jese (bostjan _A_T_ a2o.si)
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



#ifndef _GNU_SOURCE
#define _GNU_SOURCE // For getutline_r() in utmp.c
#endif
#include <stddef.h>
#include <utmp.h>



int  snoopy_util_utmp_findUtmpEntryByLine (char const * const ttyLine, struct utmp * const resultEntryBuf);
int  snoopy_util_utmp_findUtmpEntryByPath (char const * const ttyPath, struct utmp * const resultEntryBuf);

int  snoopy_util_utmp_doesEntryContainIpAddr (struct utmp const * const utmpEntry);
int  snoopy_util_utmp_getUtmpIpAddrAsString (struct utmp const * const utmpEntry, char * const resultBuf, size_t resultBufSize);

void snoopy_util_utmp_test_setAlternateUtmpFilePath (char const * const utmpPath);
