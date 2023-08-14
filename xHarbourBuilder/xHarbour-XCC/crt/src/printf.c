/****************************************************************************
 *                                                                          *
 * File    : printf.c                                                       *
 *                                                                          *
 * Purpose : printf function.                                               *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00-09-15  Created                                              *
 *           01-09-02  Added _Lockfileatomic() and _Unlockfileatomic().     *
 *                                                                          *
 ****************************************************************************/

#include "xstdio.h"

/* write to file */
static void *prout(void *str, const char *buf, size_t n)
{
    return (fwrite(buf, 1, n, (FILE *)str) == n) ? str : 0;
}

/* print formatted to stdout */
int __cdecl (printf)(const char * restrict fmt, ...)
{
    int ans;
    va_list ap;

    va_start(ap, fmt);
    _Lockfileatomic(stdout);
    ans = __printf(&prout, stdout, fmt, ap);
    _Unlockfileatomic(stdout);
    va_end(ap);

    fflush(stdout);  /* added by PO */

    return ans;
}

