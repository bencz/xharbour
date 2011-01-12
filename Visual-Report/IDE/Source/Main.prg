/*
 * $Id$
 */

// Copyright   WinFakt! / SOCS BVBA  http://www.WinFakt.com
//
// This source file is an intellectual property of SOCS bvba.
// You may NOT forward or share this file under any conditions!

#include "vxh.ch"
#include "cstruct.ch"
#include "debug.ch"

static oApp

PROCEDURE Main( cFile )
   SET CENTURY ON
   SET AUTOPEN OFF

   RepApp( cFile )
   QUIT
RETURN

//-------------------------------------------------------------------------------------------------------

CLASS RepApp INHERIT Application
   DATA Props    EXPORTED INIT {=>}
   DATA Report   EXPORTED
   
   ACCESS Project INLINE ::Report
   METHOD Init() CONSTRUCTOR
ENDCLASS

METHOD Init( cFile ) CLASS RepApp
   oApp := Self
   HSetCaseMatch( ::Props, .F. )
   ::Super:Init( NIL )
   MainForm( NIL )
   ::Run()
RETURN Self

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

CLASS MainForm FROM WinForm
   METHOD Init() CONSTRUCTOR
   METHOD OnClose()
ENDCLASS

METHOD Init() CLASS MainForm
   LOCAL aEntries, cReport, oItem, aComp, n, i
   ::Super:Init()
   
   ::Caption := "Visual Report"
   ::Icon    := "__VR"
   ::xName   := "VR"
   
   ::BackColor := ::System:CurrentScheme:ToolStripPanelGradientEnd
   ::OnWMThemeChanged := {|o| o:BackColor := ::System:CurrentScheme:ToolStripPanelGradientEnd }
   ::Application:Report := Report()

   ::Create()

   // StatusBar section ----------------------
   WITH OBJECT StatusBar( Self )
      :ImageList := ImageList( :this, 16, 16 ):Create()
      :ImageList:AddIcon( "__VR" )
      :Create()
      WITH OBJECT StatusBarPanel( :this )
         :ImageIndex := 1
         :Caption  := "WinFakt! Visual Reporter. Copyright "+CHR(169)+" "+Str(Year(Date()))+" WinFakt! / SOCS BVBA http://www.WinFakt.com. All rights reserved"
         :Width    := :Parent:Drawing:GetTextExtentPoint32( :Caption )[1] + 40
         :Create()
      END
   END

   //--------------------------------------
   WITH OBJECT MenuStrip( ToolStripContainer( Self ):Create() )

      :Row := 1
      :ImageList := ImageList( :this, 16, 16 ):Create()
      :ImageList:AddImage( IDB_STD_SMALL_COLOR )
      :ImageList:MaskColor := ::System:Color:Cyan
      :Create()

      WITH OBJECT ::Application:Props[ "FileMenu" ] := MenuStripItem( :this )
         :ImageList := :Parent:ImageList
         :Caption := "&File"
         :Create()

         WITH OBJECT MenuStripItem( :this )
            :Caption           := "&New"
            :ImageIndex        := ::System:StdIcons:FileNew
            :ShortCutText      := "Ctrl+N"
            :ShortCutKey:Ctrl  := .T.
            :ShortCutKey:Key   := ASC( "N" )
            :Action            := {|o| ::Application:Report := Report():New() }
            :Create()
         END

         WITH OBJECT MenuStripItem( :this )
            :Caption           := "&Open"
            :ImageIndex        := ::System:StdIcons:FileOpen
            :ShortCutText      := "Ctrl+O"
            :ShortCutKey:Ctrl  := .T.
            :ShortCutKey:Key   := ASC( "O" )
            :Action            := {|o|::Application:Report:Open() }
            :Create()
         END

         WITH OBJECT ::Application:Props[ "SaveMenu" ] := MenuStripItem( :this )
            :Caption          := "&Save"
            :ImageIndex       := ::System:StdIcons:FileSave
            :Enabled          := .F.
            :ShortCutText     := "Ctrl+S"
            :ShortCutKey:Ctrl := .T.
            :ShortCutKey:Key  := ASC( "S" )
            :Action     := {|o|::Application:Report:Save() }
            :Create()
         END

         WITH OBJECT ::Application:Props[ "SaveAsMenu" ] := MenuStripItem( :this )
            :Caption    := "Save &As ... "
            :ImageIndex := 0
            :Enabled    := .F.
            :Action     := {|o|::Application:Report:Save(.T.) }
            :Create()
         END

         WITH OBJECT MenuStripItem( :this )
            :Begingroup := .T.
            :Caption    := "&Exit"
            :ImageIndex := 0
            :Action     := {|o|::Application:MainForm:Close() }
            :Create()
         END

      END

      WITH OBJECT ::Application:Props[ "EditMenu" ] := MenuStripItem( :this )
         :Caption := "&Edit"
         :ImageList := :Parent:ImageList
         :Create()

         WITH OBJECT MenuStripItem( :this )
            :Caption := "&Copy"
            :Create()
         END

         WITH OBJECT MenuStripItem( :this )
            :Caption := "&Cut"
            :Create()
         END

         WITH OBJECT MenuStripItem( :this )
            :Caption := "&Paste"
            :Create()
         END

         WITH OBJECT MenuStripItem( :this )
            :Caption := "&Delete"
            :Create()
         END

         WITH OBJECT MenuStripItem( :this )
            :Caption := "&Undo"
            :Create()
         END

         WITH OBJECT MenuStripItem( :this )
            :Caption := "&Redo"
            :Create()
         END
      END

      WITH OBJECT ::Application:Props[ "ViewMenu" ] := MenuStripItem( :this )
         :Caption := "&View"
         :ImageList := :Parent:ImageList
         :Create()

         WITH OBJECT ::Application:Props[ "ViewMenuHeader" ] := MenuStripItem( :this )
            :Caption := "&Header"
            :Action  := {|o| o:Checked := !o:Checked, ::Application:Props[ "Header" ]:Visible := o:Checked }
            :Create()
         END

         WITH OBJECT ::Application:Props[ "ViewMenuFooter" ] := MenuStripItem( :this )
            :Caption := "&Footer"
            :Action  := {|o| o:Checked := !o:Checked, ::Application:Props[ "Footer" ]:Visible := o:Checked }
            :Create()
         END
      END

      WITH OBJECT ::Application:Props[ "HelpMenu" ] := MenuStripItem( :this )
         :Caption := "&Help"
         :ImageList := :Parent:ImageList
         :Create()

         WITH OBJECT MenuStripItem( :this )
            :Caption := "&Help"
            :Action  := {|| MessageBox( , "Sorry, no help available yet.", "Visual Report", MB_OK | MB_ICONEXCLAMATION ) }
            :Create()
         END

         WITH OBJECT MenuStripItem( :this )
            :Caption := "&About"
            :Action  := {|| MessageBox( , ::StatusBarPanel1:Caption, "About Visual Report", MB_OK | MB_ICONINFORMATION ) }
            :Create()
         END
      END
   END

   WITH OBJECT ToolStrip( ::ToolStripContainer1 )
      //:ShowChevron := .F.
      :Caption := "Standard"
      :Row     := 2
      :ImageList := ImageList( :this, 16, 16 ):Create()
      :ImageList:AddImage( IDB_STD_SMALL_COLOR )
      :ImageList:MaskColor := ::System:Color:Cyan
      :ImageList:AddBitmap( "TBEXTRA" )
      //:ImageList:AddBitmap( "TREE" )
      :Create()

      WITH OBJECT ToolStripButton( :this )
         :Caption           := "New"
         :ImageIndex        := ::System:StdIcons:FileNew
         :Action            := {|o| ::Application:Report := Report():New() }
         :Create()
      END

      WITH OBJECT ::Application:Props[ "OpenBttn" ] := ToolStripButton( :this )
         :ImageIndex   := ::System:StdIcons:FileOpen
         :ToolTip:Text := "Open"
         :Action       := {|o|::Application:Report:Open() }
         :DropDown     := 2
         :Create()
         aEntries := ::Application:IniFile:GetEntries( "Recent" )

         FOR EACH cReport IN aEntries
             oItem := MenuStripItem( :this )
             oItem:GenerateMember := .F.
             oItem:Caption := cReport
             oItem:Action  := {|o| ::Application:Report:Open( o:Caption ) }
             oItem:Create()
         NEXT
      END

      WITH OBJECT ::Application:Props[ "CloseBttn" ] := ToolStripButton( :this )
         :ImageIndex   := 16
         :ToolTip:Text := "Close"
         :Action       := {|o|::Application:Report:Close() }
         :Enabled      := .F.
         :Create()
      END

      WITH OBJECT ::Application:Props[ "SaveBttn" ] := ToolStripButton( :this )
         :ImageIndex   := ::System:StdIcons:FileSave
         :ToolTip:Text := "Save"
         :Action       := {|o|::Application:Report:Save() }
         :Enabled      := .F.
         :Create()
      END
   END

   WITH OBJECT ::Application:Props[ "ToolBox" ] := ToolBox( Self )
      :Dock:Top     := ::ToolStripContainer1
      :Dock:Bottom  := ::StatusBar1
      :Dock:Margins   := "2,2,2,2"

      :FullRowSelect    := .T.
      :FlatBorder       := .T.

      :NoHScroll        := .T.
      :HasButtons       := .T.
      :LinesAtRoot      := .T.
      :ShowSelAlways    := .T.
      :HasLines         := .F.
      :DisableDragDrop  := .T.

      :Enabled      := .F.
      :Create()
   END

   WITH OBJECT Splitter( Self )
      :Owner := ::Application:Props[ "ToolBox" ]
      :Position := 3
      :Create()
   END

   WITH OBJECT ::Application:Props[ "PropEditor" ] := PropEditor( Self )
      :Width          := 300
      :Height         := 300
      :Dock:Right     := :Parent
      :Dock:Top       := ::ToolStripContainer1
      :Dock:Bottom    := ::StatusBar1
      :Dock:Margins   := "2,2,2,2"

      :FullRowSelect  := .T.
      :FlatBorder     := .T.

      :NoHScroll      := .T.
      :HasButtons     := .T.
      :LinesAtRoot    := .T.
      :ShowSelAlways  := .T.

      :Columns := { {120,::System:Color:White}, {120,::System:Color:White} }
      :Create()
      :SetFocus()
      :BackColor := GetSysColor( COLOR_BTNFACE )
      :ExpandAll()
   END

   WITH OBJECT Splitter( Self )
      :Owner := ::Application:Props[ "PropEditor" ]
      :Position := 1
      :Create()
   END

   WITH OBJECT ::Application:Props:Components := ComponentPanel( Self )
      :Dock:Left      := ::Application:Props[ "ToolBox" ]
      :Dock:Right     := ::Application:Props[ "PropEditor" ]
      :Dock:Bottom    := ::StatusBar1
      :Top            := 300
      :Height         := 50
      :StaticEdge     := .F.
      :FlatCaption    := .T.
      :FlatBorder     := .T.
      :GenerateMember := .F.
      :Dock:Margins   := "0,2,0,2"
      :BackColor        := ::System:CurrentScheme:ToolStripGradientBegin
      :OnWMThemeChanged := {|o| o:BackColor := ::System:CurrentScheme:ToolStripGradientBegin }
      :Visible := .T.
      :Create()
   END


   WITH OBJECT ::Application:Props[ "Header" ] := HeaderEdit( Self )
      :Caption        := "Header"
      :BackColor      := ::System:Color:White
      :Height         := 100
      :Dock:Margins   := "0,2,0,2"
      :Dock:Left      := ::Application:Props[ "ToolBox" ]
      :Dock:Right     := ::Application:Props[ "PropEditor" ]
      :Dock:Top       := ::ToolStripContainer1
      :Visible        := ::Application:IniFile:ReadInteger( "View", "Header", 0 )==1
      :Application:Props[ "ViewMenuHeader" ]:Checked := :Visible
      :Create()
   END

   WITH OBJECT ::Application:Props[ "Footer" ] := FooterEdit( Self )
      :Caption        := "Footer"
      :BackColor      := ::System:Color:White
      :Border         := .T.
      :Height         := 100
      :Dock:Margins   := "0,2,0,3"
      :Dock:Left      := ::Application:Props:ToolBox
      :Dock:Right     := ::Application:Props:PropEditor
      :Dock:Bottom    := ::Application:Props:Components
      :Visible        := ::Application:IniFile:ReadInteger( "View", "Footer", 0 )==1
      :Application:Props[ "ViewMenuFooter" ]:Checked := :Visible
      :Create()
   END

   WITH OBJECT ::Application:Props[ "Body" ] := BodyEdit( Self )
      :BackColor      := ::System:Color:White
      :Dock:Margins   := "0,3,0,3"
      :Dock:Left      := ::Application:Props[ "ToolBox" ]
      :Dock:Right     := ::Application:Props[ "PropEditor" ]
      :Dock:Top       := ::Application:Props[ "Header" ]
      :Dock:Bottom    := ::Application:Props[ "Footer" ]
      :Create()
   END

   ::RestoreLayout(, "Layout")
   ::Application:Props[ "ToolBox" ]:RestoreLayout(, "Layout")
   ::Application:Props[ "PropEditor" ]:RestoreLayout(, "Layout")

   ::Application:Report:ResetQuickOpen()

   ::Show()

RETURN Self

METHOD OnClose() CLASS MainForm
   LOCAL nClose
   IF ::Application:Report != NIL 
      IF ( nClose := ::Application:Report:Close() ) != NIL
         RETURN nClose
      ENDIF
   ENDIF
   
   ::SaveLayout(, "Layout")
   ::Application:Props[ "ToolBox" ]:SaveLayout(, "Layout")
   ::Application:Props[ "PropEditor" ]:SaveLayout(, "Layout")
   ::Application:IniFile:Write( "View", "Header", IIF( ::Application:Props[ "ViewMenuHeader" ]:Checked, 1, 0 ) )
   ::Application:IniFile:Write( "View", "Footer", IIF( ::Application:Props[ "ViewMenuFooter" ]:Checked, 1, 0 ) )
RETURN NIL

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

CLASS Report
   DATA Header, Body, Footer
   DATA VrReport      EXPORTED
   DATA xModified     EXPORTED INIT .F.
   DATA xFileName     EXPORTED INIT ""
   
   ACCESS Modified    INLINE ::xModified
   ASSIGN Modified(l) INLINE oApp:MainForm:Caption := "Visual Report [" + ::GetName() + "]" + IIF( l, " *", "" ), ::xModified := l

   ACCESS FileName    INLINE ::xFileName
   ASSIGN FileName(c) INLINE oApp:MainForm:Caption := "Visual Report [" + ::GetName(c) + "]" + IIF( ::xModified, " *", "" ), ::xFileName := c

   METHOD Save()
   METHOD SaveAs()
   METHOD Close()
   METHOD Open()
   METHOD New()
   METHOD ResetQuickOpen()
   METHOD GetName()
   METHOD GetRectangle() INLINE {0,0,0,0}
   METHOD EditReset() INLINE NIL
   METHOD Generate()
ENDCLASS

//-------------------------------------------------------------------------------------------------------
METHOD New() CLASS Report
   oApp:Props:ToolBox:Enabled    := .T.
   oApp:Props:SaveMenu:Enabled   := .T.
   oApp:Props:SaveAsMenu:Enabled := .T.
   oApp:Props:CloseBttn:Enabled  := .T.
   oApp:Props:SaveBttn:Enabled   := .T.
   oApp:Props:ToolBox:RedrawWindow( , , RDW_INVALIDATE + RDW_UPDATENOW + RDW_ALLCHILDREN )   
   ::FileName := "Untitled.vrt"
   oApp:MainForm:Caption := "Visual Report [" + ::FileName + "]"
   ::VrReport := VrReport( NIL )
   oApp:Props:Components:AddButton( ::VrReport )
RETURN Self

//-------------------------------------------------------------------------------------------------------
METHOD GetName( cName, lExt ) CLASS Report
   LOCAL aName
   DEFAULT cName TO ::FileName
   DEFAULT lExt  TO .T.
   aName := hb_aTokens( cName, "\" )
   cName := ATAIL(aName)
   IF !lExt
      aName := hb_aTokens( ATAIL(aName), "." )
      cName := aName[1]
   ENDIF
RETURN cName

//-------------------------------------------------------------------------------------------------------
METHOD Close() CLASS Report
   LOCAL nClose
   IF ::Modified
       nClose := MessageBox( oApp:MainForm:hWnd, "Save changes to " + ::FileName, "Visual Report", MB_YESNOCANCEL )
       DO CASE
          CASE nClose == IDYES
               IF ! ::Save()
                  RETURN 0
               ENDIF
          CASE nClose == IDCANCEL
               RETURN 0
      END
   ENDIF
   
   WITH OBJECT oApp
      AEVAL( :Props[ "Header" ]:Objects, {|o|o:EditCtrl:Destroy()} )
      AEVAL( :Props[ "Body"   ]:Objects, {|o|o:EditCtrl:Destroy()} )
      AEVAL( :Props[ "Footer" ]:Objects, {|o|o:EditCtrl:Destroy()} )

      :Props:PropEditor:ActiveObject := NIL

      :Props:Header:RedrawWindow( , , RDW_INVALIDATE + RDW_UPDATENOW + RDW_ALLCHILDREN )   
      :Props:Body:RedrawWindow( , , RDW_INVALIDATE + RDW_UPDATENOW + RDW_ALLCHILDREN )   
      :Props:Footer:RedrawWindow( , , RDW_INVALIDATE + RDW_UPDATENOW + RDW_ALLCHILDREN )   

      :Props:ToolBox:Enabled    := .F.
      :Props:SaveMenu:Enabled   := .F.
      :Props:SaveBttn:Enabled   := .F.
      :Props:SaveAsMenu:Enabled := .F.
      :Props:CloseBttn:Enabled  := .F.
      :Props:ToolBox:RedrawWindow( , , RDW_INVALIDATE + RDW_UPDATENOW + RDW_ALLCHILDREN )   
      :MainForm:Caption := "Visual Report"
      :Props:PropEditor:ResetProperties( {{ NIL }} )
   END
RETURN NIL

//-------------------------------------------------------------------------------------------------------
METHOD Open( cReport ) CLASS Report
   LOCAL oFile
   IF cReport == NIL
      oFile := OpenFileDialog( oApp:MainForm )
      oFile:Multiselect := .F.
      oFile:Filter := "Visual Report Files (*.vrt)|*.vrt"
      IF !oFile:Show()
         RETURN NIL
      ENDIF
      cReport := oFile:FileName
   ENDIF

   oApp:Props:ToolBox:Enabled    := .T.
   oApp:Props:SaveMenu:Enabled   := .T.
   oApp:Props:SaveBttn:Enabled   := .T.
   oApp:Props:SaveAsMenu:Enabled := .T.
   oApp:Props:CloseBttn:Enabled  := .T.
   oApp:Props:ToolBox:RedrawWindow( , , RDW_INVALIDATE + RDW_UPDATENOW + RDW_ALLCHILDREN )   
   ::FileName := cReport
   ::ResetQuickOpen( cReport )
RETURN Self

METHOD Generate( oCtrl, cBuffer ) CLASS Report
   cBuffer += "   // ---- " + lower(oCtrl:Name) + " ---------"+ CRLF
   cBuffer += "   oCtrl := " + lower(oCtrl:ClassName) + "( NIL )" + CRLF
   cBuffer += "   oCtrl:Parent := oRep" + CRLF
   IF oCtrl:ClsName == "Label"
      cBuffer += "   oCtrl:Text   := " + ValToPrgExp( oCtrl:Text ) + CRLF
   ENDIF
   IF oCtrl:lUI
      cBuffer += "   oCtrl:ForeColor      := " + ValToPrgExp( oCtrl:ForeColor ) + CRLF
      cBuffer += "   oCtrl:BackColor      := " + ValToPrgExp( oCtrl:BackColor ) + CRLF

      cBuffer += "   oCtrl:Font:FaceName  := " + ValToPrgExp( oCtrl:Font:FaceName ) + CRLF
      cBuffer += "   oCtrl:Font:PointSize := " + ValToPrgExp( oCtrl:Font:PointSize ) + CRLF
      cBuffer += "   oCtrl:Font:Italic    := " + ValToPrgExp( oCtrl:Font:Italic ) + CRLF
      cBuffer += "   oCtrl:Font:Underline := " + ValToPrgExp( oCtrl:Font:Underline ) + CRLF
      cBuffer += "   oCtrl:Font:Weight    := " + ValToPrgExp( oCtrl:Font:Weight ) + CRLF

      cBuffer += "   oCtrl:Left   := " + XSTR( oCtrl:Left ) + CRLF
      cBuffer += "   oCtrl:Top    := " + XSTR( oCtrl:Top ) + CRLF
      cBuffer += "   oCtrl:Width  := " + XSTR( oCtrl:Width ) + CRLF
      cBuffer += "   oCtrl:Height := " + XSTR( oCtrl:Height ) + CRLF
      cBuffer += "   oCtrl:Create()" + CRLF
 
      cBuffer += "   nHeight := MAX( oCtrl:PDFCtrl:Attribute( 'Bottom' )-oCtrl:PDFCtrl:Attribute( 'Top' ), nHeight )" + CRLF
   ENDIF
RETURN Self

//-------------------------------------------------------------------------------------------------------
METHOD Save( lSaveAs ) CLASS Report
   LOCAL cHrb, cName, n, nHeight, cBuffer, aCtrls, i, pHrb, xhbPath, aCtrl
   DEFAULT lSaveAs TO .F.

   IF ::FileName == "Untitled.vrt" .OR. lSaveAs
      IF ( cName := ::SaveAs() ) == NIL
         RETURN .F.
      ENDIF
      ::FileName := cName
   ENDIF
   ::Modified := .F.

   aCtrls := {oApp:Props:Header:Objects, oApp:Props:Body:Objects, oApp:Props:Footer:Objects}
   
   cBuffer := "#include 'vxh.ch'"                                                            + CRLF +;
              "static nHeight, oRep"                                                         + CRLF + CRLF +;
              "FUNCTION " + ::GetName(,.F.) + "( lDesign )"                                  + CRLF +;
              "   LOCAL oCtrl"                                                               + CRLF +;
              "   DEFAULT lDesign TO .F."                                                    + CRLF +;
              "   oRep := VrReport():Create()"                                               + CRLF +;
              "   IF oRep == NIL"                                                            + CRLF +;
              "      RETURN NIL"                                                             + CRLF +;
              "   ENDIF"                                                                     + CRLF
              IF ::VrReport:DataSource != NIL
   cBuffer += "   WITH OBJECT oRep:DataSource := " + ::VrReport:DataSource:ClassName + "( NIL )" + CRLF
                  IF ! EMPTY( ::VrReport:DataSource:FileName )
   cBuffer += "      :FileName := " + ValToPrgExp(::VrReport:DataSource:FileName)            + CRLF
   cBuffer += "      :Alias    := " + ValToPrgExp(::VrReport:DataSource:Alias)               + CRLF
                     IF ! EMPTY( ::VrReport:DataSource:bFilter )
   cBuffer += "      :bFilter  := " + ::VrReport:DataSource:bFilter                          + CRLF
                     ENDIF
   cBuffer += "      :Create()"                                                              + CRLF
                  ENDIF
   cBuffer += "   END"                                                                       + CRLF
              ENDIF
   cBuffer += "   oRep:HeaderHeight := " + XSTR( oApp:Props:Header:Height ) + " * 20"        + CRLF
   cBuffer += "   oRep:FooterHeight := " + XSTR( oApp:Props:Footer:Height ) + " * 20"        + CRLF
   cBuffer += "   oRep:StartPage()"                                                          + CRLF + CRLF
              ;
   //cBuffer += "   // Generate non UI components"                                             + CRLF
   //FOR i := 1 TO LEN( aCtrls )
   //    FOR n := 1 TO LEN( aCtrls[i] )
   //        IF !aCtrls[i][n]:lUI
   //           ::Generate( aCtrls[i][n], @cBuffer )
   //        ENDIF
   //    NEXT
   //NEXT

   cBuffer += "   // Generate First Page Header"                                             + CRLF
   cBuffer += "   " + ::GetName(,.F.) + "_PrintHeader()"                                     + CRLF + CRLF
//---------------------------------------------------------------------------------------------------------   

   cBuffer += "   // Generate body area"                                                     + CRLF
               IF ::VrReport:DataSource != NIL
   cBuffer += "   oRep:DataSource:EditCtrl:Select()"                                         + CRLF
   cBuffer += "   WHILE ! oRep:DataSource:EditCtrl:Eof()"                                    + CRLF
   cBuffer += "      " + ::GetName(,.F.) + "_PrintBody()"                                    + CRLF 
   cBuffer += "      IF oRep:nRow >= ( oRep:oPDF:PageLength - oRep:FooterHeight )"           + CRLF
   cBuffer += "         " + ::GetName(,.F.) + "_PrintFooter()"                               + CRLF
   cBuffer += "         oRep:EndPage()"                                                      + CRLF
   cBuffer += "         oRep:StartPage()"                                                    + CRLF
   cBuffer += "         " + ::GetName(,.F.) + "_PrintHeader()"                               + CRLF
   cBuffer += "      ENDIF"                                                                  + CRLF
   cBuffer += "      oRep:DataSource:EditCtrl:Skip()"                                        + CRLF
   cBuffer += "   ENDDO"                                                                     + CRLF
                ELSE
   cBuffer += "   " + ::GetName(,.F.) + "_PrintBody()"                                       + CRLF 
   cBuffer += "   " + ::GetName(,.F.) + "_PrintFooter()"                                     + CRLF
               ENDIF
//---------------------------------------------------------------------------------------------------------   

   cBuffer += "   oRep:End()"                                                                + CRLF
               IF ::VrReport:DataSource != NIL
   cBuffer += "   oRep:DataSource:EditCtrl:Close()"                                       + CRLF
               ENDIF
   cBuffer += "   oRep:Preview()"                                                            + CRLF
   cBuffer += "RETURN NIL" + CRLF + CRLF

//---------------------------------------------------------------------------------------------------------   
   cBuffer += "STATIC FUNCTION " + ::GetName(,.F.) + "_PrintHeader()"                        + CRLF
   cBuffer += "   local nHeight := 0" + CRLF
   aCtrl := oApp:Props:Header:Objects
   FOR n := 1 TO LEN( aCtrl )
       IF aCtrl[n]:lUI
          ::Generate( aCtrl[n], @cBuffer )
       ENDIF
   NEXT
   cBuffer += "   oRep:nRow += nHeight + ( 10*" + XSTR( oApp:Props:Header:Height ) + ")"     + CRLF
   cBuffer += "RETURN NIL" + CRLF + CRLF

//---------------------------------------------------------------------------------------------------------   
   cBuffer += "STATIC FUNCTION " + ::GetName(,.F.) + "_PrintFooter()"                        + CRLF
   cBuffer += "   local nHeight := 0" + CRLF
   aCtrl := oApp:Props:Footer:Objects
   FOR n := 1 TO LEN( aCtrl )
       IF aCtrl[n]:lUI
          ::Generate( aCtrl[n], @cBuffer )
       ENDIF
   NEXT
   cBuffer += "   oRep:nRow += nHeight" + CRLF
   cBuffer += "RETURN NIL" + CRLF + CRLF

//---------------------------------------------------------------------------------------------------------   

   cBuffer += "STATIC FUNCTION " + ::GetName(,.F.) + "_PrintBody()"                          + CRLF
   cBuffer += "   local nHeight := 0" + CRLF
   aCtrl := oApp:Props:Body:Objects
   FOR n := 1 TO LEN( aCtrl )
       IF aCtrl[n]:lUI
          ::Generate( aCtrl[n], @cBuffer )
       ENDIF
   NEXT
   cBuffer += "   oRep:nRow += nHeight" + CRLF
   cBuffer += "RETURN NIL" + CRLF

//---------------------------------------------------------------------------------------------------------   
   MEMOWRIT( ::FileName, cBuffer, .F. )
   
   xhbPath := oApp:IniFile:Read( "Compiler", "Path", "c:\xHB" )
   IF FILE( xhbPath + "\bin\xHB.exe" )
      WaitExecute( xhbPath + "\bin\xHB.exe", ::FileName + " -i"+xhbPath+"\include\w32;"+xhbPath+"\include /gh /n" , , SW_HIDE )

      cHrb := STRTRAN( ::FileName, ".vrt", ".hrb" )
      
      FERASE( ::FileName )
      FRENAME( cHrb, ::FileName )
      
      IF FILE( ::FileName )
         pHrb := __hrbLoad( ::FileName )
         __hrbDo( pHrb, .F. )
         __hrbUnload( pHrb )
      ENDIF
   ENDIF

RETURN .T.

//-------------------------------------------------------------------------------------------------------
METHOD SaveAs() CLASS Report
   LOCAL oFile
   WITH OBJECT ( oFile := SaveFileDialog( oApp:MainForm ) )
      :FileExtension   := "prg"
      :CheckPathExists := .T.
      :Filter          := "Visual Report Files (*.vrt)|*.vrt"
      IF !:Show() .OR. EMPTY( :FileName )
         RETURN NIL
      ENDIF
   END
RETURN oFile:FileName

//-------------------------------------------------------------------------------------------------------

METHOD ResetQuickOpen( cFile ) CLASS Report
   LOCAL lMembers, aEntries, n, cProject, oItem, nId, nBkHeight, oLink, x, oMenu

   // IniFile Recently open projects
   aEntries := oApp:IniFile:ReadArray( "Recent" )

   WHILE LEN( aEntries ) > 19
      ADEL( aEntries, 1, .T. ) 
   ENDDO
   
   IF !EMPTY( cFile )
      IF ( n := ASCAN( aEntries, {|c| UPPER(c) == UPPER(cFile) } ) ) > 0
         ADEL( aEntries, n, .T. )
      ENDIF
      AINS( aEntries, 1, cFile, .T. )
   ENDIF
   
   oApp:IniFile:Write( "Recent", aEntries )

   // Reset Open Dropdown menu
   WITH OBJECT oApp:Props[ "OpenBttn" ]   // Open Button
      :Children := {}
      FOR EACH cProject IN aEntries
          oItem := MenuStripItem( :this )
          oItem:GenerateMember := .F.
          oItem:Caption := cProject
          oItem:Action  := {|o| oApp:Report:Open( o:Caption ) }
          oItem:Create()
      NEXT
   END

RETURN Self
