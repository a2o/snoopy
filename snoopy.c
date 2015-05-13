/* snoopy.c -- execve() logging wrapper 
 * Copyright (c) 2000 marius@linux.com,mbm@linux.com
 * Version 1.3
 *
 * $Id: snoopy.c,v 1.32 2000/12/21 06:53:03 marius Exp $
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

#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <syslog.h>
#include "snoopy.h"

#define min(a,b) a<b?a:b

#if defined(RTLD_NEXT)
#  define REAL_LIBC RTLD_NEXT
#else
#  define REAL_LIBC ((void *) -1L)
#endif

#define FN(ptr,type,name,args)  ptr = (type (*)args)dlsym (REAL_LIBC, name)

inline void log(const char *filename, char **argv) {
	static char *logstring=NULL; 
	static int argc, size=0;
	register int i, spos=0;
	#if INTEGRITY_CHECK
	static char **argv_copy;
	static int *t_size;
	#endif

	#if ROOT_ONLY
	if (getuid() != 0) 
	   	return; 
	#endif

	#if IGNORE_NULL
	if (getlogin() == 0)
		return;
	#endif

	for(argc=0; *(argv+argc)!='\0';argc++);

	#if INTEGRITY_CHECK
	argv_copy = (char**)malloc(sizeof(char*)*argc);
	t_size = (int*)malloc(sizeof(int)*argc);
	for(i=0; i<argc; i++) {
	    size = sizeof(char)*strlen(*(argv+i));
		*(t_size+i) = size;
		*(argv_copy+i) = (char*)malloc(size);
		memcpy(*(argv_copy+i), *(argv+i), size);
	}
	*(argv_copy+argc) = '\0';
	size=0;
	#endif

	openlog("snoopy", LOG_PID, LOG_AUTHPRIV);

	#if MAX
	logstring = (char *)malloc(sizeof(char)*MAX*argc);
	for(i=0; i<argc; i++) 
	   spos += min(snprintf(logstring+spos, MAX, "%s ", *(argv+i)), MAX);
	#else
	for(i=0; i<argc; i++)
	    size += sizeof(char)*strlen(*(argv+i))+1;
	size++; /*make space for that \0*/
	logstring = (char*)malloc(sizeof(char)*size);

	for(i=0; i<argc; i++)
	   spos += sprintf(logstring+spos, "%s ", *(argv+i));
	#endif

	#if INTEGRITY_CHECK
	for(i=0; i<argc; i++)
		if(memcmp(*(argv+i), *(argv_copy+i), *(t_size+i)))
		   syslog(LOG_ERR, "Integrity check failed");

	for(i=0; i<argc; i++)
	   free(*(argv_copy+i));
	#endif

	syslog(LOG_INFO, "[%s, uid:%d sid:%d]: %s", getlogin(), getuid(), getsid(0), logstring); 
	free(logstring);

}

int execve(const char *filename, char **argv, char **envp) {
	static int (*func)(const char *, char **, char **);

	FN(func,int,"execve",(const char *, char **, char **));
	log(filename, argv);

	return (*func) (filename, argv, envp);
}

int execv(const char *filename, char **argv) {
	static int (*func)(const char *, char **);

	FN(func,int,"execv",(const char *, char **));
	log(filename, argv);

	return (*func) (filename, argv);
}
