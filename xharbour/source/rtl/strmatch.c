/*
 * $Id: strmatch.c,v 1.1.1.1 2001/12/21 10:42:06 ronpinkas Exp $
 */

/*
 * Harbour Project source code:
 * String matching functions
 *
 * Copyright 1999-2001 Viktor Szakats <viktor.szakats@syenar.hu>
 * www - http://www.harbour-project.org
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
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

#include <ctype.h>

#include "hbapi.h"

#include "regex.h"

static BOOL hb_strMatchDOS( const char * pszString, const char * pszMask )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_strMatchDOS(%s, %s)", pszString, pszMask));

   while( *pszMask != '\0' && *pszString != '\0' )
   {
      if( *pszMask == '*' )
      {
         while( *pszMask == '*' )
         {
            pszMask++;
         }

         if( *pszMask == '\0' )
         {
            return TRUE;
         }
         else if( *pszMask == '?' )
         {
            pszString++;
         }
         else
         {
            while( toupper( *pszString ) != toupper( *pszMask ) )
            {
               if( *( ++pszString ) == '\0' )
               {
                  return FALSE;
               }
            }

            while( toupper( *pszString ) == toupper( *pszMask ) )
            {
               if( *( ++pszString ) == '\0' )
               {
                  break;
               }
            }

            pszMask++;
         }
      }
      else if( toupper( *pszMask ) != toupper( *pszString ) && *pszMask != '?' )
      {
         return FALSE;
      }
      else
      {
         pszMask++;
         pszString++;
      }
   }

   return ! ( ( *pszMask != '\0' && *pszString == '\0' && *pszMask != '*') || ( *pszMask == '\0' && *pszString != '\0' ) );
}

BOOL hb_strMatchRegExp( const char * szString, const char * szMask )
{
   regex_t re;
   regmatch_t aMatches[1];
   int CFlags = REG_EXTENDED, EFlags = 0;

   HB_TRACE(HB_TR_DEBUG, ("hb_strMatchRegExp(%s, %s)", szString, szMask));

   if( regcomp( &re, szMask, CFlags ) == 0 )
   {
      if( regexec( &re, szString, 1, aMatches, EFlags ) == 0 )
      {
         return aMatches[0].rm_so == 0 && aMatches[0].rm_eo == strlen( szString );
      }
   }

   return FALSE;
}

