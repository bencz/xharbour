/*
*-----------------------------------------------------------------------------
* WoopGUI for Harbour - Win32 OOP GUI library source code
* Copyright 2002 Francesco Saverio Giudice <info@fsgiudice.com>
*
*-----------------------------------------------------------------------------
* Parts of this project come from:
* "Harbour MiniGUI"
*                   Copyright 2002 Roberto Lopez <roblez@ciudad.com.ar>
*                   http://www.geocities.com/harbour_minigui/
* "Harbour GUI framework for Win32"
*                   Copyright 2001 Alexander S.Kresin <alex@belacy.belgorod.su>
*                   Copyright 2001 Antonio Linares <alinares@fivetech.com>
*                   http://www.harbour-project.org
*-----------------------------------------------------------------------------
*
*/
/*------------------------------------------------------------------------------
* Low Level C Generic Routines
*------------------------------------------------------------------------------*/
#define _WIN32_WINNT 0x0400
#define WINVER 0x0400
#define _WIN32_IE 0x0501

#include <windows.h>
#include <commctrl.h>
#include "hbapi.h"
#include "hbvm.h"
#include "hbstack.h"
#include "hbapiitm.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))


HB_FUNC ( GET_X_LPARAM )
{
  LPARAM lParam = hb_parnl( 1 );
  hb_retni( GET_X_LPARAM( lParam ) );
}

//----------------------------------------------------------------------------

HB_FUNC ( GET_Y_LPARAM )
{
  LPARAM lParam = hb_parnl( 1 );
  hb_retni( GET_Y_LPARAM( lParam ) );
}

//----------------------------------------------------------------------------

HB_FUNC ( GETSTRINGPTR )
{
   char *cString = hb_parcx( 1 );
   hb_retnl( ( LONG_PTR) cString );
}

