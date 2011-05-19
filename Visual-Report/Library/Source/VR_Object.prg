/*
 * $Id$
 */

//-----------------------------------------------------------------------------------------------
// Copyright   WinFakt! / SOCS BVBA  http://www.WinFakt.com
//
// This source file is an intellectual property of SOCS bvba.
// You may NOT forward or share this file under any conditions!
//-----------------------------------------------------------------------------------------------

#include "debug.ch"
#include "vxh.ch"
#define PIX_PER_INCH   1440


#define  acObjectTypeText           5

CLASS VrObject
   DATA Font        PUBLISHED

   DATA lUI         EXPORTED INIT .T.
   DATA Name        EXPORTED
   DATA Text        EXPORTED
   DATA Objects     EXPORTED INIT {}

   DATA Left        EXPORTED INIT 0
   DATA Top         EXPORTED INIT 0
   DATA Width       EXPORTED INIT 150
   DATA Height      EXPORTED INIT 150

   DATA Alignment   EXPORTED INIT 0
   DATA EnumAlignment EXPORTED INIT { { "No Alignment", "Left", "Center", "Right" }, {0,1,2,3} }

   DATA Application EXPORTED
   DATA System      EXPORTED
   DATA Parent      EXPORTED
   DATA ClsName     EXPORTED
   DATA __ClsInst   EXPORTED
   DATA aProperties INIT { { "Left", "Position" },;
                           { "Top",  "Position" },;
                           { "Alignment",  "Position" } }
   ACCESS EditMode  INLINE ::__ClsInst != NIL
   ACCESS __xCtrlName INLINE ::ClsName
   
   DATA EditCtrl    EXPORTED
   DATA PDFCtrl     EXPORTED
   
   DATA Report      EXPORTED
   DATA nPixPerInch EXPORTED INIT PIX_PER_INCH
   
   METHOD Init() CONSTRUCTOR
   METHOD GetValue( cVal ) INLINE ::&cVal
   METHOD SetControlName()
   METHOD Create()
   METHOD SetSize()
   METHOD Draw()           VIRTUAL
   METHOD FillRect()
   METHOD MoveWindow()     INLINE ::EditCtrl:MoveWindow( ::Left, ::Top )
   METHOD WriteProps()     VIRTUAL
   METHOD Configure()      VIRTUAL
   METHOD Delete()
   METHOD __GetDataSource()
ENDCLASS

METHOD Init( oParent ) CLASS VrObject
   ::Application := __GetApplication()
   ::System      := __GetSystem()
   ::Parent      := oParent
   IF ::Parent != NIL
      ::SetControlName()
   ENDIF
   ::Font := Font()
RETURN Self

METHOD __GetDataSource( cDataSource ) CLASS VrObject
   LOCAL n, oSource
   IF ( n := ASCAN( ::Objects, {|o| o:Name == cDataSource} ) ) > 0
      oSource := ::Objects[n]
   ENDIF
RETURN oSource

METHOD Create() CLASS VrObject
   IF ::Parent != NIL
      AADD( IIF( ::lUI, ::Parent:Objects, ::Application:Props:CompObjects ), Self )
      IF ::lUI
         ::EditCtrl:OnWMLButtonDown := {|| ::Application:Props:PropEditor:ResetProperties( {{ Self }} ) }
      ENDIF
   ENDIF
RETURN Self

METHOD Delete() CLASS VrObject
   LOCAL n := ASCAN( ::Parent:Objects, Self,,, .T. )
   IF n > 0
      ADEL( ::Parent:Objects, n, .T. )
      ::EditCtrl:Destroy()
      ::Application:Props:Components:Children[1]:Select():SelectComponent()
      ::Parent:InvalidateRect()
   ENDIF
RETURN Self

METHOD SetControlName() CLASS VrObject
   LOCAL cProp, n := 1
   IF UPPER( ::Parent:ClassName ) != "VRREPORT"
      WHILE .T.
         cProp := ::ClsName + XSTR( n )
         IF ASCAN( ::Parent:Objects, {|o| UPPER(o:Name) == UPPER(cProp) } ) == 0
            EXIT
         ENDIF
         n ++
      ENDDO
      ::Name := cProp
      ::Text := cProp
   ENDIF
RETURN n

METHOD SetSize( cx, cy ) CLASS VrObject
   LOCAL aRect
   DEFAULT cx TO ::Width
   DEFAULT cy TO ::Height
   
   WITH OBJECT ::EditCtrl
      :Parent:nDownPos := {0,0}
      aRect := :GetRectangle()
      aRect := {aRect[1]-1, aRect[2]-1, aRect[3]+1, aRect[4]+1}
      :Parent:InvalidateRect( aRect, .F. )

      :xWidth  := cx
      :xHeight := cy
      :MoveWindow()

      :Parent:nDownPos := NIL
      aRect := :GetRectangle()
      aRect := {aRect[1]-1, aRect[2]-1, aRect[3]+1, aRect[4]+1}
      :Parent:InvalidateRect( aRect, .F. )
   END
RETURN Self

METHOD FillRect( aRect, cColor ) CLASS VrObject
   LOCAL cName := "Box" + AllTrim( Str( ::nBox++ ) )

   WITH OBJECT ::Parent:oPDF
      :CreateObject( acObjectTypeText, cName )
      :ObjectAttribute( cName, "BackColor", IIF( cColor != NIL, cColor, PADL( DecToHexa( ::BackColor ), 6, "0" ) ) )

      :ObjectAttribute( cName, "Left",   aRect[1] )
      :ObjectAttribute( cName, "Top",    aRect[2] )
      :ObjectAttribute( cName, "Right",  aRect[3] )
      :ObjectAttribute( cName, "Bottom", aRect[4] )
   END
RETURN Self

