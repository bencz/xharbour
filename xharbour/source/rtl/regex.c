/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Regular Expressions Interface functions
 *
 * www - http://www.xharbour.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version, with one exception:
 *
 * The exception is that if you link the Harbour Runtime Library (HRL)
 * and/or the Harbour Virtual Machine (HVM) with other files to produce
 * an executable, this does not by itself cause the resulting executable
 * to be covered by the GNU General Public License. Your use of that
 * executable is in no way restricted on account of linking the HRL
 * and/or HVM code into it.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA (or visit
 * their web site at http://www.gnu.org/).
 *
 */

/*
 * 2002-Dec-13 Ron Pinkas added POSIX REG_STAREND support.
 */

/*
 * 2002-Dec-13 Ron Pinkas added xHarbour wrapper:
 *
 *    HB_FUNC( HB_ATX )
 *
 */

/*
 * 2003-Feb-3 Giancarlo Niccolai added the wrappers
 *
 *    HB_FUNC( HB_REGEX* )
 *
 */

#define HB_THREAD_OPTIMIZE_STACK

#include "hbdefs.h"
#include "hbstack.h"
#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"

#include "regex.h"

/*
 * It will be good to separate RegEx internals from xHarbour API
 * and define our own meta functions so C modules which use regex
 * will not have to include regex.h and we will be free to make
 * any modifications in internal structures (also use native
 * platform regex library which exists in most of *nixes), Druzus
 */
#if !defined( HB_ALLOC_ALIGNMENT ) || ( HB_ALLOC_ALIGNMENT + 1 == 1 )
#  define HB_ALLOC_ALIGNMENT     8
#endif

HB_EXPORT regex_t * hb_getregex( PHB_ITEM pRegEx, BOOL lIgnCase, BOOL lNL, BOOL *fFree )
{
   char * szRegEx = hb_itemGetCPtr( pRegEx );
   regex_t * pRetReg = NULL;

   *fFree = FALSE;
   if( szRegEx && *szRegEx )
   {
      if( memcmp( szRegEx, "***", 3 ) == 0 )
      {
         pRetReg = (regex_t *) ( szRegEx + HB_ALLOC_ALIGNMENT );
         pRetReg->re_pcre = (pcre *) ( ( BYTE * ) pRetReg + sizeof( regex_t ) );
      }
      else
      {
         int CFlags = REG_EXTENDED | ( lIgnCase ? REG_ICASE : 0 ) |
                      ( lNL ? REG_NEWLINE : 0 );

         pRetReg = ( regex_t * ) hb_xgrab( sizeof( regex_t ) );
         if( regcomp( pRetReg, szRegEx, CFlags ) == 0 )
         {
            *fFree = TRUE;
         }
         else
         {
            hb_xfree( pRetReg );
            pRetReg = NULL;
            hb_errRT_BASE_SubstR( EG_ARG, 3012, "Invalid Regular expression", "Regex subsystem", 1, pRegEx );
         }
      }
   }

   return pRetReg;
}

void HB_EXPORT hb_freeregex( regex_t *pReg )
{
   regfree( pReg );
   hb_xfree( pReg );
}

/*
 *  This is a worker function may be called by PRG Wrappers HB_REGEX and HB_REGEXMATCH
 *  in such case paramaters ara on the HB_BM_STACK, or it may be called directly by the
 *  HVM when executing the HB_P_MATCH and HB_P_LIKE operator. In such case the operands
 *  are passed directly.
 */
BOOL HB_EXPORT hb_regex( char cRequest, PHB_ITEM pRegEx, PHB_ITEM pString )
{
   HB_THREAD_STUB

   #ifndef REGEX_MAX_GROUPS
      #define REGEX_MAX_GROUPS 16
   #endif

   regex_t *pReg;
   regmatch_t aMatches[REGEX_MAX_GROUPS];
   int EFlags = 0;//REG_BACKR;
   int i, iMatches = 0;
   int iMaxMatch = REGEX_MAX_GROUPS;

   PHB_ITEM pCaseSensitive = hb_param( 3, HB_IT_LOGICAL );
   PHB_ITEM pNewLine = hb_param( 4, HB_IT_LOGICAL );
   BOOL fFree = FALSE;

   if( pRegEx == NULL )
   {
      pRegEx = hb_param( 1, HB_IT_STRING );
      pString = hb_param( 2, HB_IT_STRING );
   }

   if( pRegEx == NULL || pString == NULL ||
       !HB_IS_STRING( pRegEx ) || !HB_IS_STRING( pString ) )
   {
      hb_errRT_BASE_SubstR( EG_ARG, 3012, "Wrong parameters", "Regex subsystem", 4, pRegEx, pString, hb_paramError( 3 ), hb_paramError( 4 ) );
      return FALSE;
   }

   pReg = hb_getregex( pRegEx,
                       pCaseSensitive != NULL && !pCaseSensitive->item.asLogical.value,
                       pNewLine != NULL && pNewLine->item.asLogical.value,
                       &fFree );

   /* now check if it is a precompiled regex */
   if ( hb_isregexstring( pRegEx ) ) //( pRegEx->item.asString.length > 3 && memcmp( pRegEx->item.asString.value, "***", 3 ) == 0 )
   {
      aMatches[0].rm_eo = pRegEx->item.asString.length - 3 -
                          ((real_pcre *) pReg->re_pcre)->size;
   }
   else
   {
      aMatches[0].rm_eo = pRegEx->item.asString.length;
   }

   if( cRequest != 0 && cRequest != 4)
   {
      iMaxMatch = 1;
   }

   aMatches[0].rm_so = 0;

   if( regexec( pReg, pString->item.asString.value, iMaxMatch, aMatches, EFlags ) == 0 )
   {
      switch ( cRequest )
      {
         case 0: // Wants Results
         {
            // Count sucessful matches
            for ( i = 0; i < iMaxMatch; i ++ )
            {
               if ( aMatches[i].rm_eo != -1 )
               {
                  iMatches = i;
               }
            }

            iMatches++;
            hb_arrayNew( &(HB_VM_STACK.Return), iMatches );

            for ( i = 0; i < iMatches; i++ )
            {
               if (aMatches[i].rm_eo > -1 )
               {
                  hb_itemPutCL( hb_arrayGetItemPtr(&(HB_VM_STACK.Return), i + 1 ), pString->item.asString.value + aMatches[i].rm_so, aMatches[i].rm_eo - aMatches[i].rm_so );
               }
               else
               {
                  hb_itemPutCL( hb_arrayGetItemPtr(&(HB_VM_STACK.Return), i + 1 ), "",0 );
               }
            }

            if( fFree )
            {
               hb_freeregex( pReg );
            }

            return TRUE;
         }

         case 1: // HB_P_LIKE
            if( fFree )
            {
               //TraceLog( NULL, "%s like %s\n", pString->item.asString.value, pRegEx->item.asString.value );
               hb_freeregex( pReg );
            }

            return aMatches[0].rm_so == 0 && (ULONG) (aMatches[0].rm_eo) == pString->item.asString.length;

         case 2: // HB_P_MATCH
            if( fFree )
            {
               //TraceLog( NULL, ">%s< has >%s<\n", pString->item.asString.value, pRegEx->item.asString.value );
               hb_freeregex( pReg );
            }

            return TRUE;

         case 3: // Split
         {
            HB_ITEM Match;
            char *str = pString->item.asString.value;
            int iMax = hb_parni( 5 );
            int iCount = 1;

            hb_arrayNew( &(HB_VM_STACK.Return), 0 );

            Match.type = HB_IT_NIL;

            do
            {
               hb_itemPutCL( &Match, str, aMatches[0].rm_so );
               hb_arrayAddForward( &(HB_VM_STACK.Return), &Match );
               str += aMatches[ 0 ].rm_eo;
               iCount++;
            }
            while( *str && (iMax == 0 || iMax > iCount) && regexec( pReg, str, 1, aMatches, EFlags ) == 0 );

            /* last match must be done also in case that str is empty; this would
               mean an empty split field at the end of the string */
            hb_itemPutCL( &Match, str, strlen( str ) );
            hb_arrayAddForward( &(HB_VM_STACK.Return), &Match );
         }

         if( fFree )
         {
            hb_freeregex( pReg );
         }

         return TRUE;

         case 4: // Wants Results AND positions
         {
            HB_ITEM aSingleMatch;

            // Count sucessful matches
            for ( i = 0; i < iMaxMatch; i ++ )
            {
               if ( aMatches[i].rm_eo != -1 )
               {
                  iMatches = i;
               }
            }

            iMatches++;
            hb_arrayNew( &(HB_VM_STACK.Return), iMatches );

            for ( i = 0; i < iMatches; i++ )
            {
               aSingleMatch.type = HB_IT_NIL;

               if ( aMatches[i].rm_eo != -1 )
               {
                  hb_arrayNew( &aSingleMatch, 3 );

                  //matched string
                  hb_itemPutCL( hb_arrayGetItemPtr( &aSingleMatch, 1 ), pString->item.asString.value + aMatches[i].rm_so, aMatches[i].rm_eo - aMatches[i].rm_so );

                  // begin of match
                  hb_itemPutNI( hb_arrayGetItemPtr( &aSingleMatch, 2 ), aMatches[i].rm_so );

                  // End of match
                  hb_itemPutNI( hb_arrayGetItemPtr( &aSingleMatch, 3 ), aMatches[i].rm_eo );
               }
               else
               {
                  hb_itemPutCL( hb_arrayGetItemPtr( &aSingleMatch, 1 ), "", 0 );
                  hb_itemPutNI( hb_arrayGetItemPtr( &aSingleMatch, 2 ), 0 );
                  hb_itemPutNI( hb_arrayGetItemPtr( &aSingleMatch, 3 ), 0 );
               }

               hb_itemArrayPut( &(HB_VM_STACK.Return), i + 1, &aSingleMatch );
            }

            if ( fFree )
               hb_freeregex( pReg );
            return TRUE;

         }

      }
   }

   /* If we have no match, we must anyway return an array of one element
      for request kind == 3 (split) */
   if ( fFree )
   {
      hb_freeregex( pReg );
   }

   if( cRequest == 3 )
   {
      hb_arrayNew( &(HB_VM_STACK.Return), 0 );
      hb_arrayAdd( &(HB_VM_STACK.Return), pString );
      return TRUE;
   }

   return FALSE;
}

/*
 Converts OS Directory Wild Mask to an equivalent RegEx
 Caller must allocate sRegEx with enough space for conversion.
 returns the length of the resulting RegEx.
 */
HB_EXPORT int Wild2RegEx( char *sWild, char* sRegEx, BOOL bMatchCase )
{
   char cChar;
   int iLen = strlen( sWild );
   int i, iLenResult = 0;

   if( bMatchCase == FALSE )
   {
      strncpy( sRegEx, "(?i)", 4 );
      iLenResult = 4;
   }

   strncpy( sRegEx + iLenResult, "\\b", 2 );
   iLenResult += 2;

   for( i = 0; i < iLen; i++ )
   {
      cChar = sWild[i];

      switch( cChar )
      {
         case '*':
            // Space NOT allowed as 1st. character.
            if( i == 0 )
            {
               strncpy( sRegEx + iLenResult, "[^ ]", 4 );
               iLenResult += 4;
            }

            strncpy( sRegEx + iLenResult, ".*", 2 );
            iLenResult += 2;
            break;

         case '?':
            strncpy( sRegEx + iLenResult, ".?", 2 );
            iLenResult += 2;
            break;

         case '.':
            strncpy( sRegEx + iLenResult, "\\.", 2 );
            iLenResult += 2;
            break;

         default:
            sRegEx[ iLenResult++ ] = cChar;
      }
   }

   strncpy( sRegEx + iLenResult, "\\b", 2 );
   iLenResult += 2;

   sRegEx[ iLenResult ] = '\0';

   return iLenResult;
}

HB_FUNC( HB_ATX )
{
#ifdef HB_API_MACROS
   HB_THREAD_STUB
#endif
   #define REGEX_MAX_GROUPS 16
   regex_t *pReg ;
   regmatch_t aMatches[REGEX_MAX_GROUPS];
   int EFlags = 0;//REG_BACKR;
   ULONG ulLen;
   BOOL fFree;

   PHB_ITEM pRegEx = hb_param( 1, HB_IT_STRING );
   PHB_ITEM pString = hb_param( 2, HB_IT_STRING );
   PHB_ITEM pCaseSensitive = hb_param( 3, HB_IT_LOGICAL );
   ULONG ulStart = hb_parnl(4), ulEnd = hb_parnl(5);

   if( pRegEx && pString && ulStart <= pString->item.asString.length )
   {
      pReg = hb_getregex( pRegEx,
                          pCaseSensitive && ! pCaseSensitive->item.asLogical.value,
                          FALSE, &fFree );
      if( pReg )
      {
         if( ulStart || ulEnd )
         {
            EFlags |= REG_STARTEND;
            aMatches[0].rm_so = ulStart > 1 ? ulStart - 1 : 0;
            aMatches[0].rm_eo = ulEnd > 0 && ulEnd < pString->item.asString.length ? ulEnd : pString->item.asString.length;
         }

         if( regexec( pReg, pString->item.asString.value, REGEX_MAX_GROUPS, aMatches, EFlags ) == 0 )
         {
            // Very STARNGE bug if string is found at position 0 regex "forgets" to add the offset!!!
            if( ulStart && aMatches[0].rm_so == 0 )
            {
               aMatches[0].rm_so += ulStart - 1;
               aMatches[0].rm_eo += ulStart - 1;
            }

            ulLen = aMatches[0].rm_eo - aMatches[0].rm_so;

            if( hb_pcount() > 3 )
            {
               hb_stornl( aMatches[0].rm_so + 1, 4 );
            }

            if( hb_pcount() > 4 )
            {
               hb_stornl( aMatches[0].rm_eo - aMatches[0].rm_so, 5 );
            }

            hb_retclen( pString->item.asString.value + aMatches[0].rm_so, ulLen );

            if( fFree )
               hb_freeregex( pReg );

            return;
         }

         if( fFree )
            hb_freeregex( pReg );
      }
   }

   if( hb_pcount() > 3 )
   {
      hb_stornl( 0, 4 );
   }

   if( hb_pcount() > 4 )
   {
      hb_stornl( 0, 5 );
   }
}

HB_FUNC( WILD2REGEX )
{
#ifdef HB_API_MACROS
   HB_THREAD_STUB
#endif
   char sRegEx[ _POSIX_PATH_MAX ];
   int iLen = Wild2RegEx( hb_parcx( 1 ), sRegEx, hb_parl( 2 ) );

   hb_retclen( sRegEx, iLen );
}

// Returns array of Match + Sub-Matches.
HB_FUNC( HB_REGEX )
{
   hb_regex( 0, NULL, NULL );
}

// Returns array of { Match, start, end}, { Sub-Matches, start, end}
HB_FUNC( HB_REGEXATX )
{
   hb_regex( 4, NULL, NULL );
}

// Returns just .T. if match found or .F. otherwise.
HB_FUNC( HB_REGEXMATCH )
{
#ifdef HB_API_MACROS
   HB_THREAD_STUB
#endif
   hb_retl ( hb_regex( hb_parl(3) ? 1 : 2, NULL, NULL ) );
}

// Splits the string in an array of matched expressions
HB_FUNC( HB_REGEXSPLIT )
{
   hb_regex( 3, NULL, NULL );
}

HB_FUNC( HB_REGEXCOMP )
{
#ifdef HB_API_MACROS
   HB_THREAD_STUB
#endif

   regex_t re;
   char *cRegex;
   int CFlags = REG_EXTENDED;

   PHB_ITEM pRegEx = hb_param( 1, HB_IT_STRING );
   PHB_ITEM pCaseSensitive = hb_param( 2, HB_IT_LOGICAL );
   PHB_ITEM pNewLine = hb_param( 3, HB_IT_LOGICAL );

   if( pRegEx == NULL )
   {
      hb_errRT_BASE_SubstR( EG_ARG, 3012, "Wrong parameter count/type",
         NULL,
         2, hb_param( 1, HB_IT_ANY ), hb_param( 2, HB_IT_ANY ));
      return;
   }

   if( pCaseSensitive != NULL && pCaseSensitive->item.asLogical.value == ( int )FALSE )
   {
      CFlags |= REG_ICASE;
   }

   if( pNewLine != NULL && pNewLine->item.asLogical.value == ( int )TRUE )
   {
      CFlags |= REG_NEWLINE;
   }

   if( regcomp( &re, pRegEx->item.asString.value, CFlags ) == 0 )
   {
      ULONG nSize = ((real_pcre *) re.re_pcre)->size;
      cRegex = (char *) hb_xgrab( HB_ALLOC_ALIGNMENT + sizeof( re ) + nSize );
      memcpy( cRegex + HB_ALLOC_ALIGNMENT + sizeof( re ), re.re_pcre, nSize );
      (pcre_free)(re.re_pcre);
      re.re_pcre = (pcre *) ( cRegex + HB_ALLOC_ALIGNMENT + sizeof( re ) );
      memcpy( cRegex, "***", 3 );
      memcpy( cRegex + HB_ALLOC_ALIGNMENT, &re, sizeof( re ) );
      hb_retclenAdoptRaw( cRegex, HB_ALLOC_ALIGNMENT + sizeof( re ) + nSize );
   }
   else {
      hb_errRT_BASE_SubstR( EG_ARG, 3012, "Invalid Regular expression",
      "Regex subsystem",
      3, pRegEx, hb_paramError( 3 ), hb_paramError( 4 ));
   }
}

HB_FUNC( HB_ISREGEXSTRING )
{
#ifdef HB_API_MACROS
   HB_THREAD_STUB
#endif
   PHB_ITEM pRegEx = hb_param( 1, HB_IT_STRING );
  hb_retl( pRegEx && hb_isregexstring( pRegEx ) ) ;
}
