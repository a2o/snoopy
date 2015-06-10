#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif


// do not try to be smart and make this go away on NDEBUG, the _debug
// here means that it just doesn't print a message, it still does the
// check.  MKAY?
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#ifdef NO_LINENOS
// versions that don't feature line numbers
#define log_err(M, ...) fprintf(stderr, "[ERROR] (errno: %s) " M "\n", clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (errno: %s) " M "\n", clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] " M "\n", ##__VA_ARGS__)
#else
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.%s", "")

#define TRACE(C,E) debug("--> %s(%s:%d) %s:%d ", "" #C, State_event_name(E), E, __FUNCTION__, __LINE__)

#define error_response(F, C, M, ...)  {Response_send_status(F, &HTTP_##C); sentinel(M, ##__VA_ARGS__);}

#define error_unless(T, F, C, M, ...) if(!(T)) error_response(F, C, M, ##__VA_ARGS__)


#endif
