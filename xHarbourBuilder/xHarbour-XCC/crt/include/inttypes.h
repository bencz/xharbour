#ifndef _INTTYPES_H
#define _INTTYPES_H

/* inttypes.h - C99 standard header */

#include <stdint.h>

#ifndef _WINCE

/* fprintf formats for signed integers */
#define PRId8        "hhd"
#define PRId16       "hd"
#define PRId32       "ld"
#define PRId64       "lld"
#define PRIdLEAST8   "hhd"
#define PRIdLEAST16  "hd"
#define PRIdLEAST32  "ld"
#define PRIdLEAST64  "lld"
#define PRIdFAST8    "hhd"
#define PRIdFAST16   "hd"
#define PRIdFAST32   "ld"
#define PRIdFAST64   "lld"
#define PRIdMAX      "lld"
#define PRIdPTR      "lld"

#define PRIi8        "hhi"
#define PRIi16       "hi"
#define PRIi32       "li"
#define PRIi64       "lli"
#define PRIiLEAST8   "hhi"
#define PRIiLEAST16  "hi"
#define PRIiLEAST32  "li"
#define PRIiLEAST64  "lli"
#define PRIiFAST8    "hhi"
#define PRIiFAST16   "hi"
#define PRIiFAST32   "li"
#define PRIiFAST64   "lli"
#define PRIiMAX      "lli"
#define PRIiPTR      "lli"

/* fprintf formats for unsigned integers */
#define PRIo8        "hho"
#define PRIo16       "ho"
#define PRIo32       "lo"
#define PRIo64       "llo"
#define PRIoLEAST8   "hho"
#define PRIoLEAST16  "ho"
#define PRIoLEAST32  "lo"
#define PRIoLEAST64  "llo"
#define PRIoFAST8    "hho"
#define PRIoFAST16   "ho"
#define PRIoFAST32   "lo"
#define PRIoFAST64   "llo"
#define PRIoMAX      "llo"
#define PRIoPTR      "llo"

#define PRIu8        "hhu"
#define PRIu16       "hu"
#define PRIu32       "lu"
#define PRIu64       "llu"
#define PRIuLEAST8   "hhu"
#define PRIuLEAST16  "hu"
#define PRIuLEAST32  "lu"
#define PRIuLEAST64  "llu"
#define PRIuFAST8    "hhu"
#define PRIuFAST16   "hu"
#define PRIuFAST32   "lu"
#define PRIuFAST64   "llu"
#define PRIuMAX      "llu"
#define PRIuPTR      "llu"

#define PRIx8        "hhx"
#define PRIx16       "hx"
#define PRIx32       "lx"
#define PRIx64       "llx"
#define PRIxLEAST8   "hhx"
#define PRIxLEAST16  "hx"
#define PRIxLEAST32  "lx"
#define PRIxLEAST64  "llx"
#define PRIxFAST8    "hhx"
#define PRIxFAST16   "hx"
#define PRIxFAST32   "lx"
#define PRIxFAST64   "llx"
#define PRIxMAX      "llx"
#define PRIxPTR      "llx"

#define PRIX8        "hhX"
#define PRIX16       "hX"
#define PRIX32       "lX"
#define PRIX64       "llX"
#define PRIXLEAST8   "hhX"
#define PRIXLEAST16  "hX"
#define PRIXLEAST32  "lX"
#define PRIXLEAST64  "llX"
#define PRIXFAST8    "hhX"
#define PRIXFAST16   "hX"
#define PRIXFAST32   "lX"
#define PRIXFAST64   "llX"
#define PRIXMAX      "llX"
#define PRIXPTR      "llX"

/* fscanf formats for signed integers */
#define SCNd8        "hhd"
#define SCNd16       "hd"
#define SCNd32       "ld"
#define SCNd64       "lld"
#define SCNdLEAST8   "hhd"
#define SCNdLEAST16  "hd"
#define SCNdLEAST32  "ld"
#define SCNdLEAST64  "lld"
#define SCNdFAST8    "hhd"
#define SCNdFAST16   "hd"
#define SCNdFAST32   "ld"
#define SCNdFAST64   "lld"
#define SCNdMAX      "lld"
#define SCNdPTR      "lld"

#define SCNi8        "hhi"
#define SCNi16       "hi"
#define SCNi32       "li"
#define SCNi64       "lli"
#define SCNiLEAST8   "hhi"
#define SCNiLEAST16  "hi"
#define SCNiLEAST32  "li"
#define SCNiLEAST64  "lli"
#define SCNiFAST8    "hhi"
#define SCNiFAST16   "hi"
#define SCNiFAST32   "li"
#define SCNiFAST64   "lli"
#define SCNiMAX      "lli"
#define SCNiPTR      "lli"

/* fscanf formats for unsigned integers */
#define SCNo8        "hho"
#define SCNo16       "ho"
#define SCNo32       "lo"
#define SCNo64       "llo"
#define SCNoLEAST8   "hho"
#define SCNoLEAST16  "ho"
#define SCNoLEAST32  "lo"
#define SCNoLEAST64  "llo"
#define SCNoFAST8    "hho"
#define SCNoFAST16   "ho"
#define SCNoFAST32   "lo"
#define SCNoFAST64   "llo"
#define SCNoMAX      "llo"
#define SCNoPTR      "llo"

#define SCNu8        "hhu"
#define SCNu16       "hu"
#define SCNu32       "lu"
#define SCNu64       "llu"
#define SCNuLEAST8   "hhu"
#define SCNuLEAST16  "hu"
#define SCNuLEAST32  "lu"
#define SCNuLEAST64  "llu"
#define SCNuFAST8    "hhu"
#define SCNuFAST16   "hu"
#define SCNuFAST32   "lu"
#define SCNuFAST64   "llu"
#define SCNuMAX      "llu"
#define SCNuPTR      "llu"

#define SCNx8        "hhx"
#define SCNx16       "hx"
#define SCNx32       "lx"
#define SCNx64       "llx"
#define SCNxLEAST8   "hhx"
#define SCNxLEAST16  "hx"
#define SCNxLEAST32  "lx"
#define SCNxLEAST64  "llx"
#define SCNxFAST8    "hhx"
#define SCNxFAST16   "hx"
#define SCNxFAST32   "lx"
#define SCNxFAST64   "llx"
#define SCNxMAX      "llx"
#define SCNxPTR      "llx"

typedef struct {
    intmax_t quot;
    intmax_t rem;
} imaxdiv_t;

/* conversion functions for greatest-width integer types */
intmax_t __cdecl imaxabs(intmax_t);
imaxdiv_t __cdecl imaxdiv(intmax_t, intmax_t);
intmax_t __cdecl strtoimax(const char * restrict, char ** restrict, int);
uintmax_t __cdecl strtoumax(const char * restrict, char ** restrict, int);
intmax_t __cdecl wcstoimax(const wchar_t * restrict, wchar_t ** restrict, int);
uintmax_t __cdecl wcstoumax(const wchar_t * restrict, wchar_t ** restrict, int);

#else /* _WINCE */

/* fprintf formats for signed integers */
#define PRId8        "d"
#define PRId16       "d"
#define PRId32       "d"
#define PRId64       "ld"
#define PRIdLEAST8   "d"
#define PRIdLEAST16  "d"
#define PRIdLEAST32  "d"
#define PRIdLEAST64  "ld"
#define PRIdFAST8    "d"
#define PRIdFAST16   "d"
#define PRIdFAST32   "d"
#define PRIdFAST64   "ld"
#define PRIdMAX      "ld"
#define PRIdPTR      "ld"

#define PRIi8        "i"
#define PRIi16       "i"
#define PRIi32       "i"
#define PRIi64       "li"
#define PRIiLEAST8   "i"
#define PRIiLEAST16  "i"
#define PRIiLEAST32  "i"
#define PRIiLEAST64  "li"
#define PRIiFAST8    "i"
#define PRIiFAST16   "i"
#define PRIiFAST32   "i"
#define PRIiFAST64   "li"
#define PRIiMAX      "li"
#define PRIiPTR      "li"

/* fprintf formats for unsigned integers */
#define PRIo8        "o"
#define PRIo16       "o"
#define PRIo32       "o"
#define PRIo64       "lo"
#define PRIoLEAST8   "o"
#define PRIoLEAST16  "o"
#define PRIoLEAST32  "o"
#define PRIoLEAST64  "lo"
#define PRIoFAST8    "o"
#define PRIoFAST16   "o"
#define PRIoFAST32   "o"
#define PRIoFAST64   "lo"
#define PRIoMAX      "lo"
#define PRIoPTR      "lo"

#define PRIu8        "u"
#define PRIu16       "u"
#define PRIu32       "u"
#define PRIu64       "lu"
#define PRIuLEAST8   "u"
#define PRIuLEAST16  "u"
#define PRIuLEAST32  "u"
#define PRIuLEAST64  "lu"
#define PRIuFAST8    "u"
#define PRIuFAST16   "u"
#define PRIuFAST32   "u"
#define PRIuFAST64   "lu"
#define PRIuMAX      "lu"
#define PRIuPTR      "lu"

#define PRIx8        "x"
#define PRIx16       "x"
#define PRIx32       "x"
#define PRIx64       "lx"
#define PRIxLEAST8   "x"
#define PRIxLEAST16  "x"
#define PRIxLEAST32  "x"
#define PRIxLEAST64  "lx"
#define PRIxFAST8    "x"
#define PRIxFAST16   "x"
#define PRIxFAST32   "x"
#define PRIxFAST64   "lx"
#define PRIxMAX      "lx"
#define PRIxPTR      "lx"

#define PRIX8        "X"
#define PRIX16       "X"
#define PRIX32       "X"
#define PRIX64       "lX"
#define PRIXLEAST8   "X"
#define PRIXLEAST16  "X"
#define PRIXLEAST32  "X"
#define PRIXLEAST64  "lX"
#define PRIXFAST8    "X"
#define PRIXFAST16   "X"
#define PRIXFAST32   "X"
#define PRIXFAST64   "lX"
#define PRIXMAX      "lX"
#define PRIXPTR      "lX"

/* fscanf formats for signed integers */
#define SCNd8        "d"
#define SCNd16       "d"
#define SCNd32       "d"
#define SCNd64       "ld"
#define SCNdLEAST8   "d"
#define SCNdLEAST16  "d"
#define SCNdLEAST32  "d"
#define SCNdLEAST64  "ld"
#define SCNdFAST8    "d"
#define SCNdFAST16   "d"
#define SCNdFAST32   "d"
#define SCNdFAST64   "ld"
#define SCNdMAX      "ld"
#define SCNdPTR      "ld"

#define SCNi8        "i"
#define SCNi16       "i"
#define SCNi32       "i"
#define SCNi64       "li"
#define SCNiLEAST8   "i"
#define SCNiLEAST16  "i"
#define SCNiLEAST32  "i"
#define SCNiLEAST64  "li"
#define SCNiFAST8    "i"
#define SCNiFAST16   "i"
#define SCNiFAST32   "i"
#define SCNiFAST64   "li"
#define SCNiMAX      "li"
#define SCNiPTR      "li"

/* fscanf formats for unsigned integers */
#define SCNo8        "o"
#define SCNo16       "o"
#define SCNo32       "o"
#define SCNo64       "lo"
#define SCNoLEAST8   "o"
#define SCNoLEAST16  "o"
#define SCNoLEAST32  "o"
#define SCNoLEAST64  "lo"
#define SCNoFAST8    "o"
#define SCNoFAST16   "o"
#define SCNoFAST32   "o"
#define SCNoFAST64   "lo"
#define SCNoMAX      "lo"
#define SCNoPTR      "lo"

#define SCNu8        "u"
#define SCNu16       "u"
#define SCNu32       "u"
#define SCNu64       "lu"
#define SCNuLEAST8   "u"
#define SCNuLEAST16  "u"
#define SCNuLEAST32  "u"
#define SCNuLEAST64  "lu"
#define SCNuFAST8    "u"
#define SCNuFAST16   "u"
#define SCNuFAST32   "u"
#define SCNuFAST64   "lu"
#define SCNuMAX      "lu"
#define SCNuPTR      "lu"

#define SCNx8        "x"
#define SCNx16       "x"
#define SCNx32       "x"
#define SCNx64       "lx"
#define SCNxLEAST8   "x"
#define SCNxLEAST16  "x"
#define SCNxLEAST32  "x"
#define SCNxLEAST64  "lx"
#define SCNxFAST8    "x"
#define SCNxFAST16   "x"
#define SCNxFAST32   "x"
#define SCNxFAST64   "lx"
#define SCNxMAX      "lx"
#define SCNxPTR      "lx"

/* conversion functions for greatest-width integer types */
#define imaxdiv_t  ldiv_t
#define imaxabs  labs
#define imaxdiv  ldiv
#define strtoimax  strtol
#define strtoumax  strtoul
#define wcstoimax  wcstol
#define wcstoumax  wcstoul

#endif /* _WINCE */

#endif /* _INTTYPES_H */
