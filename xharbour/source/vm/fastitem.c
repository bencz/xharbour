/*
 * $Id: fastitem.c,v 1.0 2001/12/22 06:36:17 ronpinkas Exp $
 */

/*
 * xHarbour Project source code:
 * The FastItem Optimization API
 *
 * Copyright 2001 Ron Pinkas <ron@@ronpinkas.com>
 * www - http://www.xharbour.org
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
 * As a special exception, xHarbour license gives permission for
 * additional uses of the text contained in its release of xHarbour.
 *
 * The exception is that, if you link the xHarbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the xHarbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released with this xHarbour
 * explicit exception.  If you add/copy code from other sources,
 * as the General Public License permits, the above exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for xHarbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 */

#include "hbapi.h"
#include "hbstack.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbdate.h"
#include "hbset.h"

void hb_itemShareValue( PHB_ITEM pDest, PHB_ITEM pSource )
{
   HB_TRACE( HB_TR_INFO, ("*** hb_itemShareValue(%p, %p)", pDest, pSource ) );

   if( pDest->type )
   {
      hb_itemClear( pDest );
   }

   if( pDest == pSource )
   {
      hb_errInternal( HB_EI_ITEMBADCOPY, NULL, "hb_itemShareValue()", NULL );
   }

   memcpy( pDest, pSource, sizeof( HB_ITEM ) );

   pDest->bShadow = TRUE;
}

void hb_itemForwardValue( PHB_ITEM pDest, PHB_ITEM pSource )
{
   HB_TRACE( HB_TR_DEBUG, ("hb_itemForwardValue(%p, %p)", pDest, pSource ) );

   if( pDest->type )
   {
      hb_itemClear( pDest );
   }

   if( pDest == pSource )
   {
      hb_errInternal( HB_EI_ITEMBADCOPY, NULL, "hb_itemShareValue()", NULL );
   }

   memcpy( pDest, pSource, sizeof( HB_ITEM ) );

   pSource->bShadow = TRUE;
}

void hb_itemPushEnvelopeString( char * szText, ULONG length )
{
   HB_TRACE(HB_TR_DEBUG, ( "hb_itemPushEnvelopeString( \"%s\", %lu )", szText, length ) );

   ( hb_stackTopItem() )->bShadow = TRUE;

   ( hb_stackTopItem() )->type = HB_IT_STRING;
   ( hb_stackTopItem() )->item.asString.length = length;
   ( hb_stackTopItem() )->item.asString.value = szText;

   hb_stackPush();
}

void hb_retcAdopt( char * szText )
{
   HB_TRACE( HB_TR_INFO, ("hb_retcAdopt(%s)", szText ) );

   hb_itemClear( &hb_stack.Return );

   ( &hb_stack.Return )->type = HB_IT_STRING;
   ( &hb_stack.Return )->item.asString.value = szText;
   ( &hb_stack.Return )->item.asString.length = strlen( szText );
}

void hb_retclenAdopt( char * szText, ULONG ulLen )
{
   szText[ulLen] = '\0';

   HB_TRACE( HB_TR_INFO, ("hb_retclenAdopt( %p, %lu ) \"%s\"", szText, ulLen, szText ) );

   hb_itemClear( &hb_stack.Return );

   ( &hb_stack.Return )->type = HB_IT_STRING;
   ( &hb_stack.Return )->item.asString.value = szText;
   ( &hb_stack.Return )->item.asString.length = ulLen;
}
