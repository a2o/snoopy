/* Snoopy 1.1
 *
 * $Id: snoopy.h,v 1.3 2000/08/19 02:25:31 marius Exp $
 * 
 */

/* ROOT_ONLY
 * log only the actions running under uid 0, set 1 to enable
 */
#define ROOT_ONLY 0

/* MAX_LINE_LEN
 * to make sure that snoopy doesn't just fill your syslog with garbage
 * we put a limit on how many characters per log entry snoopy can use
 */
#define MAX 512
