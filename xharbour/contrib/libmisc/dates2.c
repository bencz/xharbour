/*
 * $Id: dates2.c,v 1.1 2003/04/14 16:09:12 lculik Exp $
 */

/*
 * Harbour Project source code:
 * Additional date functions
 *
 * Copyright 1999 Jose Lalin <dezac@corevia.com>
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

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 1999 Jon Berg <jmberg@pnh10.med.navy.mil>
 *    DateTime()
 *
 * See doc/license.txt for licensing terms.
 *
 */

#include <ctype.h>
#include <time.h>

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapilng.h"
#include "hbdate.h"

static int s_daysinmonth[ 12 ] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

BOOL hb_isleapyear( long lYear )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_isleapyear(%ld)", lYear));

   return ( lYear % 4 == 0 && lYear % 100 != 0 ) || ( lYear % 400 == 0 );
}

long hb_daysinmonth( long lYear, long lMonth )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_daysinmonth(%ld, %ld)", lYear, lMonth));

   if( lMonth > 0 && lMonth < 13 )
      return s_daysinmonth[ lMonth - 1 ] + 
             ( ( lMonth == 2 && hb_isleapyear( lYear ) ) ? 1 : 0 );
   else
      return 0;
}

long hb_doy( long lYear, long lMonth, long lDay )
{
   int i;
   int iDoy = 0;

   HB_TRACE(HB_TR_DEBUG, ("hb_doy(%ld, %ld, %ld)", lYear, lMonth, lDay));

   for( i = 1; i < lMonth; i++ )
      iDoy += hb_daysinmonth( lYear, i );

   return iDoy + lDay;
}

long hb_wom( long lYear, long lMonth, long lDay )
{
   int iWom;

   HB_TRACE(HB_TR_DEBUG, ("hb_wom(%ld, %ld, %ld)", lYear, lMonth, lDay));

   iWom = lDay + hb_dateDOW( lYear, lMonth, 1 ) - 1;
   if( iWom > 0 )
      return ( iWom - hb_dateDOW( lYear, lMonth, lDay ) ) / 7 + 1;
   else
      return 0;
}

long hb_woy( long lYear, long lMonth, long lDay, BOOL bISO )
{
   int iWeek, n;

   HB_TRACE(HB_TR_DEBUG, ("hb_woy(%ld, %ld, %ld, %d)", lYear, lMonth, lDay, (int) bISO));

   lDay = hb_doy( lYear, lMonth, lDay );
   n = ( ( ( 1 - ( bISO ? 1 : 0 ) ) % 7 ) ) - 1;
   lDay += ( n > 0 ) ? 1 : 0;
   iWeek = lDay / 7;
   if( bISO )
      iWeek += ( n < 4 ) ? 1 : 0;
   else
      ++iWeek;

   return iWeek;
}

HB_FUNC( AMONTHS )
{
   HB_ITEM Return, Tmp;
   int i;

   Tmp.type = HB_IT_NIL;
   Return.type = HB_IT_NIL;
   hb_arrayNew( &Return, 0 );

   for( i = 0; i < 12; i++ )
   {
      hb_arrayAddForward( &Return, hb_itemPutC( &Tmp, ( char * ) hb_langDGetItem( HB_LANG_ITEM_BASE_MONTH + i ) ) );
   }

   hb_itemReturn( &Return );
}

HB_FUNC( ADAYS )
{
   HB_ITEM Return, Tmp;
   int i;

   Tmp.type = HB_IT_NIL;
   Return.type = HB_IT_NIL;

   hb_arrayNew( &Return, 0 );

   for( i = 0; i < 7; i++ )
   {
      hb_arrayAddForward( &Return, hb_itemPutC( &Tmp, ( char * ) hb_langDGetItem( HB_LANG_ITEM_BASE_DAY + i ) ));
   }

   hb_itemReturn( &Return );
}

HB_FUNC( ISLEAPYEAR )
{
   PHB_ITEM pDate = hb_param( 1, HB_IT_DATE );

   if( pDate )
   {
      long lYear, lMonth, lDay;

      hb_dateDecode( hb_itemGetDL( pDate ), &lYear, &lMonth, &lDay );
      hb_retl( hb_isleapyear( lYear ) );
   }
   else
      hb_retl( FALSE );
}

HB_FUNC( DAYSINMONTH )
{
   PHB_ITEM pDate = hb_param( 1, HB_IT_DATE );

   if( pDate )
   {
      long lYear, lMonth, lDay;

      hb_dateDecode( hb_itemGetDL( pDate ), &lYear, &lMonth, &lDay );
      hb_retni( hb_daysinmonth( lYear, lMonth ) );
   }
   else
      hb_retni( 0 );
}

HB_FUNC( EOM )
{
   PHB_ITEM pDate = hb_param( 1, HB_IT_DATE );

   if( pDate )
   {
      long lYear, lMonth, lDay;

      hb_dateDecode( hb_itemGetDL( pDate ), &lYear, &lMonth, &lDay );
      hb_retd( lYear, lMonth, hb_daysinmonth( lYear, lMonth ) );
   }
   else
      hb_retdl( 0 );
}

HB_FUNC( BOM )
{
   PHB_ITEM pDate = hb_param( 1, HB_IT_DATE );

   if( pDate )
   {
      long lYear, lMonth, lDay;

      hb_dateDecode( hb_itemGetDL( pDate ), &lYear, &lMonth, &lDay );
      hb_retd( lYear, lMonth, 1 );
   }
   else
      hb_retdl( 0 );
}

HB_FUNC( WOM )
{
   PHB_ITEM pDate = hb_param( 1, HB_IT_DATE );

   if( pDate )
   {
      long lYear, lMonth, lDay;

      hb_dateDecode( hb_itemGetDL( pDate ), &lYear, &lMonth, &lDay );
      hb_retni( hb_wom( lYear, lMonth, lDay ) );
   }
   else
      hb_retni( 0 );
}

HB_FUNC( DOY )
{
   PHB_ITEM pDate = hb_param( 1, HB_IT_DATE );

   if( pDate )
   {
      long lYear, lMonth, lDay;

      hb_dateDecode( hb_itemGetDL( pDate ), &lYear, &lMonth, &lDay );
      hb_retni( hb_doy( lYear, lMonth, lDay ) );
   }
   else
      hb_retni( 0 );
}

/* Return the nWeek of the year (1 - 52, 0 - 52 if ISO) */

HB_FUNC( WOY )
{
   PHB_ITEM pDate = hb_param( 1, HB_IT_DATE );

   if( pDate )
   {
      long lYear, lMonth, lDay;

      hb_dateDecode( hb_itemGetDL( pDate ), &lYear, &lMonth, &lDay );
      hb_retni( hb_woy( lYear, lMonth, lDay, ISLOG( 2 ) ? hb_parl( 2 ) : TRUE ) );
   }
   else
      hb_retni( 0 );
}

HB_FUNC( EOY )
{
   PHB_ITEM pDate = hb_param( 1, HB_IT_DATE );

   if( pDate )
   {
      long lYear, lMonth, lDay;

      hb_dateDecode( hb_itemGetDL( pDate ), &lYear, &lMonth, &lDay );
      hb_retd( lYear, 12, 31 );
   }
   else
      hb_retdl( 0 );
}

HB_FUNC( BOY )
{
   PHB_ITEM pDate = hb_param( 1, HB_IT_DATE );

   if( pDate )
   {
      long lYear, lMonth, lDay;

      hb_dateDecode( hb_itemGetDL( pDate ), &lYear, &lMonth, &lDay );
      hb_retd( lYear, 1, 1 );
   }
   else
      hb_retdl( 0 );
}

HB_FUNC( DATETIME )
{
   time_t current_time;

   time( &current_time );

   hb_retc( ctime( &current_time ) );
}
