/* Snoopy 1.1
 *
 * $Id: snoopy.h,v 1.10 2000/12/21 06:53:03 marius Exp $
 * 
 */

/* ROOT_ONLY
 * log only the actions running under uid 0, set 1 to enable
 */
#define ROOT_ONLY 0

/* MAX
 * maximum size of any argument.  if set to 0, snoopy will ensure that all
 * arguments get logged to its full lenght; this is also a slower process
 */
#define MAX 32

/* INTEGRITY_CHECK
 * adds an integrity check to snoopy, should be unnecessary, but is here as an
 * extreme precaution.  performance killer, don't turn on unless you think
 * you're having problems / are debugging
 */
#define INTEGRITY_CHECK 0
