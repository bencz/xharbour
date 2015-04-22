/*
 * $Id$
 */
//------------------------------------------------------------------------------------------------------*
//                                                                                                      *
// MDIChild.prg                                                                                         *
//                                                                                                      *
// Copyright (C) xHarbour.com Inc. http://www.xHarbour.com                                              *
//                                                                                                      *
//  This source file is an intellectual property of xHarbour.com Inc.                                   *
//  You may NOT forward or share this file under any conditions!                                        *
//------------------------------------------------------------------------------------------------------*

#include "vxh.ch"
#include "debug.ch"

#define ATL_IDM_FIRST_MDICHILD 50000
#define IDM_MDI_BASE      (ATL_IDM_FIRST_MDICHILD - 5)
#define IDM_MDI_ICON      (IDM_MDI_BASE + 0)
#define IDM_MDI_GAP       (IDM_MDI_BASE + 1)
#define IDM_MDI_MINIMIZE  (IDM_MDI_BASE + 2)
#define IDM_MDI_RESTORE   (IDM_MDI_BASE + 3)
#define IDM_MDI_CLOSE     (IDM_MDI_BASE + 4)
#define IDM_MDI_FIRST     IDM_MDI_ICON
#define IDM_MDI_LAST      IDM_MDI_CLOSE

#xcommand ODEFAULT <v> TO <x> [, <vN> TO <xN>]         ;
       => IF <v> == nil .OR. VALTYPE( <v> ) == "O"; <v> := <x> ; END            ;
          [; IF <vN> == nil ; <vN> := <xN> ; END]

//-----------------------------------------------------------------------------------------------

CLASS MDIChildWindow INHERIT WinForm
   ACCESS Form       INLINE Self

   DATA __oCoolMenu  PROTECTED
   METHOD Init()  CONSTRUCTOR
   METHOD __WinProc( hWnd, nMsg, nwParam, nlParam ) INLINE DefMDIChildProc( hWnd, nMsg, nwParam, nlParam )
   METHOD Create()//                                INLINE ::Super:Create(), ::InvalidateRect(), Self
   METHOD ResetMDIMenu()
   METHOD Show()
ENDCLASS

//-----------------------------------------------------------------------------------------------

METHOD Init( oParent, aParams, cProjectName ) CLASS MDIChildWindow
   DEFAULT ::__xCtrlName TO "MDIChild"
   ::xMDIChild   := .T.
   ::Modal       := .F.
   ::ClsName     := "MDIChild"
   ::Super:Init( IIF( VALTYPE( oParent ) == "O", oParent:MDIClient, oParent ), aParams, cProjectName )
   ::ClassStyle  := CS_VREDRAW | CS_HREDRAW
   ::IsContainer := .F.
RETURN Self

//-----------------------------------------------------------------------------------------------

/*
   LOCAL mcs := (struct MDICREATESTRUCT)

   mcs:szClass := ::ClsName
   mcs:x       := ::Left
   mcs:y       := ::Top
   mcs:cx      := ::Width
   mcs:cy      := ::Height
   mcs:hOwner  := ::Application:Instance
   mcs:style   := 0
   mcs:szTitle := ::Text

   IF ! IsRegistered( ::Instance, ::ClsName )
      ::__Register()
   ENDIF

   ::hWnd := SendMessage( ::Parent:hWnd, WM_MDICREATE, 0, mcs )
*/

METHOD Create() CLASS MDIChildWindow
   LOCAL oItem, nPos, o, n, cText
   ::ControlParent := .T.

   ::Style := ::Style & NOT( WS_POPUP )
   ::Style := ::Style & NOT( WS_CHILD )
   ::Style := ::Style | WS_OVERLAPPED

   ::ExStyle := ::ExStyle | WS_EX_MDICHILD
   ::Super:Create()

   ::Style := GetWindowLong( ::hWnd, GWL_STYLE )

   ::SetStyle( WS_MAXIMIZEBOX, ::MaximizeBox )
   ::SetStyle( WS_MINIMIZEBOX, ::MinimizeBox )
   ::SetStyle( WS_SYSMENU, ::Sysmenu )

   AADD( ::Parent:Children, Self )

   IF ::__hParent != NIL
      InvalidateRect( ::hWnd )
   ENDIF
   IF ::Parent != NIL .AND. ::Parent:WindowsMenu .AND. ( n := ASCAN( ::Parent:Parent:Children, {|o| o:__xCtrlName == "CoolMenu"} ) ) > 0
      ::__oCoolMenu := ::Parent:Parent:Children[n]
      IF ASCAN( ::__oCoolMenu:aItems, {|o| o:MDIMenu == .T. } ) == 0

         FOR n := 1 TO len( ::__oCoolMenu:aItems )
             cText := UPPER( ::__oCoolMenu:aItems[n]:Caption )
             IF "HELP" IN cText
                nPos := n-1
                EXIT
             ENDIF
         NEXT

         o := CoolMenuItem( ::__oCoolMenu )
         o:Caption := "&Windows"
         o:MDIMenu := .T.
         o:Create( nPos )

         oItem := MenuItem( o )
         oItem:Caption:= "Tile Horizontally"
         oItem:Action := {|o| o:Menu:Parent:Parent:MdiTileHorizontal() }
         oItem:Create()
         oItem := MenuItem( o )
         oItem:Caption:= "Tile Vertically"
         oItem:Action := {|o| o:Menu:Parent:Parent:MdiTileVertical() }
         oItem:Create()
         oItem := MenuItem( o )
         oItem:Caption:= "Cascade"
         oItem:Action := {|o| o:Menu:Parent:Parent:MdiCascade() }
         oItem:Create()
         oItem := MenuItem( o )
         oItem:Caption:= "Arrange Icons"
         oItem:Action := {|o| o:Menu:Parent:Parent:MdiIconArrange() }
         oItem:Create()
         oItem := MenuItem( o )
         oItem:Caption:= "-"
         oItem:Create()
      ENDIF
      ::ResetMDIMenu()
   ENDIF
RETURN Self

METHOD ResetMDIMenu() CLASS MDIChildWindow
   LOCAL oMenu, oChild, oItem, n
   IF ::__oCoolMenu != NIL .AND. ( n := ASCAN( ::__oCoolMenu:aItems, {|o| o:MDIMenu == .T. } ) ) > 0
      oMenu := ::__oCoolMenu:aItems[n]
      FOR n := 1 TO LEN( oMenu:Children )
          IF n > 5
             oMenu:Children[n]:Delete()
             n--
          ENDIF
      NEXT
      n := 1
      FOR EACH oChild IN ::Parent:Children
          IF ! Empty( oChild:Text )
             oItem := MenuItem( oMenu )
             oItem:Caption:= "&"+ALLTRIM(STR(n))+" "+oChild:Text
             oItem:Cargo  := oChild
             oItem:Action := {|o| o:Cargo:MdiActivate() }
             oItem:Create()
             n++
          ENDIF
      NEXT
   ENDIF
RETURN Self

METHOD Show( nShow ) CLASS MDIChildWindow
   LOCAL nRet
   DEFAULT nShow TO ::ShowMode

   IF nShow == SW_HIDE
      RETURN ::Hide()
   ENDIF

   IF ::hWnd != NIL
      IF !::__lShown
         ::__lShown := .T.
         nRet := ExecuteEvent( "OnLoad", Self )
         ODEFAULT nRet TO ::OnLoad( Self )
      ENDIF
      ShowWindow( ::hWnd, IIF( ! ::DesignMode, nShow, SW_SHOW ) )
   ENDIF

   DO CASE
      CASE nShow == SW_SHOWMAXIMIZED
           SendMessage( ::Parent:hWnd, WM_MDIMAXIMIZE, ::hWnd )
      OTHERWISE
           ::SetWindowPos(,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER)
   ENDCASE
   ::Style := ::Style | WS_VISIBLE
RETURN Self

//5I5B4M
