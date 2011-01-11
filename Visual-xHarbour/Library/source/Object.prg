/*
 * $Id$
 */
//------------------------------------------------------------------------------------------------------*
//                                                                                                      *
// Object.prg                                                                                           *
//                                                                                                      *
// Copyright (C) xHarbour.com Inc. http://www.xHarbour.com                                              *
//                                                                                                      *
//  This source file is an intellectual property of xHarbour.com Inc.                                   *
//  You may NOT forward or share this file under any conditions!                                        *
//------------------------------------------------------------------------------------------------------*

#include "vxh.ch"
#include "debug.ch"
#include "error.ch"

CLASS Object
   ACCESS Instance             INLINE   IIF( __GetApplication() != NIL, __GetApplication():Instance, GetModuleHandle() )

   PROPERTY Theming READ xTheming WRITE __SetTheming DEFAULT .T. PROTECTED

   DATA hWnd                   EXPORTED
   
   DATA Error                  EXPORTED
   DATA GenerateMember         PUBLISHED INIT .T.
   DATA Parent                 EXPORTED
   DATA ThemeName              EXPORTED
   DATA hTheme                 EXPORTED

   DATA __xCtrlName            EXPORTED

   DATA __ForceSysColor        EXPORTED INIT .F.
   DATA __ClassInst            EXPORTED
   DATA __lCopyCut             EXPORTED INIT .T.
   DATA __IsControl            EXPORTED INIT .F.
   DATA __IsStandard           EXPORTED INIT .F.
   DATA __IdeImageIndex        EXPORTED
   DATA __hIcon                EXPORTED
   DATA __DockChildren         EXPORTED
   DATA __CustomOwner          EXPORTED INIT .F.
   DATA __OriginalName         EXPORTED
   DATA InvisibleAtRuntime     EXPORTED INIT .F.

   DATA ClsName                EXPORTED
   DATA Children               EXPORTED INIT {}
   DATA ExtraChildren          EXPORTED INIT {}
   DATA Cargo                  EXPORTED
   DATA TreeItem               EXPORTED
   
   DATA Owner                  EXPORTED
   DATA Components             EXPORTED INIT {}
   DATA Property               EXPORTED

   DATA EventHandler           EXPORTED
   DATA __OnInitCanceled       EXPORTED INIT .F.
   DATA __InstApp              EXPORTED
   DATA xName                  EXPORTED
   ACCESS Name                 INLINE ::xName PERSISTENT
   ASSIGN Name(c)              INLINE ::__SetCtrlName(c)

   ACCESS Application          INLINE IIF( ::__InstApp != NIL, ::__InstApp, __GetApplication() )
   ACCESS System               INLINE __GetSystem()

   DATA Events                 EXPORTED

   ACCESS Form                 INLINE IIF( ::ClsName == "VXH_FORM_IDE" .OR. ::Parent == NIL, Self, ::Parent:Form )
   ACCESS This                 INLINE Self


   METHOD HasMessage( cMsg )   INLINE __ObjHasMsg( Self, cMsg )
   METHOD HasProperty( cName ) INLINE ::Property != NIL .AND. hScan( ::Property, cName ) > 0
   METHOD __SetCtrlName()
   METHOD GetControlName()
   METHOD __CreateProperty()
   METHOD __SetAsProperty()
   METHOD __SetTheming()

   METHOD SetWindowTheme( cSubAppName, cSubIdList ) INLINE SetWindowTheme( ::hWnd, ToUnicode( cSubAppName ), ToUnicode( cSubIdList ) )
   METHOD RemoveWindowTheme()                       INLINE SetWindowTheme( ::hWnd, ToUnicode( "" ), ToUnicode( "" ) )
   METHOD Create()
   METHOD __InvalidMember()
   METHOD RemoveProperty()
   error HANDLER OnError()

ENDCLASS

//-----------------------------------------------------------------------------------------------------------------------------
METHOD OnError( ... ) CLASS Object
   LOCAL cMsg, n, hProperty, oParent, uRet, aParams := HB_AParams()
   cMsg := __GetMessage()
   
   IF PCount() == 0 .AND. ::Property != NIL
      IF hScan( ::Property, {|c| UPPER(c) == UPPER(cMsg) } ) > 0
         uRet := ::Property[ cMsg ]
       ELSE
         uRet := ::__InvalidMember( cMsg )
      ENDIF
    ELSEIF !EMPTY( cMsg )
      IF PCount() == 0

         IF ( __clsParent( ::ClassH, "CUSTOMCONTROL" ) .OR. __clsParent( ::Parent:ClassH, "CUSTOMCONTROL" ) )
            RETURN ::Form:&cMsg //::Form:Property[ cMsg ]
         ENDIF
      ENDIF

      uRet := ::__InvalidMember( cMsg )
   ENDIF
RETURN uRet

//-----------------------------------------------------------------------------------------------------------------------------
METHOD __InvalidMember( cMsg ) CLASS Object
   LOCAL uRet, oErr := ErrorNew()
   oErr:Args          := { Self, cMsg,  }
   oErr:CanDefault    := .F.
   oErr:CanRetry      := .F.
   oErr:CanSubstitute := .T.
   oErr:Description   := "Invalid Class Member"
   oErr:GenCode       := EG_NOVARMETHOD
   oErr:Operation     := cMsg
   oErr:Severity      := ES_ERROR
   oErr:SubCode       := -1
   oErr:SubSystem     := ::classname
   uRet := Eval( ErrorBlock(), oErr )
RETURN uRet

//-----------------------------------------------------------------------------------------------------------------------------
METHOD __SetTheming( lSet ) CLASS Object
   IF ::hWnd != NIL
      IF !lSet
         ::RemoveWindowTheme()
       ELSEIF ::ThemeName != NIL
         ::SetWindowTheme( , ::ThemeName )
      ENDIF
   ENDIF
   AEVAL( ::Children, {|o|o:Theming := lSet } )
RETURN Self

//-----------------------------------------------------------------------------------------------------------------------------
METHOD __SetCtrlName(c) CLASS Object
   LOCAL n, cProp, nProp
   IF !(::Name == c) .AND. ::Form != NIL
      ::Form:__SetAsProperty( c, Self )
      ::xName := c
   ENDIF
RETURN c

//-----------------------------------------------------------------------------------------------------------------------------
METHOD __CreateProperty( cBaseName ) CLASS Object
   LOCAL n, aProp, nPos, cName

   DEFAULT cBaseName TO ::__xCtrlName
   IF ::Form != NIL .AND. ::__xCtrlName != "ToolTip"
      IF !( ::Caption == "[ Add New Item ]" )
         n := ::GetControlName( cBaseName )
         ::Form:__SetAsProperty( cBaseName + ALLTRIM( STR( n ) ), Self )
      ENDIF
   ENDIF
RETURN SELF

//-----------------------------------------------------------------------------------------------------------------------------
METHOD GetControlName( cName, lIde ) CLASS Object
   LOCAL o, cProp, n := 1, oControl, lCont, lComp := .T., oForm := ::Form
   WHILE ::Application != NIL .AND. oForm != NIL .AND. oForm:Property != NIL
      cProp := cName + XSTR( n )
      IF hScan( oForm:Property, {|c| UPPER(c) == UPPER(cProp) } ) == 0
         EXIT
      ENDIF
      n ++
   ENDDO
RETURN n

//-----------------------------------------------------------------------------------------------------------------------------
METHOD __SetAsProperty( cName, oObj ) CLASS Object
   LOCAL n

   IF oObj:ClsName == TOOLTIPS_CLASS .OR. ::Property == NIL .OR. (::Application != NIL .AND. !::Application:GenerateMembers)
      RETURN Self
   ENDIF
   IF oObj:ClsName == "AtlAxWin" .AND. oObj:xName != NIL .AND. ! ( oObj:xName == cName ) .AND. procname(4) == "USERCONTROL:INIT"
      cName := oObj:xName
   ENDIF
   IF !( oObj == Self ) //.AND. ::__ClassInst == NIL
      IF !EMPTY( oObj:xName ) .AND. ( n := hScan( ::Property, {|c| UPPER(c) == UPPER(oObj:xName) } ) ) > 0
         HDelAt( ::Property, n )
      ENDIF
      ::Property[ cName ] := oObj
   ENDIF
   oObj:xName := cName
RETURN Self

//-----------------------------------------------------------------------------------------------------------------------------
METHOD Create() CLASS Object
   LOCAL nRet := ExecuteEvent( "OnInit", Self )
   
   IF ::__ClassInst != NIL
      ::Application:ObjectTree:Set( Self )
   ENDIF
   IF VALTYPE( nRet ) == "N" .AND. nRet == 0
      ::__OnInitCanceled := .T.
   ENDIF

   IF !::GenerateMember .OR. ::__OnInitCanceled
      ::RemoveProperty()
   ENDIF
RETURN Self

METHOD RemoveProperty() CLASS Object
   LOCAL n
   IF /*::__ClassInst == NIL .AND.*/ !EMPTY( ::xName ) .AND. ( n := hScan( ::Form:Property, {|c| UPPER(c) == UPPER(::xName) } ) ) > 0
      RETURN HDelAt( ::Form:Property, n )
   ENDIF
RETURN NIL
