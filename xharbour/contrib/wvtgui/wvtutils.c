/*
 * $Id: wvtutils.c,v 1.14 2007/03/18 05:40:46 vouchcac Exp $
 */

/*
 * Harbour Project source code:
 * Video subsystem for Win32 using GUI windows instead of Console
 *     Copyright 2003 Peter Rees <peter@rees.co.nz>
 *                    Rees Software & Systems Ltd
 * based on
 *   Bcc ConIO Video subsystem by
 *     Copyright 2002 Marek Paliwoda <paliwoda@inteia.pl>
 *     Copyright 2002 Przemyslaw Czerpak <druzus@polbox.com>
 *   Video subsystem for Win32 compilers
 *     Copyright 1999-2000 Paul Tucker <ptucker@sympatico.ca>
 *     Copyright 2002 Przemysław Czerpak <druzus@polbox.com>
 *
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 *
 * Copyright 1999 David G. Holm <dholm@jsd-llc.com>
 *    hb_gt_Tone()
 *
 * See doc/license.txt for licensing terms.
 *
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option )
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.   If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/ ).
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
 * not apply to the code that you add in this way.   To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

//-------------------------------------------------------------------//

#define HB_OS_WIN_32_USED

//-------------------------------------------------------------------//

#include "hbgtwvt.h"

/* workaround for missing declaration in MinGW32 */
#if !defined(TTM_SETTITLE) && defined(TTM_SETTITLEA)
   #define TTM_SETTITLE TTM_SETTITLEA
#endif

//-------------------------------------------------------------------//

static GLOBAL_DATA *_s = NULL;

//-------------------------------------------------------------------//

HB_EXTERN_BEGIN

extern HANDLE  hb_hInstance;

extern BOOL wvt_Array2Rect(PHB_ITEM aRect, RECT *rc );
extern PHB_ITEM wvt_Rect2Array( RECT *rc  );
extern BOOL wvt_Array2Point(PHB_ITEM aPoint, POINT *pt );
extern PHB_ITEM wvt_Point2Array( POINT *pt  );
extern BOOL wvt_Array2Size(PHB_ITEM aSize, SIZE *siz );
extern PHB_ITEM wvt_Size2Array( SIZE *siz  );
extern void  wvt_Rect2ArrayEx( RECT *rc ,PHB_ITEM aRect );
extern void wvt_Point2ArrayEx( POINT *pt  , PHB_ITEM aPoint);
extern void wvt_Size2ArrayEx( SIZE *siz ,PHB_ITEM aSize );

HB_EXTERN_END

//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//
//              Pritpal Bedi <pritpal@vouchcac.com>
//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

void HB_EXPORT hb_wvt_wvtUtils( void )
{
   _s = hb_wvt_gtGetGlobalData();
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_UTILS )
{
   hb_wvt_wvtUtils();
}

//-------------------------------------------------------------------//
//
//     Wvt_ChooseFont( cFontName, nHeight, nWidth, nWeight, nQuality, ;
//                                    lItalic, lUnderline, lStrikeout )
//
HB_FUNC( WVT_CHOOSEFONT )
{
   CHOOSEFONT  cf = { 0 };
   LOGFONT     lf = { 0 };
   LONG        PointSize = 0;

   if ( ! ISNIL( 2 ) )
   {
      PointSize = -MulDiv( ( LONG ) hb_parnl( 2 ), GetDeviceCaps( _s->hdc, LOGPIXELSY ), 72 ) ;
   }

   lf.lfHeight         = PointSize;
   lf.lfWidth          = ISNIL( 3 ) ? 0 : hb_parni( 3 );
   lf.lfWeight         = ISNIL( 4 ) ? 0 : hb_parni( 4 );
   lf.lfItalic         = ISNIL( 6 ) ? 0 : hb_parl( 6 );
   lf.lfUnderline      = ISNIL( 7 ) ? 0 : hb_parl( 7 );
   lf.lfStrikeOut      = ISNIL( 8 ) ? 0 : hb_parl( 8 );
   lf.lfCharSet        = DEFAULT_CHARSET;
   lf.lfQuality        = ISNIL( 5 ) ? DEFAULT_QUALITY : hb_parni( 5 );
   lf.lfPitchAndFamily = FF_DONTCARE;
   if ( ISCHAR( 1 ) )
   {
      strcpy( lf.lfFaceName, hb_parcx( 1 ) );
   }

   cf.lStructSize      = sizeof( CHOOSEFONT );
   cf.hwndOwner        = _s->hWnd;
   cf.hDC              = ( HDC ) NULL;
   cf.lpLogFont        = &lf;
   cf.iPointSize       = 0;
   cf.Flags            = CF_SCREENFONTS | CF_EFFECTS | CF_SHOWHELP | CF_INITTOLOGFONTSTRUCT ;
   cf.rgbColors        = RGB( 0,0,0 );
   cf.lCustData        = 0L;
   cf.lpfnHook         = ( LPCFHOOKPROC ) NULL;
   cf.lpTemplateName   = ( LPSTR ) NULL;
   cf.hInstance        = ( HINSTANCE ) NULL;
   cf.lpszStyle        = ( LPSTR ) NULL;
   cf.nFontType        = SCREEN_FONTTYPE;
   cf.nSizeMin         = 0;
   cf.nSizeMax         = 0;

   if ( ChooseFont( &cf ) )
   {
      PointSize = -MulDiv( lf.lfHeight, 72, GetDeviceCaps( _s->hdc, LOGPIXELSY ) ) ;

      hb_reta( 8 );

      hb_storc(  lf.lfFaceName     , -1, 1 );
      hb_stornl( ( LONG ) PointSize, -1, 2 );
      hb_storni( lf.lfWidth        , -1, 3 );
      hb_storni( lf.lfWeight       , -1, 4 );
      hb_storni( lf.lfQuality      , -1, 5 );
      hb_storl(  lf.lfItalic       , -1, 6 );
      hb_storl(  lf.lfUnderline    , -1, 7 );
      hb_storl(  lf.lfStrikeOut    , -1, 8 );
   }
   else
   {
      hb_reta( 8 );

      hb_storc(  ""        , -1, 1 );
      hb_stornl( ( LONG ) 0, -1, 2 );
      hb_storni( 0         , -1, 3 );
      hb_storni( 0         , -1, 4 );
      hb_storni( 0         , -1, 5 );
      hb_storl(  0         , -1, 6 );
      hb_storl(  0         , -1, 7 );
      hb_storl(  0         , -1, 8 );
   }

   return ;
}

//-------------------------------------------------------------------//
//
//    Wvt_ChooseColor( nRGBInit, aRGB16, nFlags ) => nRGBSelected
//
HB_FUNC( WVT_CHOOSECOLOR )
{
   CHOOSECOLOR cc ;
   COLORREF    crCustClr[ 16 ] ;
   int         i ;

   for( i = 0 ; i < 16 ; i++ )
   {
     crCustClr[ i ] = ( ISARRAY( 2 ) ? ( COLORREF ) hb_parnl( 2, i+1 ) : GetSysColor( COLOR_BTNFACE ) ) ;
   }

   cc.lStructSize   = sizeof( CHOOSECOLOR ) ;
   cc.hwndOwner     = _s->hWnd ;
   cc.rgbResult     = ISNIL( 1 ) ?  0 : ( COLORREF ) hb_parnl( 1 ) ;
   cc.lpCustColors  = crCustClr ;
   cc.Flags         = ( WORD ) ( ISNIL( 3 ) ? CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN : hb_parnl( 3 ) );

   if ( ChooseColor( &cc ) )
   {
      hb_retnl( cc.rgbResult ) ;
   }
   else
   {
      hb_retnl( -1 );
   }
}

//-------------------------------------------------------------------//
//
//  Wvt_MessageBox( cMessage, cTitle, nIcon, hWnd )
//
HB_FUNC( WVT_MESSAGEBOX )
{
   hb_retni( MessageBox( _s->hWnd, hb_parcx( 1 ), hb_parcx( 2 ), ISNIL( 3 ) ? MB_OK : hb_parni( 3 ) ) ) ;
}

//-------------------------------------------------------------------//
#if _WIN32_IE > 0x400
//-------------------------------------------------------------------//
//
//                              Tooltips
//
//-------------------------------------------------------------------//

HB_FUNC( WVT_SETTOOLTIPACTIVE )
{
   BOOL bActive = _s->bToolTipActive;

   if ( ! ISNIL( 1 ) )
   {
      _s->bToolTipActive = hb_parl( 1 );
   }

   hb_retl( bActive );
}

//-------------------------------------------------------------------//
//
//   Wvt_SetToolTip( nTop, nLeft, nBottom, nRight, cToolText )
//
HB_FUNC( WVT_SETTOOLTIP )
{
   TOOLINFO ti = { 0 };
   POINT    xy = { 0 };
   int      iTop, iLeft, iBottom, iRight;

   if ( ! _s->bToolTipActive )
   {
      return;
   }

   ti.cbSize    = sizeof( TOOLINFO );
   ti.hwnd      = _s->hWnd;
   ti.uId       = 100000;

   if ( SendMessage( _s->hWndTT, TTM_GETTOOLINFO, 0, ( LPARAM ) &ti ) )
   {
      xy      = hb_wvt_gtGetXYFromColRow( hb_parni( 2 ), hb_parni( 1 ) );
      iTop    = xy.y;
      iLeft   = xy.x;

      xy      = hb_wvt_gtGetXYFromColRow( hb_parni( 4 )+1, hb_parni( 3 )+1 );
      iBottom = xy.y - 1;
      iRight  = xy.x - 1;

      ti.lpszText    = hb_parc( 5 );
      ti.rect.left   = iLeft;
      ti.rect.top    = iTop;
      ti.rect.right  = iRight;
      ti.rect.bottom = iBottom;

      SendMessage( _s->hWndTT, TTM_SETTOOLINFO, 0, ( LPARAM ) &ti );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETTOOLTIPTEXT )
{
   TOOLINFO ti;

   ti.cbSize = sizeof( TOOLINFO );
   ti.hwnd   = _s->hWnd;
   ti.uId    = 100000;

   if ( SendMessage( _s->hWndTT, TTM_GETTOOLINFO, 0, ( LPARAM ) &ti ) )
   {
      ti.lpszText = hb_parcx( 1 );
      SendMessage( _s->hWndTT, TTM_UPDATETIPTEXT, 0, ( LPARAM ) &ti );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETTOOLTIPMARGIN )
{
   RECT rc = { 0 };

   rc.left   = hb_parni( 2 );
   rc.top    = hb_parni( 1 );
   rc.right  = hb_parni( 4 );
   rc.bottom = hb_parni( 3 );

   SendMessage( _s->hWndTT, TTM_SETMARGIN, 0, ( LPARAM ) &rc );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETTOOLTIPWIDTH )
{
   int iTipWidth = SendMessage( _s->hWndTT, TTM_GETMAXTIPWIDTH, 0, 0 );

   if ( ISNUM( 1 ) )
   {
      SendMessage( _s->hWndTT, TTM_SETMAXTIPWIDTH, 0, ( LPARAM ) ( int ) hb_parni( 1 ) );
   }

   hb_retni( iTipWidth );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETTOOLTIPBKCOLOR )
{
   COLORREF cr = SendMessage( _s->hWndTT, TTM_GETTIPBKCOLOR, 0, 0 );

   if ( ISNUM( 1 ) )
   {
      SendMessage( _s->hWndTT, TTM_SETTIPBKCOLOR, ( WPARAM ) ( COLORREF ) hb_parnl( 1 ), 0 );
   }
   hb_retnl( ( COLORREF ) cr );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETTOOLTIPTEXTCOLOR )
{
   COLORREF cr = SendMessage( _s->hWndTT, TTM_GETTIPTEXTCOLOR, 0, 0 );

   if ( ISNUM( 1 ) )
   {
      SendMessage( _s->hWndTT, TTM_SETTIPTEXTCOLOR, ( WPARAM ) ( COLORREF ) hb_parnl( 1 ), 0 );
   }

   hb_retnl( ( COLORREF ) cr );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETTOOLTIPTITLE )
{
   int iIcon;

   if ( ! ISNIL( 2 ) )
   {
      iIcon = ISNIL( 1 ) ? 0 : hb_parni( 1 );
      if ( iIcon > 3 )
      {
         iIcon = 0 ;
      }
      SendMessage( _s->hWndTT, TTM_SETTITLE, ( WPARAM ) iIcon, ( LPARAM ) hb_parcx( 2 ) );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETTOOLTIPWIDTH )
{
   hb_retni( SendMessage( _s->hWndTT, TTM_GETMAXTIPWIDTH, 0, 0 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETTOOLTIPBKCOLOR )
{
   hb_retnl( ( COLORREF ) SendMessage( _s->hWndTT, TTM_GETTIPBKCOLOR, 0, 0 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETTOOLTIPTEXTCOLOR )
{
   hb_retnl( ( COLORREF ) SendMessage( _s->hWndTT, TTM_GETTIPTEXTCOLOR, 0, 0 ) );
}

//-------------------------------------------------------------------//
#endif
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

HB_FUNC( WVT_SETGUI )
{
   BOOL bGui = _s->bGui;

   if ( ! ISNIL( 1 ) )
   {
      _s->bGui = hb_parl( 1 );
   }

   hb_retl( bGui );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETTIMER )
{
   hb_retl( SetTimer( _s->hWnd, 101, hb_parni( 1 ), NULL ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_KILLTIMER )
{
   hb_retl( KillTimer( _s->hWnd, 101 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETONTOP )
{
   RECT rect = { 0 };

   GetWindowRect( _s->hWnd, &rect );

   hb_retl( SetWindowPos( _s->hWnd, HWND_TOPMOST,
                          rect.left,
                          rect.top,
                          0,
                          0,
                          SWP_NOSIZE + SWP_NOMOVE + SWP_NOACTIVATE ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETASNORMAL )
{
   RECT rect = { 0 };

   GetWindowRect( _s->hWnd, &rect );

   hb_retl( SetWindowPos( _s->hWnd, HWND_NOTOPMOST,
                          rect.left,
                          rect.top,
                          0,
                          0,
                          SWP_NOSIZE + SWP_NOMOVE + SWP_NOACTIVATE ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_MINIMIZE )
{
   ShowWindow( _s->hWnd, SW_MINIMIZE );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_MAXIMIZE )
{
   ShowWindow( _s->hWnd, SW_RESTORE );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETMOUSEPOS )
{
   POINT xy = { 0 };

   xy = hb_wvt_gtGetXYFromColRow( hb_parni( 2 ), hb_parni( 1 ) );

   if ( ClientToScreen( _s->hWnd, &xy ) )
   {
      hb_retl( SetCursorPos( xy.x, xy.y + ( _s->PTEXTSIZE.y / 2 ) ) );
   }
   else
   {
      hb_retl( FALSE );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETPAINTRECT )
{
   PHB_ITEM info = hb_itemArrayNew( 4 );
   PHB_ITEM temp = hb_itemNew( NULL );

   hb_arraySet( info, 1, hb_itemPutNI( temp, _s->rowStart ) );
   hb_arraySet( info, 2, hb_itemPutNI( temp, _s->colStart ) );
   hb_arraySet( info, 3, hb_itemPutNI( temp, _s->rowStop  ) );
   hb_arraySet( info, 4, hb_itemPutNI( temp, _s->colStop  ) );

   hb_itemRelease( temp );
   hb_itemReturn( info );
   hb_itemRelease( info );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETPOINTER )
{
   int     iCursor = hb_parni( 1 );
   HCURSOR hCursor;

   switch ( iCursor )
   {
   case 1:
      hCursor = LoadCursor( NULL, IDC_ARROW    );
      break;

   case 2:
      hCursor = LoadCursor( NULL, IDC_IBEAM    );
      break;

   case 3:
      hCursor = LoadCursor( NULL, IDC_WAIT     );
      break;

   case 4:
      hCursor = LoadCursor( NULL, IDC_CROSS    );
      break;

   case 5:
      hCursor = LoadCursor( NULL, IDC_UPARROW  );
      break;

   case 6:
      hCursor = LoadCursor( NULL, IDC_SIZE     );
      break;

   case 7:
      hCursor = LoadCursor( NULL, IDC_ICON     );
      break;

   case 8:
      hCursor = LoadCursor( NULL, IDC_SIZENWSE );
      break;

   case 9:
      hCursor = LoadCursor( NULL, IDC_SIZENESW );
      break;

   case 10:
      hCursor = LoadCursor( NULL, IDC_SIZEWE   );
      break;

   case 11:
      hCursor = LoadCursor( NULL, IDC_SIZENS   );
      break;

   case 12:
      hCursor = LoadCursor( NULL, IDC_SIZEALL  );
      break;

   case 13:
      hCursor = LoadCursor( NULL, IDC_NO       );
      break;

   case 14:
      hCursor = LoadCursor( NULL, IDC_HAND     );
      break;

   case 15:
      hCursor = LoadCursor( NULL, IDC_APPSTARTING );
      break;

   case 16:
      hCursor = LoadCursor( NULL, IDC_HELP     );
      break;

   default:
      hCursor = LoadCursor( NULL, IDC_ARROW    );
      break;
   }

   SetClassLong( _s->hWnd, GCL_HCURSOR, ( DWORD ) hCursor );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETFONT )
{
   hb_retl( hb_wvt_gtSetFont(
            ISNIL( 1 ) ? _s->fontFace   : hb_parcx( 1 ),
            ISNIL( 2 ) ? _s->fontHeight : hb_parni( 2 ),
            ISNIL( 3 ) ? _s->fontWidth  : hb_parni( 3 ),
            ISNIL( 4 ) ? _s->fontWeight : hb_parni( 4 ),
            ISNIL( 5 ) ? _s->fontQuality: hb_parni( 5 )
           ) ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETICON )
{
   if ( ISNUM( 1 ) || ISCHAR( 2 ) )
   {
      hb_retnl( hb_wvt_gtSetWindowIcon( hb_parni( 1 ), hb_parc( 2 ) ) ) ;
   }
   else
   {
      hb_retnl( hb_wvt_gtSetWindowIconFromFile( hb_parcx( 1 ) ) ) ;
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETTITLE )
{
   hb_wvt_gtSetWindowTitle( hb_parcx( 1 ) ) ;
   return ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETWINDOWPOS )
{
   hb_wvt_gtSetWindowPos( hb_parni( 1 ), hb_parni( 2 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETWINDOWHANDLE )
{
   hb_retnl( ( LONG ) hb_wvt_gtGetWindowHandle() ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETCODEPAGE )
{
   hb_retni( hb_wvt_gtSetCodePage( hb_parni( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_CENTERWINDOW )
{
   hb_retl( hb_wvt_gtSetCentreWindow(
               ISNIL( 1 ) ? TRUE  : hb_parl( 1 ),
               ISNIL( 2 ) ? FALSE : hb_parl( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETMOUSEMOVE )
{
   if ( ISNIL( 1 ) )
   {
      hb_retl( _s->MouseMove );
   }
   else
   {
      hb_retl( hb_wvt_gtSetMouseMove( hb_parl( 1 ) ) );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETXYFROMROWCOL )
{
   PHB_ITEM info = hb_itemArrayNew( 2 );
   PHB_ITEM temp = hb_itemNew( NULL );
   POINT    xy = { 0 };

   xy = hb_wvt_gtGetXYFromColRow( hb_parni( 2 ), hb_parni( 1 ) );

   hb_arraySet( info, 1, hb_itemPutNL( temp, xy.x ) );
   hb_arraySet( info, 2, hb_itemPutNL( temp, xy.y ) );

   hb_itemRelease( temp );
   hb_itemReturn( info );
   hb_itemRelease( info );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETFONTINFO )
{
   PHB_ITEM info = hb_itemArrayNew( 7 );
   PHB_ITEM temp = hb_itemNew( NULL );

   hb_arraySet( info, 1, hb_itemPutC(  temp, _s->fontFace    ) );
   hb_arraySet( info, 2, hb_itemPutNL( temp, _s->fontHeight  ) );
   hb_arraySet( info, 3, hb_itemPutNL( temp, _s->fontWidth   ) );
   hb_arraySet( info, 4, hb_itemPutNL( temp, _s->fontWeight  ) );
   hb_arraySet( info, 5, hb_itemPutNL( temp, _s->fontQuality ) );
   hb_arraySet( info, 6, hb_itemPutNL( temp, _s->PTEXTSIZE.y ) );
   hb_arraySet( info, 7, hb_itemPutNL( temp, _s->PTEXTSIZE.x ) );

   hb_itemRelease( temp );
   hb_itemReturn( info );
   hb_itemRelease( info );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETPALETTE )
{
   PHB_ITEM info = hb_itemArrayNew( 16 );
   PHB_ITEM temp = hb_itemNew( NULL );
   int      i;

   for ( i = 0; i < 16; i++ )
   {
      hb_arraySet( info, i+1, hb_itemPutNL( temp, hb_wvt_gtGetColorData( i ) ) );
   }
   hb_itemRelease( temp );
   hb_itemReturn( info );
   hb_itemRelease( info );
}

//-------------------------------------------------------------------//
//
//    Wvt_SetPalette( aRGBValues ) -> An array of 16 elements with RGB values
//
HB_FUNC( WVT_SETPALETTE )
{
   int       i;

   for ( i = 0; i < 16; i++ )
   {
      hb_wvt_gtSetColorData( i, hb_parnl( 1, i+1 ) );
   }
}

//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//
//                 Peter Rees <peter@rees.co.nz>
//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

HB_FUNC( WVT_SETMENU )
{
   SetMenu( _s->hWnd, ( HMENU ) hb_parni( 1 ) ) ;

   hb_wvt_gtResetWindow();
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETPOPUPMENU )
{
   HMENU hPopup = _s->hPopup ;

   _s->hPopup = ( HMENU ) hb_parnl( 1 );
   if ( hPopup )
   {
      hb_retnl( ( LONG ) hPopup );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_CREATEMENU )
{
  hb_retnl( ( LONG ) CreateMenu() ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_CREATEPOPUPMENU )
{
  hb_retnl( ( LONG ) CreatePopupMenu() ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_APPENDMENU )
{
  char    ucBuf[ 256 ];
  int     i,iLen ;
  LPCTSTR lpszCaption;

  if ( ISCHAR( 4 ) )
  {
    iLen = hb_parclen( 4 );
    if ( iLen > 0 && iLen < 256 )   // Translate '~' to '&'
    {
      lpszCaption = hb_parcx( 4 ) ;
      for ( i = 0; i < iLen; i++ )
      {
        ucBuf[ i ] = ( *lpszCaption == '~' ) ? '&' : *lpszCaption ;
        lpszCaption++;
      }
      ucBuf[ iLen ]= '\0';
      lpszCaption = ucBuf ;
    }
    else
    {
      lpszCaption = hb_parcx( 4 ) ;
    }
  }
  else
  {
    lpszCaption = ( LPCTSTR ) hb_parni( 4 ) ; // It is a SEPARATOR or Submenu
  }

  hb_retl( AppendMenu( ( HMENU ) hb_parnl( 1 ), ( UINT ) hb_parni( 2 ), ( UINT_PTR ) hb_parni( 3 ), ( LPCTSTR ) lpszCaption ) ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_DELETEMENU )
{
  hb_retl( DeleteMenu( ( HMENU ) hb_parnl( 1 ), ( UINT ) hb_parni( 2 ), ( UINT ) hb_parni( 3 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_DESTROYMENU )
{
  hb_retl( DestroyMenu( ( HMENU ) hb_parnl( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_ENABLEMENUITEM )
{
  hb_retni( EnableMenuItem( ( HMENU ) hb_parnl( 1 ), ( UINT ) hb_parni( 2 ), ( UINT ) hb_parni( 3 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETLASTMENUEVENT )
{
  hb_retni( hb_wvt_gtGetLastMenuEvent() ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETLASTMENUEVENT )
{
  hb_retni( hb_wvt_gtSetLastMenuEvent( hb_parni(1) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETMENUKEYEVENT )
{
  int iEvent = 0;

  if ( ISNUM( 1 ) )
  {
    iEvent = hb_parnl( 1 ) ;
  }

  hb_retni( hb_wvt_gtSetMenuKeyEvent( iEvent ) ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_DRAWMENUBAR )
{
  DrawMenuBar( _s->hWnd ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETSCREENWIDTH )
{
  hb_retni( GetSystemMetrics( SM_CXSCREEN ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETSCREENHEIGHT )
{
  hb_retni( GetSystemMetrics( SM_CYSCREEN ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETWINDOWCENTRE )
{
  hb_wvt_gtSetCentreWindow( hb_parl( 1 ), hb_parl( 2 ) ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETALTF4CLOSE )
{
  hb_retl( hb_wvt_gtSetAltF4Close( hb_parl( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_PROCESSMESSAGES )
{
  hb_wvt_gtDoProcessMessages();

  hb_retl( 1 );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETTITLE )
{
  BYTE ucText[ 1024 ];

  hb_wvt_gtGetWindowTitle( ( char* ) ucText, 1023 );

  hb_retc( ( char* ) ucText ) ;
}

//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//
//   Author.....: Francesco Saverio Giudice <info@fsgiudice.com>
//   Syntax.....: Wvt_GetRGBColor( nColor ) --> nRGBColor
//   Description: Return the RGB values passing the color positional value
//                0=Black, 1=Blue, etc
//                as returned from hb_ColorToN()
//   Creat. Date: 2004/01/15
//   Last Modif.: 2004/01/15
//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

HB_FUNC( WVT_GETRGBCOLOR )
{
   int iColor;
   if ( !ISNIL( 1 ) )
   {
      iColor = hb_parni( 1 );
      if ( iColor >= 0 && iColor < 16 )  /* Test bound error */
      {
         hb_retnl( hb_wvt_gtGetColorData( iColor ) );
      }
   }
}

//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//
//                       Giancarlo Niccolai
//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

HB_FUNC( WVT_GETCLIPBOARD )
{
    HGLOBAL   hglb;
    LPTSTR    lptstr;

    if ( !IsClipboardFormatAvailable( CF_TEXT ) )
    {
      hb_ret();
    }

    if ( !OpenClipboard( NULL ) )
    {
      hb_ret();
    }

    hglb = GetClipboardData( CF_TEXT );
    if ( hglb != NULL )
    {
       lptstr = ( LPSTR ) GlobalLock( hglb );
       if ( lptstr != NULL )
       {
          hb_retc( lptstr );
          GlobalUnlock( hglb );
       }
    }
    CloseClipboard();
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SETCLIPBOARD )
{
   LPTSTR  lptstrCopy;
   HGLOBAL hglbCopy;
   char *  cText;
   int     nLen;

   if ( !IsClipboardFormatAvailable( CF_TEXT ) )
   {
     hb_retl( FALSE );
     return;
   }

   // Check params
   //
   if ( ! ISCHAR( 1 ) )
   {
     hb_retl( FALSE );
     return;
   }

   if ( ! OpenClipboard( NULL ) )
   {
     hb_retl( FALSE );
     return;
   }
   EmptyClipboard();

   // Get text from PRG
   //
   cText = hb_parcx( 1 );
   nLen  = hb_parclen( 1 );

   // Allocate a global memory object for the text.
   //
   hglbCopy = GlobalAlloc( GMEM_MOVEABLE, ( nLen+1 ) * sizeof( TCHAR ) );
   if ( hglbCopy == NULL )
   {
       CloseClipboard();
       hb_retl( FALSE );
       return;
   }

   // Lock the handle and copy the text to the buffer.
   //
   lptstrCopy = ( LPSTR ) GlobalLock( hglbCopy );
   memcpy( lptstrCopy, cText, ( nLen+1 ) * sizeof( TCHAR ) );
   lptstrCopy[ nLen+1 ] = ( TCHAR ) 0;    // null character
   GlobalUnlock( hglbCopy );

   // Place the handle on the clipboard.
   //
   SetClipboardData( CF_TEXT, hglbCopy );

   CloseClipboard();
   hb_retl( TRUE );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_PASTEFROMCLIPBOARD )
{
   HGLOBAL   hglb;
   LPTSTR    lptstr;
   ULONG     ul;

   if ( !IsClipboardFormatAvailable( CF_TEXT ) )
   {
     hb_ret();
   }

   if ( !OpenClipboard( NULL ) )
   {
     hb_ret();
   }

   hglb = GetClipboardData( CF_TEXT );
   if ( hglb != NULL )
   {
      lptstr = ( LPSTR ) GlobalLock( hglb );
      if ( lptstr != NULL )
      {
         for ( ul=0; ul < GlobalSize( hglb ); ul++ )
         {
            hb_wvt_gtAddCharToInputQueue( ( int ) lptstr[ ul ] );
         }
         GlobalUnlock( hglb ) ;
      }
   }
   CloseClipboard();
}

//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

HB_FUNC( WVT_KEYBOARD )
{
   hb_wvt_gtAddCharToInputQueue( hb_parnl( 1 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_INVALIDATERECT )
{
   RECT  rc = { 0 };
   POINT xy = { 0 };

   xy           = hb_wvt_gtGetXYFromColRow( hb_parni( 2 ), hb_parni( 1 ) );
   rc.top       = xy.y;
   rc.left      = xy.x;
   xy           = hb_wvt_gtGetXYFromColRow( hb_parni( 4 )+1, hb_parni( 3 )+1 );
   rc.bottom    = xy.y - 1;
   rc.right     = xy.x - 1;

   InvalidateRect( _s->hWnd, &rc, TRUE );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_ISLBUTTONPRESSED )
{
   hb_retl( GetKeyState( VK_LBUTTON ) & 0x8000 );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_CLIENTTOSCREEN )
{
   PHB_ITEM info = hb_itemArrayNew( 2 );
   PHB_ITEM temp = hb_itemNew( NULL );
   POINT    xy = { 0 };

   xy = hb_wvt_gtGetXYFromColRow( hb_parni( 2 ), hb_parni( 1 ) );

   ClientToScreen( _s->hWnd, &xy );

   hb_arraySet( info, 1, hb_itemPutNL( temp, xy.x ) );
   hb_arraySet( info, 2, hb_itemPutNL( temp, xy.y ) );

   hb_itemRelease( temp );
   hb_itemReturn( info );
   hb_itemRelease( info );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETCURSORPOS )
{
   POINT    xy = { 0 };
   PHB_ITEM info = hb_itemArrayNew( 2 );
   PHB_ITEM temp = hb_itemNew( NULL );

   GetCursorPos( &xy );

   hb_arraySet( info, 1, hb_itemPutNI( temp, xy.x ) );
   hb_arraySet( info, 2, hb_itemPutNI( temp, xy.y ) );

   hb_itemRelease( temp );
   hb_itemReturn( info );
   hb_itemRelease( info );
 }

//-------------------------------------------------------------------//

HB_FUNC( WVT_TRACKPOPUPMENU )
{
   POINT xy = { 0 };

   GetCursorPos( &xy );

   hb_retnl( TrackPopupMenu( ( HMENU ) hb_parnl( 1 ) ,
                     TPM_CENTERALIGN | TPM_RETURNCMD ,
                                                xy.x ,
                                                xy.y ,
                                                   0 ,
                                            _s->hWnd ,
                                                NULL ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_GETMENU )
{
   hb_retnl( ( ULONG ) GetMenu( _s->hWnd ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_SHOWWINDOW )
{
   ShowWindow( _s->hWnd, hb_parni( 1 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_UPDATEWINDOW )
{
   UpdateWindow( _s->hWnd );
}

//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//
//                             Dialogs
//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

HB_FUNC( WVT_CREATEDIALOGDYNAMIC )
{
   PHB_ITEM pFirst = hb_param( 3,HB_IT_ANY );
   PHB_ITEM pFunc  = NULL ;
   PHB_DYNS pExecSym;
   HWND     hDlg ;
   int      iIndex;
   int      iType;
   int      iResource = hb_parni( 4 );

   /* check if we still have room for a new dialog */
   for ( iIndex = 0; iIndex < WVT_DLGML_MAX; iIndex++ )
   {
      if ( _s->hDlgModeless[ iIndex ] == NULL )
      {
         break;
      }
   }

   if ( iIndex >= WVT_DLGML_MAX )
   {
      /* no more room */
      hb_retnl( (ULONG) NULL );
      return;
   }

   if ( HB_IS_BLOCK( pFirst ) )
   {
      /* pFunc is pointing to stored code block (later) */
      pFunc = hb_itemNew( pFirst );
      iType = 2;
   }
   else if( hb_itemType( pFirst ) == HB_IT_STRING )
   {
      #ifdef __XHARBOUR__
      hb_dynsymLock();
      #endif
      pExecSym = hb_dynsymFindName( hb_itemGetC( pFirst ) );
      #ifdef __XHARBOUR__
      hb_dynsymUnlock();
      #endif
      if ( pExecSym )
      {
         pFunc = ( PHB_ITEM ) pExecSym;
      }
      iType = 1;
   }

   {
      if ( ISNUM( 3 ) )
      {
         hDlg = CreateDialogIndirect( ( HINSTANCE     ) hb_hInstance,
                                      ( LPDLGTEMPLATE ) hb_parc( 1 ),
                                                        hb_parl( 2 ) ? _s->hWnd : NULL,
                                      ( DLGPROC       ) hb_parnl( 3 ) );
      }
      else
      {
         switch ( iResource )
         {
            case 0:
            {
               hDlg = CreateDialog( ( HINSTANCE     ) hb_hInstance,
                                                      hb_parc( 1 ),
                                                      hb_parl( 2 ) ? _s->hWnd : NULL,
                                                      (DLGPROC) hb_wvt_gtDlgProcMLess );
            }
            break;

            case 1:
            {
               hDlg = CreateDialog( ( HINSTANCE     ) hb_hInstance,
                                    MAKEINTRESOURCE( ( WORD ) hb_parni( 1 ) ),
                                                      hb_parl( 2 ) ? _s->hWnd : NULL,
                                                      (DLGPROC) hb_wvt_gtDlgProcMLess );
            }
            break;

            case 2:
            {
               hDlg = CreateDialogIndirect( ( HINSTANCE     ) hb_hInstance,
                                            ( LPDLGTEMPLATE ) hb_parc( 1 ),
                                                              hb_parl( 2 ) ? _s->hWnd : NULL,
                                                              (DLGPROC) hb_wvt_gtDlgProcMLess );
            }
            break;
         }
      }

      if ( hDlg )
      {
         _s->hDlgModeless[ iIndex ] = hDlg;

         if ( pFunc )
         {
            /* if codeblock, store the codeblock and lock it there */
            if (HB_IS_BLOCK( pFirst ))
            {
               _s->pcbFunc[ iIndex ] = pFunc;
            }

            _s->pFunc[ iIndex ] = pFunc;
            _s->iType[ iIndex ] = iType;
         }
         else
         {
            _s->pFunc[ iIndex ] = NULL;
            _s->iType[ iIndex ] = 0;
         }
         SendMessage( hDlg, WM_INITDIALOG, 0, 0 );
      }
      else
      {
         //if codeblock item created earlier, release it
         if (iType==2 && pFunc)
         {
            hb_itemRelease( pFunc );
         }
         _s->hDlgModeless[ iIndex ] = NULL;
      }
   }

   hb_retnl( ( ULONG ) hDlg );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_CREATEDIALOGMODAL )
{
   PHB_ITEM pFirst    = hb_param( 3,HB_IT_ANY );
   PHB_ITEM pFunc     = NULL ;
   PHB_DYNS pExecSym;
   int      iIndex;
   int      iResource = hb_parni( 4 );
   int      iResult   = 0;
   HWND     hParent   = ISNIL( 5 ) ? _s->hWnd : ( HWND ) hb_parnl( 5 );

   /* check if we still have room for a new dialog */
   for ( iIndex = 0; iIndex < WVT_DLGMD_MAX; iIndex++ )
   {
      if ( _s->hDlgModal[ iIndex ] == NULL )
      {
         break;
      }
   }

   if ( iIndex >= WVT_DLGMD_MAX )
   {
      /* no more room */
      hb_retni( ( int ) NULL );
      return;
   }

   if ( HB_IS_BLOCK( pFirst ) )
   {
      /* pFunc is pointing to stored code block (later) */

      _s->pcbFuncModal[ iIndex ] = hb_itemNew( pFirst );

      pFunc = _s->pcbFuncModal[ iIndex ];
      _s->pFuncModal[ iIndex ] = pFunc;
      _s->iTypeModal[ iIndex ] = 2;
   }
   else if( hb_itemType( pFirst ) == HB_IT_STRING )
   {
      #ifdef __XHARBOUR__
      hb_dynsymLock();
      #endif
      pExecSym = hb_dynsymFindName( hb_itemGetC( pFirst ) );
      #ifdef __XHARBOUR__
      hb_dynsymUnlock();
      #endif
      if ( pExecSym )
      {
         pFunc = ( PHB_ITEM ) pExecSym;
      }
      _s->pFuncModal[ iIndex ] = pFunc;
      _s->iTypeModal[ iIndex ] = 1;
   }

   switch ( iResource )
   {
      case 0:
      {
         iResult = DialogBoxParam( ( HINSTANCE     ) hb_hInstance,
                                                     hb_parc( 1 ),
                                                     hParent,
                                                     (DLGPROC) hb_wvt_gtDlgProcModal,
                                ( LPARAM ) ( DWORD ) iIndex+1 );
      }
      break;

      case 1:
      {
         iResult = DialogBoxParam( ( HINSTANCE     ) hb_hInstance,
                           MAKEINTRESOURCE( ( WORD ) hb_parni( 1 ) ),
                                                     hParent,
                                                     (DLGPROC) hb_wvt_gtDlgProcModal,
                                ( LPARAM ) ( DWORD ) iIndex+1 );
      }
      break;

      case 2:
      {
         iResult = DialogBoxIndirectParam( ( HINSTANCE     ) hb_hInstance,
                                           ( LPDLGTEMPLATE ) hb_parc( 1 ),
                                                             hParent,
                                                             (DLGPROC) hb_wvt_gtDlgProcModal,
                                        ( LPARAM ) ( DWORD ) iIndex+1 );
      }
      break;
   }

   hb_retni( iResult );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT__MAKEDLGTEMPLATE )
{
   WORD  *p, *pdlgtemplate ;
   WORD  nItems = hb_parni( 1, 4 ) ;
   int   i, nchar ;
   DWORD lStyle ;

   // Parameters: 12 arrays
   // 1 for DLG template
   // 11 for item properties

   // 64k allow to build up to 255 items on the dialog
   //
   pdlgtemplate = p = ( PWORD ) LocalAlloc( LPTR, 65534 )  ;

   //---------------

    lStyle = hb_parnl(1,3) ;

    // start to fill in the dlgtemplate information.  addressing by WORDs

    *p++ = 1                        ; // version
    *p++ = 0xFFFF                   ; // signature
    *p++ = LOWORD ( hb_parnl(1,1) ) ; // Help Id
    *p++ = HIWORD ( hb_parnl(1,1) ) ;

    *p++ = LOWORD ( hb_parnl(1,2) ) ; // ext. style
    *p++ = HIWORD ( hb_parnl(1,2) ) ;

    *p++ = LOWORD (lStyle)          ;
    *p++ = HIWORD (lStyle)          ;

    *p++ = (WORD)   nItems          ;  // NumberOfItems
    *p++ = (short)  hb_parni(1,5)   ;  // x
    *p++ = (short)  hb_parni(1,6)   ;  // y
    *p++ = (short)  hb_parni(1,7)   ;  // cx
    *p++ = (short)  hb_parni(1,8)   ;  // cy
    *p++ = (short)  0               ;  // Menu (ignored for now.)
    *p++ = (short)  0x00            ;  // Class also ignored

    if ( hb_parinfa( 1,11 ) == HB_IT_STRING )
    {
        nchar = nCopyAnsiToWideChar( p, TEXT( hb_parcx( 1,11 ) ) ) ;
        p += nchar   ;
    }
    else
    {
      *p++ =0 ;
    }
    // add in the wPointSize and szFontName here iff the DS_SETFONT bit on

    if ( ( lStyle & DS_SETFONT ) )
    {
      *p++ = (short) hb_parni(1,12) ;
      *p++ = (short) hb_parni(1,13) ;
      *p++ = (short) hb_parni(1,14) ;

      nchar = nCopyAnsiToWideChar( p, TEXT( hb_parcx(1,15) ) ) ;
      p += nchar ;
    } ;

    //---------------
    // Now, for the items

   for ( i = 1 ; i <= nItems ; i++ ) {
      // make sure each item starts on a DWORD boundary
      p = lpwAlign (p) ;

      *p++ = LOWORD ( hb_parnl(2,i) ) ;    // help id
      *p++ = HIWORD ( hb_parnl(2,i) ) ;

      *p++ = LOWORD ( hb_parnl(3,i) ) ; // ext. style
      *p++ = HIWORD ( hb_parnl(3,i) ) ;

      *p++ = LOWORD ( hb_parnl(4,i) ) ; // style
      *p++ = HIWORD ( hb_parnl(4,i) ) ;

      *p++ = (short)  hb_parni(5,i)   ;  // x
      *p++ = (short)  hb_parni(6,i)   ;  // y
      *p++ = (short)  hb_parni(7,i)   ;  // cx
      *p++ = (short)  hb_parni(8,i)   ;  // cy

      *p++ = LOWORD ( hb_parnl(9,i) ) ;  // id
      *p++ = HIWORD ( hb_parnl(9,i) ) ;  // id   // 0;

      if ( hb_parinfa( 10,i ) == HB_IT_STRING )
         {
         nchar = nCopyAnsiToWideChar( p, TEXT ( hb_parcx( 10,i ) ) ) ; // class
         p += nchar ;
         }
      else
         {
         *p++ = 0xFFFF ;
         *p++ = (WORD) hb_parni(10,i) ;
         }

      if ( hb_parinfa( 11,i ) == HB_IT_STRING )
         {
         nchar = nCopyAnsiToWideChar( p, ( LPSTR ) hb_parcx( 11,i ) ) ;  // text
         p += nchar ;
         }
      else
         {
         *p++ = 0xFFFF ;
         *p++ = (WORD) hb_parni(11,i) ;
         }

      *p++ = 0x00 ;  // extras ( in array 12 )
    } ;

    p = lpwAlign( p )  ;

    hb_retclen( ( LPSTR ) pdlgtemplate, ( ( ULONG ) p - ( ULONG ) pdlgtemplate ) ) ;

    LocalFree( LocalHandle( pdlgtemplate ) ) ;
}

//-------------------------------------------------------------------//
//
//  Helper routine.  Take an input pointer, return closest
//  pointer that is aligned on a DWORD (4 byte) boundary.
//
HB_EXPORT LPWORD lpwAlign( LPWORD lpIn )
{
   ULONG ul;
   ul = ( ULONG ) lpIn;
   ul += 3;
   ul >>=2;
   ul <<=2;
  return ( LPWORD ) ul;
}

//-----------------------------------------------------------------------------

HB_EXPORT int nCopyAnsiToWideChar( LPWORD lpWCStr, LPSTR lpAnsiIn )
{
   int nChar = 0;

   do
   {
      *lpWCStr++ = ( WORD ) *lpAnsiIn;
      nChar++;
   }
   while ( *lpAnsiIn++ );

   return nChar;
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_LBADDSTRING )
{
   SendMessage( GetDlgItem( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ) ), LB_ADDSTRING, 0, ( LPARAM )( LPSTR ) hb_parcx( 3 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_LBGETCOUNT )
{
   hb_retnl( SendMessage( GetDlgItem( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ) ), LB_GETCOUNT, 0, 0 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_LBDELETESTRING )
{
   SendMessage( GetDlgItem( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ) ), LB_DELETESTRING, hb_parni( 3 ), 0 );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_LBSETCURSEL )
{
   SendMessage( GetDlgItem( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ) ), LB_SETCURSEL, hb_parni( 3 ), 0 );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_CBADDSTRING )
{
   SendMessage( GetDlgItem( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ) ), CB_ADDSTRING, 0, ( LPARAM )( LPSTR ) hb_parcx( 3 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WVT_CBSETCURSEL )
{
   SendMessage( GetDlgItem( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ) ), CB_SETCURSEL, hb_parni( 3 ), 0 );
}

//-------------------------------------------------------------------//
//
//   Wvt_DlgSetIcon( hDlg, ncIcon )
//
HB_FUNC( WVT_DLGSETICON )
{
   HICON hIcon;

   if ( ISNUM( 2 ) )
   {
      hIcon = LoadIcon( ( HINSTANCE ) hb_hInstance, MAKEINTRESOURCE( hb_parni( 2 ) ) );
   }
   else
   {
      hIcon = ( HICON ) LoadImage( ( HINSTANCE ) NULL, hb_parc( 2 ), IMAGE_ICON, 0, 0, LR_LOADFROMFILE );
   }

   if ( hIcon )
   {
      SendMessage( ( HWND ) hb_parnl( 1 ), WM_SETICON, ICON_SMALL, ( LPARAM ) hIcon ); // Set Title Bar ICON
      SendMessage( ( HWND ) hb_parnl( 1 ), WM_SETICON, ICON_BIG,   ( LPARAM ) hIcon ); // Set Task List Icon
   }

   if ( hIcon )
   {
      hb_retnl( ( ULONG ) hIcon );
   }
}

//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//
//             Direct WinApi Functions - Prefixed WIN_*()
//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//

HB_FUNC( WIN_SENDMESSAGE )
{
   char *cText ;

   if( ISBYREF( 4 ) )
   {
      cText = ( char* ) hb_xgrab( hb_parcsiz( 4 ) );
      hb_xmemcpy( cText, hb_parcx( 4 ), hb_parcsiz( 4 ) );
   }

   hb_retnl( ( ULONG ) SendMessage( ( HWND ) hb_parnl( 1 ),
                                    ( UINT ) hb_parni( 2 ),
                                    ( ISNIL( 3 ) ? 0 : ( WPARAM ) hb_parnl( 3 ) ),
                                    ( ISNIL( 4 ) ? 0 : ( ISBYREF( 4 ) ? ( LPARAM ) ( LPSTR ) cText :
                                       ( ISCHAR( 4 ) ? ( LPARAM )( LPSTR ) hb_parcx( 4 ) :
                                           ( LPARAM ) hb_parnl( 4 ) ) ) ) )
           );

   if ( ISBYREF( 4 ) )
   {
      hb_storclen( cText, hb_parcsiz( 4 ), 4 );
      hb_xfree( cText );
   }
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SENDDLGITEMMESSAGE )
{
   char     *cText;
   PHB_ITEM pText = hb_param( 5, HB_IT_STRING );

   if( pText )
   {
      cText = ( char* ) hb_xgrab( hb_itemGetCLen( pText ) + 1 );
      hb_xmemcpy( cText, hb_itemGetC( pText ), hb_itemGetCLen( pText ) + 1 );
   }
   else
   {
      cText = NULL;
   }

   hb_retnl( ( LONG ) SendDlgItemMessage( ( HWND ) hb_parnl( 1 ) ,
                                          ( int  ) hb_parni( 2 ) ,
                                          ( UINT ) hb_parni( 3 ) ,
                                          ( ISNIL( 4 ) ? 0 : ( WPARAM ) hb_parnl( 4 ) ),
                                          ( cText ? ( LPARAM ) cText : ( LPARAM ) hb_parnl( 5 ) )
                                        )
           );

  if( pText )
  {
     hb_storclen( cText, hb_itemGetCLen( pText ), 5 ) ;
  }

  if( cText )
  {
     hb_xfree( cText );
  }
}

//-------------------------------------------------------------------//
//
//  WIN_SetTimer( hWnd, nIdentifier, nTimeOut )
//
HB_FUNC( WIN_SETTIMER )
{
   hb_retl( SetTimer( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ), hb_parni( 3 ), NULL ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETFOCUS )
{
   SetFocus( ( HWND ) hb_parnl( 1 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETTEXTCOLOR )
{
   hb_retnl( ( ULONG ) SetTextColor( ( HDC ) hb_parnl( 1 ), ( COLORREF ) hb_parnl( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETBKCOLOR )
{
   hb_retnl( ( ULONG ) SetBkColor( ( HDC ) hb_parnl( 1 ), ( COLORREF ) hb_parnl( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETBKMODE )
{
   hb_retni( ( int ) SetBkMode( ( HDC ) hb_parnl( 1 ), hb_parni( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETSTOCKOBJECT )
{
   hb_retnl( ( ULONG ) GetStockObject( hb_parnl( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_DELETEOBJECT )
{
   hb_retl( DeleteObject( ( HGDIOBJ ) hb_parnl( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SELECTOBJECT )
{
   hb_retnl( ( ULONG ) SelectObject( ( HDC ) hb_parnl( 1 ), ( HGDIOBJ ) hb_parnl( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_LOWORD )
{
   hb_retnl( LOWORD( hb_parnl( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_HIWORD )
{
   hb_retnl( HIWORD( hb_parnl( 1 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_MULDIV )
{
   hb_retni( MulDiv( hb_parni( 1 ), hb_parni( 2 ), hb_parni( 3 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETDIALOGBASEUNITS )
{
   hb_retnl( ( LONG ) GetDialogBaseUnits() ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETMENU )
{
   SetMenu( ( HWND ) hb_parnl( 1 ), ( HMENU ) hb_parni( 2 ) ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETDLGITEMTEXT )
{
   SetDlgItemText( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ), hb_parc( 3 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETDLGITEMTEXT )
{
   USHORT iLen = SendMessage( GetDlgItem( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ) ), WM_GETTEXTLENGTH, 0, 0 ) + 1 ;
   char *cText = ( char* ) hb_xgrab( iLen );

   GetDlgItemText( ( HWND ) hb_parnl( 1 ),   // handle of dialog box
                   hb_parni( 2 ),            // identifier of control
                   ( LPTSTR ) cText,         // address of buffer for text
                   iLen                      // maximum size of string
                 );

   hb_retc( cText );
   hb_xfree( cText );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CHECKDLGBUTTON )
{
   hb_retl( CheckDlgButton( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ),
                            ( UINT )( ISNUM( 3 ) ? hb_parni( 3 ) : hb_parl( 3 ) ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_ISDLGBUTTONCHECKED )
{
   hb_retni( IsDlgButtonChecked( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ) ) ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CHECKRADIOBUTTON )
{
    hb_retl( CheckRadioButton( ( HWND ) hb_parnl( 1 ),   // handle of dialog box
                                        hb_parni( 2 ),   // identifier of first radio button in group
                                        hb_parni( 3 ),   // identifier of last radio button in group
                                        hb_parni( 4 )    // identifier of radio button to select
                              ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETDLGITEM )
{
   hb_retnl( ( ULONG ) GetDlgItem( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_MESSAGEBOX )
{
   hb_retni( MessageBox( ( HWND ) hb_parnl( 1 ), hb_parcx( 2 ), hb_parcx( 3 ), ISNIL( 4 ) ? MB_OK : hb_parni( 4 ) ) ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_INVALIDATERECT )
{
   InvalidateRect( ( HWND ) hb_parnl( 1 ), NULL, TRUE );
}

//-------------------------------------------------------------------//
//
//  Win_LoadIcon( ncIcon )
//
HB_FUNC( WIN_LOADICON )
{
   HICON hIcon;

   if ( ISNUM( 1 ) )
   {
      hIcon = LoadIcon( ( HINSTANCE ) hb_hInstance, MAKEINTRESOURCE( hb_parni( 1 ) ) );
   }
   else
   {
      hIcon = ( HICON ) LoadImage( ( HINSTANCE ) NULL, hb_parc( 1 ), IMAGE_ICON, 0, 0, LR_LOADFROMFILE );
   }

   hb_retnl( ( ULONG ) hIcon ) ;
}

//-------------------------------------------------------------------//
//
//  Win_LoadImage( ncImage, nSource ) -> hImage
//    nSource == 0 ResourceIdByNumber
//    nSource == 1 ResourceIdByName
//    nSource == 2 ImageFromDiskFile
//
HB_FUNC( WIN_LOADIMAGE )
{
   HBITMAP hImage;
   int     iSource = hb_parni( 2 );

   switch ( iSource )
   {
      case 0:
      {
         hImage = LoadBitmap( ( HINSTANCE ) hb_hInstance, MAKEINTRESOURCE( hb_parni( 1 ) ) );
      }
      break;

      case 1:
      {
         hImage = LoadBitmap( ( HINSTANCE ) hb_hInstance, hb_parc( 1 ) );
      }
      break;

      case 2:
      {
         hImage = ( HBITMAP ) LoadImage( ( HINSTANCE ) NULL, hb_parc( 1 ), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
      }
      break;
   }

   hb_retnl( ( ULONG ) hImage ) ;
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETCLIENTRECT )
{
   RECT     rc = { 0 };
   PHB_ITEM info = hb_itemArrayNew( 4 );
   PHB_ITEM temp = hb_itemNew( NULL );

   GetClientRect( ( HWND ) hb_parnl( 1 ), &rc );

   hb_arraySet( info, 1, hb_itemPutNI( temp, rc.left   ) );
   hb_arraySet( info, 2, hb_itemPutNI( temp, rc.top    ) );
   hb_arraySet( info, 3, hb_itemPutNI( temp, rc.right  ) );
   hb_arraySet( info, 4, hb_itemPutNI( temp, rc.bottom ) );

   hb_itemRelease( temp );
   hb_itemReturn( info );
   hb_itemRelease( info );
}

//-------------------------------------------------------------------//
//
//    Win_DrawImage( hdc, nLeft, nTop, nWidth, nHeight, cImage ) in Pixels
//
HB_FUNC( WIN_DRAWIMAGE )
{
   hb_retl( hb_wvt_DrawImage( ( HDC ) hb_parni( 1 ), hb_parni( 2 ), hb_parni( 3 ),
                                   hb_parni( 4 ), hb_parni( 5 ), hb_parc( 6 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETDC )
{
   hb_retnl( ( ULONG ) GetDC( ( HWND ) hb_parnl( 1 )  ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_RELEASEDC )
{
   hb_retl( ReleaseDC( ( HWND ) hb_parnl( 1 ), ( HDC ) hb_parnl( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_RECTANGLE )
{
   Rectangle( ( HDC ) hb_parnl( 1 ), hb_parni( 2 ), hb_parni( 3 ), hb_parni( 4 ), hb_parni( 5 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CREATEBRUSH )
{
   LOGBRUSH lb = { 0 };

   lb.lbStyle = hb_parni( 1 );
   lb.lbColor = ISNIL( 2 ) ? RGB( 0,0,0 ) : ( COLORREF ) hb_parnl( 2 ) ;
   lb.lbHatch = ISNIL( 3 ) ? 0 : hb_parni( 3 );

   hb_retnl( ( ULONG ) CreateBrushIndirect( &lb ) );
}

//-------------------------------------------------------------------//
//
//   Win_DrawText( hDC, cText, aRect, nFormat )
//
HB_FUNC( WIN_DRAWTEXT )
{
   RECT rc = { 0 };

   rc.left   = hb_parni( 3,1 );
   rc.top    = hb_parni( 3,2 );
   rc.right  = hb_parni( 3,3 );
   rc.bottom = hb_parni( 3,4 );

   hb_retl( DrawText( ( HDC ) hb_parnl( 1 ), hb_parc( 2 ), strlen( hb_parc( 2 ) ), &rc, hb_parni( 4 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_GETWINDOWRECT )
{
   RECT rc;
   PHB_ITEM info = hb_itemArrayNew( 4 );
   PHB_ITEM temp = hb_itemNew( NULL );

   GetWindowRect( ( HWND ) hb_parnl( 1 ), &rc );

   hb_arraySet( info, 1, hb_itemPutNI( temp, rc.left   ) );
   hb_arraySet( info, 2, hb_itemPutNI( temp, rc.top    ) );
   hb_arraySet( info, 3, hb_itemPutNI( temp, rc.right  ) );
   hb_arraySet( info, 4, hb_itemPutNI( temp, rc.bottom ) );

   hb_itemRelease( temp );
   hb_itemReturn( info );
   hb_itemRelease( info );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_MOVEWINDOW )
{
   MoveWindow( (HWND) hb_parnl( 1 ), hb_parnl( 2 ), hb_parnl( 3 ), hb_parnl( 4 ), hb_parnl( 5 ), hb_parl( 6 ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETPARENT )
{
   hb_retnl( ( long ) SetParent(  (HWND) hb_parnl( 1 ), (HWND) hb_parnl( 2 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_SETWINDOWLONG )
{
   hb_retnl( SetWindowLong( ( HWND ) hb_parnl( 1 ), hb_parni( 2 ), hb_parnl( 3 ) ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_ISWINDOW )
{
   hb_retl( IsWindow( (HWND) hb_parnl( 1 ) ) );
}

//----------------------------------------------------------------------//

HB_FUNC( WVT_GETFONTHANDLE )
{
   HFONT hFont;
   int   iSlot = hb_parni( 1 )-1;

   if ( iSlot >= 0 && iSlot < WVT_PICTURES_MAX )
   {
      hFont = _s->hUserFonts[ iSlot ];
   }

   hb_retnl( ( ULONG ) hFont );
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_CLIENTTOSCREEN )
{
   POINT    Point ;
   PHB_ITEM pArray = hb_param( 2 , HB_IT_ARRAY );

   if ( wvt_Array2Point( pArray ,&Point ) )
   {
      if ( ClientToScreen( (HWND) hb_parnl( 1 ), &Point ) )
      {
          wvt_Point2ArrayEx( &Point, pArray );
          hb_retl( TRUE ) ;
      }
      else
      {
         hb_retl( FALSE ) ;
      }
   }
   else
   {
      hb_retl( FALSE ) ;
   }
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_SCREENTOCLIENT )
{
   POINT    Point ;
   PHB_ITEM pArray = hb_param( 2 , HB_IT_ARRAY );

   if ( wvt_Array2Point( pArray, &Point ) )
   {
      if( ScreenToClient( (HWND) hb_parnl( 1 ), &Point ) > 0 )
      {
          wvt_Point2ArrayEx( &Point, pArray );
          hb_retl( TRUE ) ;
      }
      else
      {
         hb_retl( FALSE ) ;
      }
   }
   else
   {
      hb_retl( FALSE ) ;
   }
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//
//                     Utility Functions - Not API
//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

BOOL wvt_Array2Rect(PHB_ITEM aRect, RECT *rc )
{
   if (HB_IS_ARRAY(aRect) && hb_arrayLen(aRect) == 4) {
      rc->left   = hb_arrayGetNL(aRect,1);
      rc->top    = hb_arrayGetNL(aRect,2);
      rc->right  = hb_arrayGetNL(aRect,3);
      rc->bottom = hb_arrayGetNL(aRect,4);
      return TRUE ;
   }
   return FALSE;
}

//----------------------------------------------------------------------//

PHB_ITEM wvt_Rect2Array( RECT *rc  )
{
   PHB_ITEM aRect   = hb_itemArrayNew( 4 );
   PHB_ITEM element = hb_itemNew( NULL );

   hb_arraySet(aRect, 1, hb_itemPutNL( element, rc->left   ));
   hb_arraySet(aRect, 2, hb_itemPutNL( element, rc->top    ));
   hb_arraySet(aRect, 3, hb_itemPutNL( element, rc->right  ));
   hb_arraySet(aRect, 4, hb_itemPutNL( element, rc->bottom ));
   hb_itemRelease(element);
   return aRect;
}


//----------------------------------------------------------------------//

BOOL wvt_Array2Point(PHB_ITEM aPoint, POINT *pt )
{
   if (HB_IS_ARRAY(aPoint) && hb_arrayLen(aPoint) == 2) {
      pt->x = hb_arrayGetNL(aPoint,1);
      pt->y = hb_arrayGetNL(aPoint,2);
      return TRUE ;
   }
   return FALSE;
}

//----------------------------------------------------------------------//

PHB_ITEM wvt_Point2Array( POINT *pt  )
{
   PHB_ITEM aPoint = hb_itemArrayNew(2);
   PHB_ITEM element = hb_itemNew(NULL);

   hb_arraySet(aPoint, 1, hb_itemPutNL(element, pt->x));
   hb_arraySet(aPoint, 2, hb_itemPutNL(element, pt->y));
   hb_itemRelease(element);
   return aPoint;
}

//----------------------------------------------------------------------//

BOOL wvt_Array2Size(PHB_ITEM aSize, SIZE *siz )
{
   if (HB_IS_ARRAY(aSize) && hb_arrayLen(aSize) == 2) {
      siz->cx = hb_arrayGetNL(aSize,1);
      siz->cy = hb_arrayGetNL(aSize,2);
      return TRUE ;
   }
   return FALSE;
}

//----------------------------------------------------------------------//

PHB_ITEM wvt_Size2Array( SIZE *siz  )
{
   PHB_ITEM aSize   = hb_itemArrayNew(2);
   PHB_ITEM element = hb_itemNew(NULL);

   hb_arraySet(aSize, 1, hb_itemPutNL(element, siz->cx));
   hb_arraySet(aSize, 2, hb_itemPutNL(element, siz->cy));
   hb_itemRelease(element);
   return aSize;
}

//----------------------------------------------------------------------//

void  wvt_Rect2ArrayEx( RECT *rc ,PHB_ITEM aRect )
{
   PHB_ITEM element = hb_itemNew(NULL);

   hb_arraySet(aRect, 1, hb_itemPutNL(element, rc->left));
   hb_arraySet(aRect, 2, hb_itemPutNL(element, rc->top));
   hb_arraySet(aRect, 3, hb_itemPutNL(element, rc->right));
   hb_arraySet(aRect, 4, hb_itemPutNL(element, rc->bottom));
   hb_itemRelease(element);
}

//----------------------------------------------------------------------//

void wvt_Point2ArrayEx( POINT *pt, PHB_ITEM aPoint)
{

   PHB_ITEM element = hb_itemNew(NULL);

   hb_arraySet(aPoint, 1, hb_itemPutNL(element, pt->x));
   hb_arraySet(aPoint, 2, hb_itemPutNL(element, pt->y));
   hb_itemRelease(element);
}

//----------------------------------------------------------------------//

void wvt_Size2ArrayEx( SIZE *siz, PHB_ITEM aSize )
{
   PHB_ITEM element = hb_itemNew(NULL);

   hb_arraySet(aSize, 1, hb_itemPutNL(element, siz->cx));
   hb_arraySet(aSize, 2, hb_itemPutNL(element, siz->cy));
   hb_itemRelease(element);
}

//----------------------------------------------------------------------//

HB_FUNC( WVT__GETOPENFILENAME )
{
    int size = ( ISNIL( 2 ) ? 1024 : hb_parcsiz( 2 ) );
    OPENFILENAME ofn ;

    char * szFileName = ( char* ) hb_xgrab( size );

    strcpy( szFileName, ( ISNIL( 2 ) ? "" : hb_parc( 2 ) ) );

    ZeroMemory( &ofn, sizeof( ofn ) );

    ofn.hInstance       = GetModuleHandle( NULL )  ;
    ofn.lStructSize     = sizeof( ofn );
    ofn.hwndOwner       = ISNIL   (1) ? GetActiveWindow() : ( HWND ) hb_parnl( 1 ) ;
    ofn.lpstrTitle      = ISNIL   (3) ? NULL : hb_parc ( 3 ) ;
    ofn.lpstrFilter     = ISNIL   (4) ? NULL : hb_parc ( 4 );
    ofn.Flags           = ISNIL   (5) ? OFN_SHOWHELP : hb_parnl( 5 ) ;
    ofn.lpstrInitialDir = ISNIL   (6) ? NULL : hb_parc ( 6 );
    ofn.lpstrDefExt     = ISNIL   (7) ? NULL : hb_parc ( 7 );
    ofn.nFilterIndex    = ISNIL   (8) ? NULL : hb_parni( 8 );
    ofn.lpstrFile       = szFileName;
    ofn.nMaxFile        = size;

    if( GetOpenFileName( &ofn ) )
    {
      hb_stornl( ofn.nFilterIndex, 8 );
      hb_storclen( szFileName, size, 2 ) ;
      hb_retclen( ( char * ) ofn.lpstrFile, size );
    }
    else
    {
      hb_retc( "" );
    }
    hb_xfree( szFileName );
}

//----------------------------------------------------------------------//

HB_FUNC( WVT__GETSAVEFILENAME )
{
   HINSTANCE hInstance;
   OPENFILENAME ofn;
   char szFileName[ MAX_PATH+1 ] ;

   hInstance = GetModuleHandle( NULL );

   strcpy( szFileName, hb_parcx( 2 ) );

   ZeroMemory( &ofn, sizeof( ofn ) );

   ofn.hInstance       = hInstance  ;
   ofn.lStructSize     = sizeof( ofn );
   ofn.hwndOwner       = ISNIL   (1) ? GetActiveWindow() : (HWND) hb_parnl( 1 );
   ofn.lpstrTitle      = hb_parc (3);
   ofn.lpstrFilter     = hb_parc (4);
   ofn.Flags           = (ISNIL  (5) ? OFN_FILEMUSTEXIST|OFN_EXPLORER : hb_parnl( 4 ) );
   ofn.lpstrInitialDir = hb_parc (6);
   ofn.lpstrDefExt     = hb_parc (7);
   ofn.nFilterIndex    = hb_parni(8);
   ofn.lpstrFile       = szFileName;
   ofn.nMaxFile        = MAX_PATH;

   if( GetSaveFileName( &ofn ) )
   {
      hb_stornl( ofn.nFilterIndex, 8 );
      hb_retc( ofn.lpstrFile );
   }
   else
   {
      hb_retc( "" );
   }
}
//----------------------------------------------------------------------//

HB_FUNC( WIN_AND )
{
   hb_retnl( hb_parnl(1) & hb_parnl(2) ) ;
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_OR )
{
   hb_retnl( hb_parnl(1) | hb_parnl(2) ) ;
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_NOT )
{
   hb_retnl( ~( hb_parnl(1) ) ) ;
}

//----------------------------------------------------------------------//

HB_FUNC( WIN_TRACKPOPUPMENU )
{
   HMENU hMenu  = ( HMENU ) hb_parnl( 1 );
   UINT  uFlags = ISNIL( 2 ) ? TPM_CENTERALIGN | TPM_RETURNCMD : hb_parnl( 2 );
   HWND  hWnd   = ISNIL( 3 ) ? GetActiveWindow() : ( HWND ) hb_parnl( 3 );

   POINT xy = { 0 };

   GetCursorPos( &xy );

   hb_retnl( TrackPopupMenu( hMenu, uFlags, xy.x, xy.y, 0, hWnd, NULL ) );
}

//-------------------------------------------------------------------//

HB_FUNC( WIN_CHOOSECOLOR )
{
   CHOOSECOLOR cc ;
   COLORREF    crCustClr[ 16 ] ;
   int         i ;

   for( i = 0 ; i < 16 ; i++ )
   {
     crCustClr[ i ] = ( ISARRAY( 2 ) ? ( COLORREF ) hb_parnl( 2, i+1 ) : GetSysColor( COLOR_BTNFACE ) ) ;
   }

   cc.lStructSize   = sizeof( CHOOSECOLOR ) ;
   cc.hwndOwner     = ISNIL( 4 ) ? NULL : (HWND) hb_parnl( 4 );
   cc.rgbResult     = ISNIL( 1 ) ?  0 : ( COLORREF ) hb_parnl( 1 ) ;
   cc.lpCustColors  = crCustClr ;
   cc.Flags         = ( WORD ) ( ISNIL( 3 ) ? CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN : hb_parnl( 3 ) );

   if ( ChooseColor( &cc ) )
   {
      hb_retnl( cc.rgbResult ) ;
   }
   else
   {
      hb_retnl( -1 );
   }
}

//-------------------------------------------------------------------//


