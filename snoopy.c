/* snoopy.c -- execve() logging wrapper 
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

#include <dlfcn.h>
#include <stdio.h>
#include <syslog.h>
#include "snoopy.h"

#if defined(RTLD_NEXT)
#  define REAL_LIBC RTLD_NEXT
#else
#  define REAL_LIBC ((void *) -1L)
#endif

#define FN(ptr,type,name,args)  ptr = (type (*)args)dlsym (REAL_LIBC, name)

int execve(const char *filename, char **argv, char **envp) {
	static int (*func)(const char *, char **, char **);
	static int (*guid)(void);
	static char **ptr, *logstring; 
	static int size = MAX;

	FN(func,int,"execve",(const char *, char **, char **));
	FN(guid,int,"getuid",(void));

	ptr       = (char **)&argv[1];
	logstring = (char *)malloc((size_t *)size);
	
#if ROOT_ONLY
if ((*guid)() != 0) return (*func) (filename, argv, envp);
#endif
	
	openlog("snoopy", LOG_PID, LOG_AUTHPRIV);
	
	size -= snprintf(logstring, size,"[%s, uid:%d sid:%d]: %s",
			getlogin(), (*guid)(), getsid(0), filename);

	while (*ptr && size > 0) 
		size -= snprintf((logstring+MAX-size), size," %s",&(**ptr++));

	syslog(LOG_INFO, "%s", logstring);	
	free(logstring);
	closelog();
	
	return (*func) (filename, argv, envp);
}
