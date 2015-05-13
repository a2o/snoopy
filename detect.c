/* detect.c -- simple execve() wrapper detection 
 * Copyright (c) 2000 marius@linux.com,mbm@linux.com
 *
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

#include <stdio.h>
#include <dlfcn.h>

#if defined(RTLD_NEXT)
#  define REAL_LIBC RTLD_NEXT
#else
#  define REAL_LIBC ((void *) -1L)
#endif

int main(void) {
	void *handle = dlopen("/lib/libc.so.6", RTLD_LAZY);
	//simple test to see if the execve in memory matches libc.so.6
	if (dlsym(handle, "execve") != dlsym(REAL_LIBC, "execve"))
		printf("something fishy...\n");
	else
		printf("secure\n");
	return 0;
}
