/*
 * $Id: xide.prg,v 1.113 2002/11/07 20:05:58 what32 Exp $
 */

/*
 * xHarbour Project source code:
 *
 * xIDE Main Module
 *
 * Copyright 2002 Augusto Infante [systems@quesoro.com] Andy Wos [andrwos@aust1.net] Ron Pinkas [ron@ronpinkas.com]
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

#include "windows.ch"
#include "wingdi.ch"
#include "common.ch"
#include "hbclass.ch"
#include "debug.ch"
#include "what32.ch"
#Include "toolbar.ch"
#Include "winlview.ch"
#include "wintypes.ch"
#include "cstruct.ch"

GLOBAL oApp
GLOBAL MainFrame
GLOBAL FormEdit

//-------------------------------------------------------------------------------------------

FUNCTION Main
   local oSplash

   oApp := Application():Initialize()

   // splash screen
   oSplash := TSplash():Create( oApp, "visualxharbour.bmp", 3000 )

   WITH OBJECT oApp
      WITH OBJECT :CreateForm( MainFrame(), @MainFrame )

         :SetStyle( WS_THICKFRAME, .F. )
         :SetStyle( WS_MAXIMIZEBOX, .F. )

         :MainMenu()
         :MainToolBar()
         :MainStatusBar()

         // add the object windows
//         :Add( ObjTree():Create( MainFrame ) )
//         :Add( ObjInspect():Create( MainFrame ) )
//         :Add( ObjEdit():Create( MainFrame ) )
         
         // focus to main Frame
         :SetFocus()

      END
      :Run()
  END
RETURN( nil)

//----------------------------------------------------------------------------------------------

CLASS MainFrame FROM TFrame
   METHOD Create( oParent ) INLINE ::FCaption  := 'xHarbour xIde',;
                                   ::FLeft     := 0,;
                                   ::FTop      := 0,;
                                   ::FWidth    := GetWindowRect( GetDesktopWindow() )[3],;
                                   ::FHeight   := 131,;
                                   ::ExStyle   := WS_EX_APPWINDOW,;
                                   ::FrameWnd  := .T.,;
                                   ::Icon      := LoadIcon( hInstance(), 'IDE' ),;
                                   super:Create( oParent )

   METHOD OnCloseQuery() INLINE if( ::MsgBox( 'Quitting xIDE ?','Exit', MB_YESNO ) == IDYES,;
                                    NIL, 0 )
   METHOD MainMenu()
   METHOD MainToolBar()
   METHOD MainStatusBar()
ENDCLASS

//----------------------------------------------------------------------------------------------

METHOD MainMenu() CLASS MainFrame
   ::WindowMenu := TMenu():New()
   WITH OBJECT ::WindowMenu
      :AddPopup('&Test')
      WITH OBJECT :Popup
         :AddItem( 'Editor', 101, {||  FormEdit := TFormEdit():Create( MainFrame ) } )
         :AddItem( 'Open', 102, {|| OpenProject():Create() } )
         :AddSeparator()
         :AddItem( 'Exit'  , 200, {||MainFrame:PostMessage(WM_SYSCOMMAND,SC_CLOSE)} )
      END WITH
   END WITH
   ::SetWindowMenu()
return(self)

//----------------------------------------------------------------------------------------------

METHOD MainToolBar() CLASS MainFrame

   LOCAL n, oTool, oSplash
   LOCAL hImg1,hImg2,hImg3,hBmp,aStdTab

   ::Add( TRebar():Create( MainFrame ) )

    // add the xmake toolbar
   WITH OBJECT ::Add( TToolBar():Create( MainFrame ) )
      ToolButton():Create( ::ToolBar1 ):Hint := "New Project"
      ToolButton():Create( ::ToolBar1 ):Hint := "Open Project"
      ToolButton():Create( ::ToolBar1 ):Hint := "Properties"
      ToolButton():Create( ::ToolBar1 ):Hint := "Build Application"
      ToolButton():Create( ::ToolBar1 ):Hint := "Build and Launch Application"
      ToolButton():Create( ::ToolBar1 ):Hint := "Re-Build Application"
      ToolButton():Create( ::ToolBar1 ):Hint := "Re-Build and Launch Application"
      ToolButton():Create( ::ToolBar1 ):Hint := "Launch Application"
      ToolButton():Create( ::ToolBar1 ):Hint := "Compile Single Source"
      ToolButton():Create( ::ToolBar1 ):Hint := "Compile All Sources"
      ToolButton():Create( ::ToolBar1 ):Hint := "Link Only"
      ToolButton():Create( ::ToolBar1 ):Hint := "Compile to PPO"
      ToolButton():Create( ::ToolBar1 ):Hint := "View"
      ToolButton():Create( ::ToolBar1 ):Hint := "Files"

      :RowCount := 2

      // ----------------------------------------------------   set imagelist
      hImg1:= ImageList_Create( 20, 20, ILC_COLORDDB+ILC_MASK )
      hBmp := LoadImage( hInstance(), "XMAKE", IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT )
      ImageList_AddMasked( hImg1, hBmp, RGB( 0, 255, 255 ) )
      DeleteObject(hBmp)
      SendMessage( :handle, TB_SETIMAGELIST, 0, hImg1 )
      //---------------------------------------------------------------------
   END
   ::Rebar1:AddBand( NIL, RBBS_GRIPPERALWAYS + RBBS_NOVERT , ::ToolBar1:handle, 200, 52, 200, "", NIL )
   // add the TabControl on the Rebarband

   WITH OBJECT ::Add( ToolTabs():Create( MainFrame ) )
      :AddTab( "StdTab", TabPage():Create( MainFrame:ToolTabs ) )
      :StdTab:Caption := "Standard"
      
      :AddTab( "Win32", TabPage():Create( MainFrame:ToolTabs ) )
      :Win32:Caption := "Win32"

      :AddTab( "Additional" )
      :AddTab( "System" )
      :AddTab( "Internet" )
      :AddTab( "Dialogs" )
      :AddTab( "Samples" )
      :AddTab( "Activex" )
   END 
   ::Rebar1:AddBand( NIL, RBBS_GRIPPERALWAYS + RBBS_NOVERT , ::ToolTabs:handle, 550, 56, , "", NIL )


   // sets the controls toolbar on the TabControl
   With Object ::ToolTabs:StdTab
      //:Add( TRebar():Create( MainFrame:ToolTabs:StdTab ) )
      //:Rebar1:SetStyle( WS_BORDER, .F. )
      With Object :Add( StdTools():Create( MainFrame:ToolTabs:StdTab ) )
         :FWidth := :Parent:Width
         :FHeight:= :Parent:Height
         
         :GetHandle()
         :Show( SW_SHOW )
         
         view :Parent:Width, :Parent:Height, :Width, :Height
         
         :SetStyle( TBSTYLE_CHECKGROUP )
         aStdTab := { '', 'Frames', 'MainMenu', 'PopupMenu', 'Label', 'Edit', 'Memo', 'Button', ;
                          'CheckBox', 'RadioButton', 'ListBox', 'ComboBox', 'ScrollBar', 'GroupBox', ;
                          'RadioGroup', 'Panel', 'ActionList' }
         for n:=0 to 16

             oTool := ToolButton():Create( MainFrame:ToolTabs:StdTab:StdTools )     //n,,aStdTab[n+1], 150+n )

             oTool:Action := {|oItem| FormEdit:OnMenuCommand( oItem ) }
             oTool:Style  := TBSTYLE_BUTTON + TBSTYLE_CHECKGROUP

         next

         // ----------------------------------------------------   set imagelist
         hImg2:= ImageList_Create( 24, 24, ILC_COLORDDB+ILC_MASK )
         hBmp := LoadImage( hInstance(), "STDTAB", IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT )
         ImageList_AddMasked( hImg2, hBmp, RGB( 0, 255, 255 ) )
         DeleteObject(hBmp)
         SendMessage( :handle, TB_SETIMAGELIST, 0, hImg2 )
         //---------------------------------------------------------------------
      End
      //:Rebar1:AddBand( NIL, RBBS_NOVERT, :StdTools:handle, 100, 30,  , "", NIL )
      //:StdTools:Disable()
   End
/*
//----------------------------------------------------------------------------------------------
   With Object ::ToolTabs:Win32
//      :Add( TRebar():Create( MainFrame:ToolTabs:Win32 ) )
//      :Rebar1:SetStyle( WS_BORDER, .F. )
      With Object :Add( WinTools():Create( ::ToolTabs:Win32 ) )
         :SetStyle( TBSTYLE_CHECKGROUP )
         aStdTab := { '', 'TabControl', 'TreeView', '', 'StatusBar', 'ProgressBar', 'ToolBar', 'Rebar', ;
                      '', '' }
         for n:=0 to 9
             oTool := ToolButton():Create( ::ToolTabs:Win32:WinTools )
             oTool:Action := {|oItem| FormEdit:OnMenuCommand(oItem) }
             oTool:Style  := TBSTYLE_BUTTON + TBSTYLE_CHECKGROUP
         next

         // ----------------------------------------------------   set imagelist
         hImg2:= ImageList_Create( 24, 24, ILC_COLORDDB+ILC_MASK )
         hBmp := LoadImage( hInstance(), "WIN32", IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT )
         ImageList_AddMasked( hImg2, hBmp, RGB( 0, 255, 255 ) )
         DeleteObject(hBmp)
         SendMessage( :handle, TB_SETIMAGELIST, 0, hImg2 )
         //---------------------------------------------------------------------

      End
//      :Rebar1:AddBand( NIL, RBBS_NOVERT, :WinTools:handle, 100, 30,  , "", NIL )
      //:WinTools:Disable()
   End

   //--------- sets a QUICK access to the control
   ::ToolTabs:StdTab:StdTools:Name := "StdBar"
   ::SetLink( ::ToolTabs:StdTab:StdTools)

   ::ToolTabs:Win32:WinTools:Name := "Win32Bar"
   ::SetLink( ::ToolTabs:Win32:WinTools )
*/
return(self)

//----------------------------------------------------------------------------------------------

METHOD MainStatusBar() CLASS MainFrame
   ::Add( TStatusBar():Create( Self ) )
   ::StatusBar1:Id      := 1001
   ::StatusBar1:Caption := 'StatusBar'
   ::StatusBar1:SetPanels( { 150,380,480,580,-1 } )
   ::StatusBar1:SetPanelText( 0, "Visual xHarbour" )
return(self)

//----------------------------------------------------------------------------------------------

CLASS ToolTabs FROM TTabControl
   DATA Name INIT "ToolTabs"
ENDCLASS

//----------------------------------------------------------------------------------------------

CLASS StdTools FROM TToolBar
   DATA Name INIT "StdTools"
ENDCLASS

//----------------------------------------------------------------------------------------------

CLASS WinTools FROM TToolBar
   DATA Name INIT "WinTools"
ENDCLASS

//----------------------------------------------------------------------------------------------

CLASS OpenProject FROM TOpenDialog
   METHOD Create() INLINE ::Filter     := { {"xIde Files (*.prg)","*.prg"} },;
                          ::Title      := "Open Project",;
                          ::InitialDir := GetModuleFileName(),;
                          ::FileName   := space( 255 ),;
                          ::Execute(),;
                          XFMOpen( ::FileName )
ENDCLASS

//--------------------------------------------------------------------------//
#pragma BEGINDUMP

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <windows.h>

#include "hbapi.h"
#include "hbstack.h"
#include "hbapierr.h"
#include "hbapiitm.h"
#include "hbvm.h"
#include "hbfast.h"

#define SKIP_SPACE(p) { while( *p == ' ' || *p == '\t' ) p++; }
#define SKIP_EOL() { while( *sText == '\n' || *sText == '\r' ) sText++; }

#define XFM_MAX_EXP 1024

int XFMParse( char *sText );

HB_FUNC( XFMOPEN )
{
   PHB_ITEM pXFM = hb_param( 1, HB_IT_STRING );
   FILE *fh;
   long lSize ;
   char *sXFM;
   int  iErr;

   if( pXFM )
   {
      fh = fopen( pXFM->item.asString.value, "r" );
   }
   else
   {
      fh = NULL;
   }

   if( fh )
   {
      fseek( fh, 0, SEEK_END );
      if( ( iErr = ferror( fh ) ) != 0 )
      {
         OutputDebugString( "I/O Error\n" );
         hb_retl( 0 );
         fclose( fh );
         return;
      }

      lSize = ftell( fh );

      if( lSize > 0 )
      {
         sXFM = malloc( lSize + 1 );

         fseek( fh, 0, SEEK_SET );
         if( ( iErr = ferror( fh ) ) != 0 )
         {
            OutputDebugString( "I/O Error\n" );
            hb_retl( 0 );
            fclose( fh );
            free( sXFM );
            return;
         }

         fread( sXFM, 1, lSize, fh );
         if( ( iErr = ferror( fh ) ) != 0 )
         {
            OutputDebugString( "I/O Error\n" );
            hb_retl( 0 );
            fclose( fh );
            free( sXFM );
            return;
         }

         //OutputDebugString( "Parse:\n" );
         //OutputDebugString( sXFM );
         iErr = XFMParse( sXFM );
         //OutputDebugString( "Parsed\n" );

         hb_retl( iErr );
         //OutputDebugString( "Set Return\n" );

         fclose( fh );
         //OutputDebugString( "Closed.\n" );
         free( sXFM );
         //OutputDebugString( "Freed.\n" );
         return;
      }
   }

   hb_retl( 0 );
}

int XFMParse( char *sText )
{
   char sClass[64], sFromClass[64], sVar[64], sExp[XFM_MAX_EXP], *pTemp, *pTerm, *pEnd[16], sAssign[64], sTemp[256];
   int i, iEnd = 0, iLen;
   static PHB_DYNS pCreateForm = NULL;
   static PHB_DYNS pTFormEdit = NULL;
   PHB_DYNS pClassSym;
   HB_ITEM Exp, Control, Object, Name, Element;
   PHB_ITEM pForm;
   MSG msg ;

   Exp.type = HB_IT_NIL;
   Control.type = HB_IT_NIL;
   Object.type = HB_IT_NIL;
   Name.type = HB_IT_NIL;
   Element.type = HB_IT_NIL;

   if( pCreateForm == NULL )
   {
      pCreateForm = hb_dynsymFind( "CREATEFORM" );
   }

   if( pTFormEdit == NULL )
   {
      pTFormEdit = hb_dynsymFind( "TFORMEDIT" );
   }

   //OutputDebugString( sText );

   sText = strstr( sText, "// ! AUTO_GENERATED !" );
   if( ! sText )
   {
      return 0;
   }
   sText += 21;

   sText = strstr( sText, "CLASS" );
   if( ! *sText )
   {
      return 0;
   }
   sText += 5;

   SKIP_SPACE( sText );

   i = 0;
   while( isalnum( *sText ) )
   {
     sClass[i++] = *sText++;
   }
   sClass[i] = '\0';

   OutputDebugString( "Class: " );
   OutputDebugString( (char *) sClass );

   SKIP_SPACE( sText );

   if( strncmp( sText, "FROM", 4 ) )
   {
      return 0;
   }
   sText += 4;

   SKIP_SPACE( sText );

   i = 0;
   while( isalnum( *sText ) )
   {
     sFromClass[i++] = *sText++;
   }
   sFromClass[i] = '\0';

   OutputDebugString( " From: " );
   OutputDebugString( sFromClass );
   OutputDebugString( "\n" );

   //TFormEdit()
   hb_vmPushSymbol( pTFormEdit->pSymbol );
   hb_vmPushNil();
   hb_vmDo( 0 );

   OutputDebugString( "Done, FormEdit()\n" );

   // Save result into pForm - we will use it multiple times below.
   hb_itemForwardValue( pForm, &hb_stack.Return );

   //oApp:CreateForm( @FormEdit, TFormEdit(), MainFrame )
   hb_vmPushSymbol( pCreateForm->pSymbol );
   hb_vmPush( &OAPP );
   // See below alternative to pushing REF.
   //memcpy( ( * hb_stack.pPos ), &FORMEDIT, sizeof( HB_ITEM ) );
   //hb_stackPush();
   hb_vmPushNil();
   hb_vmPush( pForm );
   hb_vmPush( &MAINFRAME );
   hb_vmSend( 3 );

   // Instead of pushing @FormEdit
   hb_itemForwardValue( &FORMEDIT, &hb_stack.Return );

   OutputDebugString( "Done CreateForm()\n" );

   // Do events.
   while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   pEnd[ iEnd ] = strstr( sText, "END CLASS" );
   if( pEnd[ iEnd ] == NULL )
   {
      return 0;
   }
   pEnd[ iEnd++ ][0] = '\0';

   SKIP_SPACE( sText );

   SKIP_EOL();

   SKIP_SPACE( sText );

 Vars:

   if( strncmp( sText, "DATA", 4 ) )
   {
      goto Controls;
   }
   sText += 4;

   SKIP_SPACE( sText );

   i = 0;
   while( isalnum( *sText ) )
   {
     sVar[i++] = *sText++;
   }
   sVar[i] = '\0';

   OutputDebugString( "Var: " );
   OutputDebugString( (char *) sVar );

   SKIP_SPACE( sText )

   if( strncmp( sText, "INIT", 4 ) )
   {
      return 0;
   }
   sText += 4;

   SKIP_SPACE( sText );

   i = 0;
   while( i < XFM_MAX_EXP && *sText && *sText != '\n' )
   {
     sExp[i++] = *sText++;
   }
   sExp[i] = '\0';

   if( *sText != '\n' )
   {
      return 0;
   }

   OutputDebugString( " = " );
   OutputDebugString( (char *) sExp );
   OutputDebugString( "\n" );

   switch( sExp[0] )
   {
      case '.' :
        Exp.type = HB_IT_LOGICAL;
        Exp.item.asLogical.value = sExp[1] == 'T' ? TRUE : FALSE;
        break;

      case '"' :
        sExp[ strlen( sExp ) - 1 ] = '\0';
        hb_itemPutC( &Exp, (char *) sExp + 1 );
        break;

      case '{' :
        hb_arrayNew( &Exp, 0 );
        pTemp = (char *) sExp + 1;

        SKIP_SPACE( pTemp );

        while( *pTemp && *pTemp != '}' )
        {
           switch( pTemp[0] )
           {
              case '.' :
                Element.type = HB_IT_LOGICAL;
                Element.item.asLogical.value = pTemp[1] == 'T' ? TRUE : FALSE;
                pTemp += 3;
                break;

              case '"' :
                pTemp++;
                pTerm = strchr( pTemp, '"' );

                if( pTerm )
                {
                   pTerm[0] = '\0';
                   hb_itemPutC( &Element, pTemp );
                   pTemp = pTerm + 1;
                }
                else
                {
                   return 0;
                }

                break;

              default :
                pTerm = strpbrk( pTemp, ", " );
                if( pTerm )
                {
                   if( pTerm[0] == ',' )
                   {
                      pTerm[0] = '\0';

                      Element.type = HB_IT_LONG;
                      Element.item.asLong.value = atol( pTemp );

                      pTerm[0] = ',';
                   }
                   else
                   {
                      pTerm[0] = '\0';

                      Element.type = HB_IT_LONG;
                      Element.item.asLong.value = atol( pTemp );
                   }

                   pTemp = pTerm;
                }
                else
                {
                   return 0;
                }

                break;
           }

           SKIP_SPACE( pTemp );

           if( pTemp[0] == ',' )
           {
              pTemp++;
              SKIP_SPACE( pTemp );
           }
           else if( pTemp[0] != '}' )
           {
              return 0;
           }

           hb_arrayAddForward( &Exp, &Element );
           SKIP_SPACE( pTemp );
        }

        break;

      default :
        Exp.type = HB_IT_LONG;
        Exp.item.asLong.value = atol( sExp );
        break;
   }

   sAssign[0] = '_';
   sAssign[1] = '\0';

   iLen = strlen( sVar );
   sVar[ iLen ] = ';';
   sVar[ iLen + 1 ] = '\0';

   if( strstr( "CAPTION;TOP;LEFT;HEIGHT;WIDTH;", sVar ) )
   {
      sVar[ iLen ] = '\0';
      strcat( (char *) sAssign, (char *) "XX" );
   }
   else
   {
      sVar[ iLen ] = '\0';
   }

   strcat( (char *) sAssign, (char *) sVar );

   OutputDebugString( "Assign: " );
   OutputDebugString( sAssign );
   OutputDebugString( " Type: " );

   sprintf( (char *) sTemp, "%i\n", Exp.type );
   OutputDebugString( sTemp );

   hb_objSendMsg( &FORMEDIT, sAssign, 1, &Exp );
   hb_stack.Return.type = HB_IT_NIL;

   SKIP_EOL();

   SKIP_SPACE( sText );

   goto Vars;

 Controls:

   if( strncmp( sText, "CONTROL", 6 ) )
   {
      return 0;
   }
   sText += 7;

   SKIP_SPACE( sText );

   i = 0;
   while( isalnum( *sText ) )
   {
     sClass[i++] = *sText++;
   }
   sClass[i] = '\0';

   OutputDebugString( "Control: " );
   OutputDebugString( (char *) sClass );

   SKIP_SPACE( sText );

   if( strncmp( sText, "FROM", 4 ) )
   {
      return 0;
   }
   sText += 4;

   SKIP_SPACE( sText );

   i = 0;
   while( isalnum( *sText ) )
   {
     sFromClass[i++] = *sText++;
   }
   sFromClass[i] = '\0';

   OutputDebugString( " From: " );
   OutputDebugString( (char *) sFromClass );
   OutputDebugString( "\n" );

   pClassSym = hb_dynsymFind( sFromClass );
   if( pClassSym )
   {
      hb_vmPushSymbol( pClassSym->pSymbol );
      hb_vmPushNil();
      hb_vmDo( 0 );

      hb_itemForwardValue( &Control, &hb_stack.Return );

      hb_itemPutC( &Name, sClass );
      hb_objSendMsg( &Control, "_NAME", 1, &Name );
      hb_stack.Return.type = HB_IT_NIL;
   }
   else
   {
      return 0;
   }

   pEnd[ iEnd ] = strstr( sText, "END CONTROL" );
   if( pEnd[ iEnd ] == NULL )
   {
      return 0;
   }
   pEnd[ iEnd++ ][0] = '\0';

   SKIP_SPACE( sText );

   SKIP_EOL();

   SKIP_SPACE( sText );

 Properties:

   if( *sText != ':' )
   {
      if( strncmp( sText, "OBJECT", 6 ) )
      {
         if( *sText == '\0' )
         {
            if( Object.type == HB_IT_NIL )
            {
               OutputDebugString( "END CONTROL\n" );

               hb_objSendMsg( &FORMEDIT, "ADD", 1, &Control );
               hb_stack.Return.type = HB_IT_NIL;

               OutputDebugString( "Done Add()" );

               while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
               {
                  TranslateMessage(&msg);
                  DispatchMessage(&msg);
               }

               hb_objSendMsg( &FORMEDIT, "SETCONTROL", 1, &Control );
               hb_stack.Return.type = HB_IT_NIL;

               OutputDebugString( "Done SetControl()" );

               while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
               {
                  TranslateMessage(&msg);
                  DispatchMessage(&msg);
               }
            }
            else
            {
               OutputDebugString( "END OBJECT\n" );

               sAssign[0] = '_';
               sAssign[1] = '\0';
               strcat( (char *) sAssign, (char *) sClass );

               OutputDebugString( "Property: " );
               OutputDebugString( sAssign );
               OutputDebugString( "\n" );

               hb_objSendMsg( &Control, sAssign, 1, &Object );
               hb_stack.Return.type = HB_IT_NIL;

               hb_itemClear( &Object );
            }

            sText = pEnd[ --iEnd ] + 11;

            OutputDebugString( "Continue with: " );
            OutputDebugString( sText );

            sText = strchr( sText, '\n' );
            if( sText == NULL )
            {
               return 0;
            }

            SKIP_EOL();

            SKIP_SPACE( sText );
         }

         if( *sText == ':' )
         {
            goto Properties;
         }

         if( *sText )
         {
            goto Vars;
         }
         else
         {
            OutputDebugString( "Done.\n" );
            return 1;
         }
      }

      sText += 6;

      SKIP_SPACE( sText );

      i = 0;
      while( isalnum( *sText ) )
      {
        sClass[i++] = *sText++;
      }
      sClass[i] = '\0';

      OutputDebugString( "Object: " );
      OutputDebugString( (char *) sClass );

      SKIP_SPACE( sText );

      if( strncmp( sText, "IS", 2 ) )
      {
         return 0;
      }
      sText += 2;

      SKIP_SPACE( sText );

      i = 0;
      while( isalnum( *sText ) )
      {
        sFromClass[i++] = *sText++;
      }
      sFromClass[i] = '\0';

      OutputDebugString( " IS: " );
      OutputDebugString( (char *) sFromClass );
      OutputDebugString( "\n" );

      pClassSym = hb_dynsymFind( sFromClass );
      if( pClassSym )
      {
         hb_vmPushSymbol( pClassSym->pSymbol );
         hb_vmPushNil();
         hb_vmDo( 0 );

         hb_itemForwardValue( &Object, &hb_stack.Return );
      }
      else
      {
         return 0;
      }

      pEnd[ iEnd ] = strstr( sText, "END OBJECT" );
      if( pEnd[ iEnd ] == NULL )
      {
         return 0;
      }

      pEnd[ iEnd++ ][0] = '\0';

      SKIP_SPACE( sText );

      SKIP_EOL();

      SKIP_SPACE( sText );

      goto Properties;
   }

   sText += 1;

   SKIP_SPACE( sText );

   i = 0;
   while( isalnum( *sText ) )
   {
     sVar[i++] = *sText++;
   }
   sVar[i] = '\0';

   if( i == 0 )
   {
      return 1;
   }

   SKIP_SPACE( sText );

   if( strncmp( sText, ":=", 2 ) )
   {
      if( strncmp( sVar, "SetMethod", 9 ) == 0 && *sText == '(' )
      {
         sText++;

         SKIP_SPACE( sText );

         if( *sText == '"' )
         {
            sText++;

            i = 0;
            while( isalnum( *sText ) )
            {
              sVar[i++] = *sText++;
            }
            sVar[i] = '\0';

            sText++;

            OutputDebugString( "Event: " );
            OutputDebugString( (char *) sVar );

            SKIP_SPACE( sText );

            if( *sText != ',' )
            {
               return 0;
            }
            sText++;

            SKIP_SPACE( sText )

            if( strncmp( sText, "{ ||", 4 ) )
            {
               return 0;
            }
            sText += 4;

            SKIP_SPACE( sText );

            i = 0;
            while( isalnum( *sText ) || *sText == '_' )
            {
              sExp[i++] = *sText++;
            }
            sExp[i] = '\0';

            OutputDebugString( " = " );
            OutputDebugString( (char *) sExp );
            OutputDebugString( "\n" );

            sText = strchr( sText, '\n' );
            if( sText == NULL )
            {
               return 0;
            }

            SKIP_EOL();

            SKIP_SPACE( sText );

            goto Properties;
         }
      }
      return 0;
   }

   OutputDebugString( "Var: " );
   OutputDebugString( (char *) sVar );

   sText += 2;

   SKIP_SPACE( sText )

   i = 0;
   while( i < XFM_MAX_EXP && *sText && *sText != '\n' )
   {
     sExp[i++] = *sText++;
   }
   sExp[i] = '\0';

   if( *sText != '\n' )
   {
      return 0;
   }

   OutputDebugString( " = " );
   OutputDebugString( (char *) sExp );
   OutputDebugString( "\n" );

   switch( sExp[0] )
   {
      case '.' :
        Exp.type = HB_IT_LOGICAL;
        Exp.item.asLogical.value = sExp[1] == 'T' ? TRUE : FALSE;
        break;

      case '"' :
        sExp[ strlen( sExp ) - 1 ] = '\0';
        hb_itemPutC( &Exp, (char *) sExp + 1 );
        break;

      case '{' :
        hb_arrayNew( &Exp, 0 );

        pTemp = (char *) sExp + 1;

        SKIP_SPACE( pTemp );

        while( *pTemp && *pTemp != '}' )
        {
           switch( pTemp[0] )
           {
              case '.' :
                Element.type = HB_IT_LOGICAL;
                Element.item.asLogical.value = pTemp[1] == 'T' ? TRUE : FALSE;
                pTemp += 3;
                break;

              case '"' :
                pTemp++;
                pTerm = strchr( pTemp, '"' );

                if( pTerm )
                {
                   pTerm[0] = '\0';
                   hb_itemPutC( &Element, pTemp );
                   pTemp = pTerm + 1;
                }
                else
                {
                   return 0;
                }

                break;

              default :
                pTerm = strpbrk( pTemp, ", " );

                if( pTerm )
                {
                   if( pTerm[0] == ',' )
                   {
                      pTerm[0] = '\0';

                      Element.type = HB_IT_LONG;
                      Element.item.asLong.value = atol( pTemp );

                      pTerm[0] = ',';
                   }
                   else
                   {
                      pTerm[0] = '\0';

                      Element.type = HB_IT_LONG;
                      Element.item.asLong.value = atol( pTemp );
                   }

                   pTemp = pTerm;
                }
                else
                {
                   return 0;
                }

                break;
           }

           SKIP_SPACE( pTemp );

           if( pTemp[0] == ',' )
           {
              pTemp++;
              SKIP_SPACE( pTemp );
           }
           else if( pTemp[0] != '}' )
           {
              return 0;
           }

           hb_arrayAddForward( &Exp, &Element );
           SKIP_SPACE( pTemp );
        }

        break;

      default :
        Exp.type = HB_IT_LONG;
        Exp.item.asLong.value = atol( sExp );
        break;
   }

   sAssign[0] = '_';
   sAssign[1] = '\0';

   iLen = strlen( sVar );
   sVar[ iLen ] = ';';
   sVar[ iLen + 1 ] = '\0';

   if( strstr( "CAPTION;TOP;LEFT;HEIGHT;WIDTH;", sVar ) )
   {
      sVar[ iLen ] = '\0';
      strcat( (char *) sAssign, (char *) "XX" );
   }
   else
   {
      sVar[ iLen ] = '\0';
   }

   strcat( (char *) sAssign, (char *) sVar );

   OutputDebugString( "Assign: " );
   OutputDebugString( sAssign );
   OutputDebugString( " Type: " );

   sprintf( (char *) sTemp, "%i\n", Exp.type );
   OutputDebugString( sTemp );

   hb_objSendMsg( Object.type == HB_IT_NIL ? &Control : &Object, sAssign, 1, &Exp );
   hb_stack.Return.type = HB_IT_NIL;

   SKIP_EOL();

   SKIP_SPACE( sText );

   goto Properties;
}

#pragma ENDDUMP
