/*
 * $Id: valtype.c,v 1.4 2003/10/24 04:40:37 walito Exp $
 */

/*
 * Harbour Project source code:
 * VALTYPE() function
 *
 * Copyright 1999 Antonio Linares <alinares@fivetech.com>
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
 * Copyright 2002-2003 Walter Negro <anegro@overnet.com.ar>
 *    HB_ISBYREF()
 *    HB_ISNIL()
 *    HB_ISCHAR()
 *    HB_ISMEMO()
 *    HB_ISNUM()
 *    HB_ISLOGIC()
 *    HB_ISDATE()
 *    HB_ISARRAY()
 *    HB_ISOBJECT()
 *    HB_ISBLOCK()
 *    HB_ISPOINTER()
 *
 * See doc/license.txt for licensing terms.
 *
 */


#include "hbapi.h"
#include "hbapiitm.h"
#include "hbstack.h"

HB_FUNC( VALTYPE )
{
   hb_retc( hb_itemTypeStr( hb_param( 1, HB_IT_ANY ) ) );
}

HB_FUNC( HB_ISBYREF )
{
   PHB_ITEM pItem;

   if( hb_pcount() )
   {
      pItem = hb_stackItemFromBase( 1 );

      if( pItem->type & HB_IT_BYREF )
      {
         pItem = hb_itemUnRefOnce( pItem );

         if( pItem->type & HB_IT_BYREF )

            hb_retl( TRUE );

         else

            hb_retl( FALSE );
      }
      else
         hb_ret( );
   }
}

HB_FUNC( HB_ISNIL )
{
  hb_retl( ISNIL( 1 ) );
}

HB_FUNC( HB_ISCHAR )
{
  hb_retl( ISCHAR( 1 ) );
}

HB_FUNC( HB_ISMEMO )
{
  hb_retl( ISMEMO( 1 ) );
}

HB_FUNC( HB_ISNUM )
{
  hb_retl( ISNUM( 1 ) );
}

HB_FUNC( HB_ISLOGIC )
{
  hb_retl( ISLOG( 1 ) );
}

HB_FUNC( HB_ISDATE )
{
  hb_retl( ISDATE( 1 ) );
}

HB_FUNC( HB_ISARRAY )
{
  hb_retl( ISARRAY( 1 ) && hb_param( 1, HB_IT_ARRAY )->item.asArray.value->uiClass == 0 );
}

HB_FUNC( HB_ISOBJECT )
{
  hb_retl( ISOBJECT( 1 ) );
}

HB_FUNC( HB_ISBLOCK )
{
  hb_retl( ISBLOCK( 1 ) );
}

HB_FUNC( HB_ISPOINTER )
{
  hb_retl( ISPOINTER( 1 ) );
}

HB_FUNC( HB_ISHASH )
{
  hb_retl( ISHASH( 1 ) );
}

