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



#define SNOOPY_UTIL_FILE__SMALL_FILE_MAX_SIZE   10240   // 10 KB
#define SNOOPY_UTIL_FILE__SMALL_FILE_FREAD_SIZE 1024   // Must divide the SMALL_FILX_MAX_SIZE, cannot be greater than 1024
#define SNOOPY_UTIL_FILE__ERROR_MSG_MAX_SIZE    1024



int snoopy_util_file_getSmallTextFileContent (const char * const filePath, char ** contentPtr);
