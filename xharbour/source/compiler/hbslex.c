/*
 * $Id: hbslex.c,v 1.3 2003/01/28 02:25:23 what32 Exp $
 */

#include "hbcomp.h"
#include "harboury.h"
#include "hbsetup.h"
#include "hberrors.h"
#include "hbdefs.h"

#define MAX_STREAM                               2048 /* Max length of in-line LITERAL */
#define MAX_STREAM_STARTER                          7 /* "QOUT([" */
#define MAX_STREAM_TERMINATOR                       4 /* "])\n"   */
#define MAX_STREAM_EXCLUSIONS                       2

#define TOKEN_SIZE             ( 65 > HB_SYMBOL_NAME_LEN + 1 ? 65 : HB_SYMBOL_NAME_LEN + 1 )

#define YY_BUF_SIZE HB_PP_STR_SIZE

#define SLX_RULES "../source/compiler/harbour.slx"

#include "simplex.c"
