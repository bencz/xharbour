/*
 * $Id: TCStatusBar.prg,v 1.12 2002/10/13 11:16:29 what32 Exp $
 */
/*
 * xHarbour Project source code:
 *
 * Whoo.lib TStatusBar CLASS
 *
 * Copyright 2002 Augusto Infante [augusto@2vias.com.ar]
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
 */

#Include "windows.ch"
#Include "statbar.ch"
#Include "hbclass.ch"
#include "wintypes.ch"
#include "what32.ch"
#Include "cstruct.ch"
#Include "debug.ch"

*------------------------------------------------------------------------------*

CLASS TStatusBar FROM TCustomControl

//   DATA Caption INIT  "StatusBar"
   DATA Left    INIT   0
   DATA Top     INIT   0
   DATA Width   INIT   0
   DATA Height  INIT   0

   DATA Style   INIT  WS_CHILD + WS_VISIBLE

   DATA lRegister PROTECTED INIT .F.
   DATA lControl  PROTECTED INIT .T.
   DATA Msgs      PROTECTED INIT {WM_DESTROY,WM_SIZE,WM_MOVE}
   DATA WndProc   PROTECTED INIT 'ControlProc'
   DATA Name      PROTECTED INIT "msctls_statusbar32"


   DATA rect PROTECTED

   METHOD New() CONSTRUCTOR
   METHOD Create() INLINE ::handle := CreateStatusBar( ::Style, ::Caption, ::Parent:handle, ::Id  )
   METHOD SetPanels
   METHOD SetPanelText
   METHOD GetHeight
   METHOD GetPanelRect
   METHOD SetPanelIcon
   ACCESS height INLINE ::GetHeight()

ENDCLASS

*------------------------------------------------------------------------------*

METHOD New( oParent, cCaption, nId ) CLASS TStatusBar

   ::id        := nId
   ::Caption   := cCaption
   ::Style     := WS_CHILD + WS_VISIBLE

   RETURN( super:new( oParent ) )

*------------------------------------------------------------------------------*

METHOD SetPanels(aParts) CLASS TStatusBar

   LOCAL nLeft,n,aRect,bSizes := ""

   AEVAL(aParts,{|x| bSizes+=L2BIN(x)})

   RETURN( ::SendMessage( SB_SETPARTS, LEN( aParts ), bSizes ))

*------------------------------------------------------------------------------*

METHOD SetPanelText( nPart, cText ) CLASS TStatusBar

   RETURN( ::SendMessage( SB_SETTEXT, nPart, cText ))

*------------------------------------------------------------------------------*

METHOD GetHeight() CLASS TStatusBar

   ::rect := ::WindowRect()

   RETURN(::rect[4]-::rect[2])

*------------------------------------------------------------------------------*

METHOD GetPanelRect(nPanel) CLASS TStatusBar

   local aRect,aPt

   aRect:=StatusBarGetRect(::handle,nPanel)
   aRect[3]-=aRect[1]
   aRect[4]-=aRect[2]

   RETURN(aRect)

*------------------------------------------------------------------------------*

METHOD SetPanelIcon(nPanel,hIcon) CLASS TStatusBar

   SetStatusIcon( ::handle, 0, hIcon )

   RETURN(self)

*------------------------------------------------------------------------------*
