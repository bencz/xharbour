/*
 * $Id: debugger.prg,v 1.8 2003/04/09 21:10:24 lculik Exp $
 */

/*
 * Harbour Project source code:
 * The Debugger
 *
 * Copyright 1999 Antonio Linares <alinares@fivetech.com>
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version, with one exception:
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

/* NOTE: Don't use SAY/DevOut()/DevPos() for screen output, otherwise
         the debugger output may interfere with the applications output
         redirection, and is also slower. [vszakats] */
#pragma -es0
#include "hbclass.ch"
#include "hbmemvar.ch"
#include "box.ch"
#include "inkey.ch"
#include "common.ch"
#include "setcurs.ch"

#define ALTD_DISABLE   0
#define ALTD_ENABLE    1

static s_oDebugger
static s_lExit := .F.
Static nDump
memvar __DbgStatics

procedure AltD( nAction )
   do case
      case nAction == nil
           if SET( _SET_DEBUG )
              s_lExit := .f.
              s_oDebugger:lGo := .F.
              __dbgEntry( ProcLine( 1 ) )
           endif

      case nAction == ALTD_DISABLE
           SET( _SET_DEBUG, .F. )

      case nAction == ALTD_ENABLE
           SET( _SET_DEBUG, .T. )
   endcase

return

procedure __dbgEntry( uParam1, uParam2, uParam3 )  // debugger entry point

   local cModuleName, cProcName
   local nStaticsBase, nStaticIndex, cStaticName
   local cLocalName, nLocalIndex
   local nAt

   Switch ValType( uParam1 )
      case "C"   // called from hvm.c hb_vmModuleName()
           cModuleName := uParam1
           if ! s_lExit
              if s_oDebugger == nil
                 s_oDebugger := TDebugger():New()
                 s_oDebugger:Activate( cModuleName )
              else
                 if ! s_oDebugger:lTrace
                    s_oDebugger:ShowCode( cModuleName )
                 else
                    ASize( s_oDebugger:aCallStack, Len( s_oDebugger:aCallStack ) + 1 )
                    AIns( s_oDebugger:aCallStack, 1, NIL, .t. )
                    s_oDebugger:aCallStack[ 1 ] = { SubStr( cModuleName,;
                                             RAt( ":", cModuleName ) + 1 ), {}, nil }
                 endif
                 s_oDebugger:LoadVars()
              endif
           endif
           exit

      case "N"   // called from hvm.c both hb_vmDebuggerShowLines()
           public __DbgStatics         // hb_vmStaticName() and hb_vmLocalName()
           if Type( "__DbgStatics" ) == "L"
              __DbgStatics := {}
           endif

           cProcName := ProcName( 1 )
           if cProcName == "(_INITSTATICS)"
              nStaticsBase := uParam1
              cStaticName  := uParam2
              if AScan( __DbgStatics, { | a | a[ 1 ] == nStaticsBase } ) == 0
                 AAdd( __DbgStatics, { nStaticsBase, { cStaticName } } )
              else
                 AAdd( ATail( __DbgStatics )[ 2 ], cStaticName )
              endif
              return  // We can not use s_oDebugger yet, so we return
           endif

           if s_oDebugger != nil
              if PCount() == 3 // called from hvm.c hb_vmLocalName() and hb_vmStaticName()
                 if cProcName == "__EVAL" .OR. cProcName == "EVAL"
                    if !s_oDebugger:lCodeblock
                       ASize( s_oDebugger:aCallStack, Len( s_oDebugger:aCallStack ) + 1 )
                       AIns( s_oDebugger:aCallStack, 1 )
                       // nil means no line number stored yet
                       s_oDebugger:aCallStack[ 1 ] := { cProcName, {}, nil }
                       s_oDebugger:lCodeblock := .T.
                    endif
                 endif
                 if uParam3 == 1 // in-function static variable
                    cStaticName  := uParam2
                    nStaticIndex := uParam1
                    if s_oDebugger:lShowStatics
                       if ( nAt := AScan( s_oDebugger:aVars,; // Is there another var with this name ?
                            { | aVar | aVar[ 1 ] == cStaticName } ) ) != 0
                          s_oDebugger:aVars[ nAt ] := { cStaticName, nStaticIndex, "Static" }
                       else
                          AAdd( s_oDebugger:aVars, { cStaticName, nStaticIndex, "Static" } )
                       endif
                    endif
                 else            // local variable
                    cLocalName  := IIF(valtype(uParam2)=='C',uParam2,'NIL')
                    nLocalIndex := uParam1
                    if s_oDebugger:lShowLocals
                       if ( nAt := AScan( s_oDebugger:aVars,; // Is there another var with this name ?
                            { | aVar | aVar[ 1 ] == cLocalName } ) ) != 0
                          s_oDebugger:aVars[ nAt ] := { cLocalName, nLocalIndex, "Local", ProcName( 1 ) }
                       else
                          AAdd( s_oDebugger:aVars, { cLocalName, nLocalIndex, "Local", ProcName( 1 ) } )
                       endif
                    endif
                    IF Len( s_oDebugger:aCallStack )>0 .AND. valtype( s_oDebugger:aCallStack[ 1 ][ 2 ])=='A'
                      AAdd( s_oDebugger:aCallStack[ 1 ][ 2 ], { cLocalName, nLocalIndex, "Local", ProcName( 1 ) } )
                    endif
                 endif
                 if s_oDebugger:oBrwVars != nil
                    s_oDebugger:oBrwVars:RefreshAll()
                 endif
                 return
              endif
              if s_oDebugger:lTrace
                 if s_oDebugger:nTraceLevel < Len( s_oDebugger:aCallStack )
                    return
                 else
                    s_oDebugger:lTrace := .f.
                 endif
              endif
              if s_oDebugger:lGo
                 s_oDebugger:lGo := ! s_oDebugger:IsBreakPoint( uParam1 )
              endif
              if !s_oDebugger:lGo .or. InvokeDebug()  .or. ;
                 ProcName( 1 ) == "ALTD"  // debugger invoked from AltD( 1 )
                 s_oDebugger:lGo := .F.
                 s_oDebugger:SaveAppStatus()
                 s_oDebugger:GoToLine( uParam1 )
                 s_oDebugger:HandleEvent()
              endif
           endif
           exit

      default   // called from hvm.c hb_vmDebuggerEndProc()
         if Empty( ProcName( 1 ) ) // ending (_INITSTATICS)
            return
         endif
         if s_oDebugger != nil
            if s_oDebugger:lCodeblock
               s_oDebugger:lCodeblock := .F.
            endif
            s_oDebugger:EndProc()
            s_oDebugger:LoadVars()
         endif
   end

return

CLASS TDebugger

   DATA   aWindows, nCurrentWindow
   DATA   oPullDown
   DATA   oWndCode, oWndCommand, oWndStack, oWndVars
   DATA   oBar, oBrwText, cPrgName, oBrwStack, oBrwVars, aVars
   DATA   cImage
   DATA   cAppImage, nAppRow, nAppCol, cAppColors, nAppCursor
   DATA   aBreakPoints, aCallStack, aColors
   DATA   aLastCommands, nCommand, oGetListCommand
   DATA   lAnimate, lEnd, lGo, lTrace, lCaseSensitive, lMonoDisplay, lSortVars
   DATA   cSearchString, cPathForFiles, cSettingsFileName
   DATA   nTabWidth, nSpeed
   DATA   lShowPublics, lShowPrivates, lShowStatics, lShowLocals, lAll
   DATA   lShowCallStack
   DATA   nTraceLevel
   DATA   lCodeblock INIT .F.

   METHOD New()
   METHOD Activate( cModuleName )

   METHOD All()

   METHOD Animate() INLINE If( ::lAnimate, ::Step(), nil )

   METHOD BarDisplay()
   METHOD BuildCommandWindow()

   METHOD CallStackProcessKey( nKey )
   METHOD ClrModal() INLINE iif( ::lMonoDisplay, "N/W, W+/W, W/N, W+/N",;
                                "N/W, R/W, N/BG, R/BG" )

   METHOD CodeWindowProcessKey( nKey )
   METHOD Colors()
   METHOD CommandWindowProcessKey( nKey )
   METHOD EditColor( nColor, oBrwColors )
   METHOD EditSet( nSet, oBrwSets )
   METHOD EditVar( nVar )
   METHOD EndProc()
   METHOD Exit() INLINE ::lEnd := .t.
   METHOD Go() INLINE ::RestoreAppStatus(), ::lGo := .t., DispEnd(), ::Exit()
   METHOD GoToLine( nLine )
   METHOD HandleEvent()
   METHOD Hide()
   METHOD HideCallStack()
   METHOD HideVars()
   METHOD InputBox( cMsg, uValue, bValid, lEditable )
   METHOD Inspect( uValue, cValueName )
   METHOD IsBreakPoint( nLine )
   METHOD LoadSettings()
   METHOD LoadVars()

   METHOD Local()

   METHOD MonoDisplay()
   METHOD NextWindow()
   METHOD Open()
   METHOD OSShell()
   METHOD PathForFiles() 
   METHOD PrevWindow()
   METHOD Private()
   METHOD Public()
   METHOD RestoreAppStatus()
   METHOD RestoreSettings()
   METHOD SaveAppStatus()
   METHOD SaveSettings()
   METHOD Show()
   METHOD ShowAppScreen()
   METHOD ShowCallStack()
   METHOD ShowCode( cModuleName )
   METHOD ShowHelp( nTopic )
   METHOD ShowVars()

/*   METHOD Sort() INLINE ASort( ::aVars,,, {|x,y| x[1] < y[1] } ),;
                        ::lSortVars := .t.,;
                        iif( ::oBrwVars != nil, ::oBrwVars:RefreshAll(), nil ),;
          iif( ::oWndVars != nil .and. ::oWndVars:lVisible, ::oBrwVars:ForceStable(),)*/
   METHOD Sort() INLINE ASort( ::aVars,,, {|x,y| x[1] < y[1] } ),;
                        ::lSortVars := .t.,;
                        iif( ::oBrwVars != nil, ::oBrwVars:RefreshAll(), nil ),;
          iif( ::oWndVars != nil .and. ::oWndVars:lVisible, iif(!::lGo,::oBrwVars:ForceStable(),),)

   METHOD Speed() INLINE ;
          ::nSpeed := ::InputBox( "Step delay (in tenths of a second)",;
                                  ::nSpeed )

   METHOD Stack()
   METHOD Static()

   METHOD Step() INLINE ::RestoreAppStatus(), ::Exit()

   METHOD TabWidth() INLINE ;
          ::nTabWidth := ::InputBox( "Tab width", ::nTabWidth )

   METHOD ToggleBreakPoint()

   METHOD Trace() INLINE ::lTrace := .t., ::nTraceLevel := Len( ::aCallStack ),;
                         __Keyboard( Chr( 255 ) ) //forces a Step()

   METHOD ViewSets()
   METHOD WndVarsLButtonDown( nMRow, nMCol )
   METHOD LineNumbers()          // Toggles numbering of source code lines
   METHOD Locate()
   METHOD FindNext()
   METHOD FindPrevious()
   METHOD RemoveWindow()
   METHOD SearchLine()
   METHOD ToggleAnimate() INLINE ::lAnimate := ! ::lAnimate
   METHOD ToggleCaseSensitive() INLINE ::lCaseSensitive := !::lCaseSensitive
   METHOD ShowWorkAreas() INLINE __dbgShowWorkAreas( Self )

ENDCLASS


METHOD New() CLASS TDebugger

   s_oDebugger := Self
   ::aColors := {"W+/BG","N/BG","R/BG","N+/BG","W+/B","GR+/B","W/B","N/W","R/W","N/BG","R/BG"}

   ::lMonoDisplay   := .f.
   
   ::lAnimate          := .f.
   ::lEnd              := .f.
   ::lTrace            := .f.
   ::aBreakPoints      := {}
   ::aCallStack        := {}
   ::lGo               := .f.
   ::aVars             := {}
   ::lCaseSensitive    := .f.
   ::cSearchString     := ""
   // default the search path for files to the current directory
   // that way if the source is in the same directory it will still be found even if the application
   // changes the current directory with the SET DEFAULT command
   ::cPathForFiles:= CURDRIVE()+':\'+CURDIR()+'\'
   ::lShowCallStack    := .f.
   ::nTabWidth         := 4
   ::nSpeed            := 0

   ::lShowPublics      := .f.
   ::lShowPrivates     := .f.
   ::lShowStatics      := .f.
   ::lShowLocals       := .f.
   ::lAll              := .f.
   ::lSortVars         := .f.

   ::cSettingsFileName := "init.cld"
   if File( ::cSettingsFileName )
      ::LoadSettings()
   endif

   ::aWindows       := {}
   ::nCurrentWindow := 1
   ::oPullDown      := __dbgBuildMenu( Self )

   ::oWndCode       := TDbWindow():New( 1, 0, MaxRow() - 6, MaxCol() )
   ::oWndCode:Cargo       := { ::oWndCode:nTop, ::oWndCode:nLeft }
   ::oWndCode:bKeyPressed := { | nKey | ::CodeWindowProcessKey( nKey ) }
   ::oWndCode:bGotFocus   := { || ::oGetListCommand:SetFocus(), SetCursor( SC_SPECIAL1 ), ;
                              SetPos( ::oWndCode:Cargo[1],::oWndCode:Cargo[2] ) }
   ::oWndCode:bLostFocus  := { || ::oWndCode:Cargo[1] := Row(), ::oWndCode:Cargo[2] := Col(), ;
                              SetCursor( SC_NONE ) }
   /*
   ::oWndCode:bPainted := { || ::oBrwText:SetColor( __DbgColors()[ 2 ] + "," + ;
                               __DbgColors()[ 5 ] + "," + __DbgColors()[ 3 ] + "," + ;
                               __DbgColors()[ 6 ] ),;
                               iif( ::oBrwText != nil, ::oBrwText:RefreshWindow(), nil ) }
   */

   AAdd( ::aWindows, ::oWndCode )

   ::BuildCommandWindow()

return Self

METHOD PathForFiles() CLASS TDebugger

   ::cPathForFiles := ::InputBox( "Search path for source files:", ::cPathForFiles )
   IF RIGHT(::cPathForFiles,1)<>'\'
     ::cPathForFiles:=::cPathForFiles+'\'
   ENDIF
return Self

METHOD Activate( cModuleName ) CLASS TDebugger

   ::Show()
   ::ShowCode( cModuleName )
   if ::lShowCallStack
   ::ShowCallStack()
   endif
   ::ShowVars()
   ::RestoreAppStatus()

return nil

METHOD All() CLASS TDebugger

   ::lShowPublics := ::lShowPrivates := ::lShowStatics := ;
   ::lShowLocals := ::lAll := ! ::lAll

   iif( ::lAll, ::ShowVars(), ::HideVars() )

return nil

METHOD BarDisplay() CLASS TDebugger

   local cClrItem   := __DbgColors()[ 8 ]
   local cClrHotKey := __DbgColors()[ 9 ]

   DispBegin()
   SetColor( cClrItem )
   @ MaxRow(), 0 CLEAR TO MaxRow(), MaxCol()

   DispOutAt( MaxRow(),  0,;
   "F1-Help F2-Zoom F3-Repeat F4-User F5-Go F6-WA F7-Here F8-Step F9-BkPt F10-Trace",;
   cClrItem )
   DispOutAt( MaxRow(),  0, "F1", cClrHotKey )
   DispOutAt( MaxRow(),  8, "F2", cClrHotKey )
   DispOutAt( MaxRow(), 16, "F3", cClrHotKey )
   DispOutAt( MaxRow(), 26, "F4", cClrHotKey )
   DispOutAt( MaxRow(), 34, "F5", cClrHotKey )
   DispOutAt( MaxRow(), 40, "F6", cClrHotKey )
   DispOutAt( MaxRow(), 46, "F7", cClrHotKey )
   DispOutAt( MaxRow(), 54, "F8", cClrHotKey )
   DispOutAt( MaxRow(), 62, "F9", cClrHotKey )
   DispOutAt( MaxRow(), 70, "F10", cClrHotKey )
   DispEnd()

return nil

METHOD BuildCommandWindow() CLASS TDebugger

   local GetList := {}, oGet
   local cCommand

   ::oWndCommand := TDbWindow():New( MaxRow() - 5, 0, MaxRow() - 1, MaxCol(),;
                                    "Command" )

   ::oWndCommand:bGotFocus   := { || ::oGetListCommand:SetFocus(), SetCursor( SC_NORMAL ) }
   ::oWndCommand:bLostFocus  := { || SetCursor( SC_NONE ) }
   ::oWndCommand:bKeyPressed := { | nKey | ::CommandWindowProcessKey( nKey ) }
   ::oWndCommand:bPainted    := { || DispOutAt( ::oWndCommand:nBottom - 1,;
                             ::oWndCommand:nLeft + 1, "> ", __DbgColors()[ 2 ] ),;
                        oGet:ColorDisp( Replicate( __DbgColors()[ 2 ] + ",", 5 ) ),;
                        hb_ClrArea( ::oWndCommand:nTop + 1, ::oWndCommand:nLeft + 1,;
                        ::oWndCommand:nBottom - 2, ::oWndCommand:nRight - 1,;
                        iif( ::lMonoDisplay, 15, HB_ColorToN( __DbgColors()[ 2 ] ) ) ) }
   AAdd( ::aWindows, ::oWndCommand )

   ::aLastCommands := {}
   ::nCommand := 0

   cCommand := Space( ::oWndCommand:nRight - ::oWndCommand:nLeft - 3 )
   // We don't use the GET command here to avoid the painting of the GET
   AAdd( GetList, oGet := Get():New( ::oWndCommand:nBottom - 1, ::oWndCommand:nLeft + 3,;
         { | u | iif( PCount() > 0, cCommand := u, cCommand ) }, "cCommand" ) )
   oGet:ColorSpec := Replicate( __DbgColors()[ 2 ] + ",", 5 )
   ::oGetListCommand := HBGetList():New( GetList )

return nil

METHOD CallStackProcessKey( nKey ) CLASS TDebugger

   local n, nSkip, lUpdate := .f.

   do case
      case nKey == K_HOME
           if ::oBrwStack:Cargo > 1
              ::oBrwStack:GoTop()
              ::oBrwStack:ForceStable()
              lUpdate = .t.
           endif

      case nKey == K_END
           if ::oBrwStack:Cargo < Len( ::aCallStack )
              ::oBrwStack:GoBottom()
              ::oBrwStack:ForceStable()
              lUpdate = .t.
           endif

      case nKey == K_UP
           if ::oBrwStack:Cargo > 1
              ::oBrwStack:Up()
              ::oBrwStack:ForceStable()
              lUpdate = .t.
           endif

      case nKey == K_DOWN
           if ::oBrwStack:Cargo < Len( ::aCallStack )
              ::oBrwStack:Down()
              ::oBrwStack:ForceStable()
              lUpdate = .t.
           endif

      case nKey == K_PGUP
           ::oBrwStack:PageUp()
           ::oBrwStack:ForceStable()
           lUpdate = .t.

      case nKey == K_PGDN
           ::oBrwStack:PageDown()
           ::oBrwStack:ForceStable()
           lUpdate = .t.

      case nKey == K_LBUTTONDOWN
           if ( nSkip := MRow() - ::oWndStack:nTop - ::oBrwStack:RowPos ) != 0
              if nSkip > 0
                 for n = 1 to nSkip
                    ::oBrwStack:Down()
                    ::oBrwStack:Stabilize()
                 next
              else
                 for n = 1 to nSkip + 2 step -1
                    ::oBrwStack:Up()
                    ::oBrwStack:Stabilize()
                 next
              endif
              ::oBrwStack:ForceStable()
           endif
           lUpdate = .t.
   endcase

   if lUpdate
      // source line for a function
      if ::aCallStack[ ::oBrwStack:Cargo ][ 3 ] != nil
         ::GotoLine( ::aCallStack[ ::oBrwStack:Cargo ][ 3 ] )
      else
         ::GotoLine( 1 )
      endif
   endif

return nil
METHOD CodeWindowProcessKey( nKey ) CLASS TDebugger

   Switch nKey
      case K_HOME
           ::oBrwText:GoTop()
           if ::oWndCode:lFocused
              SetCursor( SC_SPECIAL1 )
           endif ; exit

      case K_END
           ::oBrwText:GoBottom()
           ::oBrwText:nCol = ::oWndCode:nLeft + 1
           ::oBrwText:nFirstCol = ::oWndCode:nLeft + 1
           SetPos( Row(), ::oWndCode:nLeft + 1 )
           if ::oWndCode:lFocused
              SetCursor( SC_SPECIAL1 )
           endif;exit

      case K_LEFT
           ::oBrwText:Left()
           exit

      case K_RIGHT
           ::oBrwText:Right()
           exit

      case K_UP
           ::oBrwText:Up()
           exit

      case K_DOWN
           ::oBrwText:Down()
           exit

      case K_PGUP
           ::oBrwText:PageUp()
           exit

      case K_PGDN
           ::oBrwText:PageDown()
           exit

   end

return nil

METHOD Colors() CLASS TDebugger

   local oWndColors := TDbWindow():New( 4, 5, 16, MaxCol() - 5,;
                                        "Debugger Colors[1..11]", ::ClrModal() )
   local aColors := { "Border", "Text", "Text High", "Text PPO", "Text Selected",;
                      "Text High Sel.", "Text PPO Sel.", "Menu", "Menu High",;
                      "Menu Selected", "Menu High Sel." }

   local oBrwColors := TBrowseNew( oWndColors:nTop + 1, oWndColors:nLeft + 1,;
                                 oWndColors:nBottom - 1, oWndColors:nRight - 1 )
   local oWin
   local nWidth := oWndColors:nRight - oWndColors:nLeft - 1
   local oCol

   if ::lMonoDisplay
      Alert( "Monochrome display" )
      return nil
   endif
   oBrwColors:Cargo :={ 1,{}} // Actual highligthed row
   oBrwColors:ColorSpec := ::ClrModal()
   oBrwColors:GOTOPBLOCK := { || oBrwColors:cargo[ 1 ]:= 1 }
   oBrwColors:GoBottomBlock := { || oBrwColors:cargo[ 1 ]:= Len(oBrwColors:cargo[ 2 ][ 1 ])}
   oBrwColors:SKIPBLOCK := { |nPos| ( nPos:= ArrayBrowseSkip(nPos, oBrwColors), oBrwColors:cargo[ 1 ]:= ;
   oBrwColors:cargo[ 1 ] + nPos,nPos ) }

   oBrwColors:AddColumn( ocol := TBColumnNew( "", { || PadR( aColors[ oBrwColors:Cargo[1] ], 14 ) } ) )
   oCol:DefColor:={1,2}
   aadd(oBrwColors:Cargo[2],acolors)
   oBrwColors:AddColumn( oCol := TBColumnNew( "",;
                       { || PadR( '"' + ::aColors[ oBrwColors:Cargo[1] ] + '"', nWidth - 15 ) } ) )
   aadd(oBrwColors:Cargo[2],acolors)
   oCol:DefColor:={1,3}
   ocol:width:=50
   oBrwColors:autolite:=.f.

   oWndColors:bPainted    := { || oBrwColors:ForceStable(),RefreshVarsS(oBrwColors)}

   oWndColors:bKeyPressed := { | nKey | SetsKeyPressed( nKey, oBrwColors,;
                               Len( aColors ), oWndColors, "Debugger Colors",;
                               { || ::EditColor( oBrwColors:Cargo[1], oBrwColors ) } ) }
   oWndColors:ShowModal()

   ::oPullDown:LoadColors()
   ::oPullDown:Refresh()
   ::BarDisplay()

   for each oWin in ::aWindows
      oWin:LoadColors()
      oWin:Refresh()
   next

return nil

METHOD CommandWindowProcessKey( nKey ) CLASS TDebugger

   local cCommand, cResult, oE
   local bLastHandler
   local lDisplay

   Switch nKey
      case K_UP
           if ::nCommand > 0
              ::oGetListCommand:oGet:VarPut( ::aLastCommands[ ::nCommand ] )
              ::oGetListCommand:oGet:Buffer := ::aLastCommands[ ::nCommand ]
              ::oGetListCommand:oGet:Pos := 1
              ::oGetListCommand:oGet:Display()
              if ::nCommand > 1
                 ::nCommand--
              endif
           endif
           exit

      case K_DOWN
           if ::nCommand > 0 .AND. ::nCommand <= Len( ::aLastCommands )
              ::oGetListCommand:oGet:VarPut( ::aLastCommands[ ::nCommand ] )
              ::oGetListCommand:oGet:Buffer := ::aLastCommands[ ::nCommand ]
              ::oGetListCommand:oGet:Pos := 1
              ::oGetListCommand:oGet:Display()
              if ::nCommand < Len( ::aLastCommands )
                 ::nCommand++
              endif
           endif
           exit

      case K_ENTER
           cCommand := ::oGetListCommand:oGet:VarGet()
           if ! Empty( cCommand )
           AAdd( ::aLastCommands, cCommand )
           ::nCommand++
           ::oWndCommand:ScrollUp( 1 )
           endif

/*           bLastHandler := ErrorBlock({ |objErr| BREAK (objErr) })

           if SubStr( LTrim( cCommand ), 1, 2 ) == "? "
              begin sequence
                  cResult := ValToStr( &( AllTrim( SubStr( LTrim( cCommand ), 3 ) ) ) )

              recover using oE
                  cResult := "Command error: " + oE:description

              end sequence

           else
              cResult := "Command error"

           endif

           ErrorBlock(bLastHandler)
           */
           do case
              case Empty( cCommand )
                 lDisplay = .f.

              case SubStr( LTrim( cCommand ), 1, 3 ) == "?? " .or. ;
                       SubStr( LTrim( cCommand ), 1, 2 ) == "? "
                 lDisplay := !Empty( cResult := DoCommand( Self,cCommand ) )

              case Upper( SubStr( LTrim( cCommand ), 1, 4 ) ) == "ANIM" .or. ;
                   Upper( SubStr( LTrim( cCommand ), 1, 7 ) ) == "ANIMATE"
                 ::lAnimate = .t.
                 ::Animate()
                 SetCursor( SC_NORMAL )
                 lDisplay = .f.

              case Upper( SubStr( LTrim( cCommand ), 1, 3 ) ) == "DOS"
                 ::OsShell()
                 SetCursor( SC_NORMAL )
                 lDisplay = .f.

              case Upper( SubStr( LTrim( cCommand ), 1, 4 ) ) == "HELP"
                 ::ShowHelp()
                 lDisplay = .f.

              case Upper( SubStr( LTrim( cCommand ), 1, 4 ) ) == "QUIT"
                 ::Exit()
                 ::Hide()
                 __Quit()

              case Upper( SubStr( LTrim( cCommand ), 1, 6 ) ) == "OUTPUT"
                 SetCursor( SC_NONE )
                 ::ShowAppScreen()
                 SetCursor( SC_NORMAL )
                 lDisplay = .f.

              otherwise
                 cResult = "Command error"
                 lDisplay = .t.

           endcase

           DispOutAt( ::oWndCommand:nBottom - 1, ::oWndCommand:nLeft + 1,;
              Space( ::oWndCommand:nRight - ::oWndCommand:nLeft - 1 ),;
              __DbgColors()[ 2 ] )
           if lDisplay
           DispOutAt( ::oWndCommand:nBottom - 1, ::oWndCommand:nLeft + 3, cResult,;
              __DbgColors()[ 2 ] )
           ::oWndCommand:ScrollUp( 1 )
           endif
           DispOutAt( ::oWndCommand:nBottom - 1, ::oWndCommand:nLeft + 1, "> ",;
              __DbgColors()[ 2 ] )
           cCommand := Space( ::oWndCommand:nRight - ::oWndCommand:nLeft - 3 )
           ::oGetListCommand:oGet:VarPut( cCommand )
           ::oGetListCommand:oGet:Buffer := cCommand
           ::oGetListCommand:oGet:Pos := 1
           ::oGetListCommand:oGet:Display()
           exit

      default
          ::oGetListCommand:GetApplyKey( nKey )
   end

return nil

METHOD EditColor( nColor, oBrwColors ) CLASS TDebugger

   local GetList    := {}
   local lPrevScore := Set( _SET_SCOREBOARD, .f. )
   local lPrevExit  := Set( _SET_EXIT, .t. )
   local cColor     := PadR( '"' + ::aColors[ nColor ] + '"',;
                             oBrwColors:aColumns[ 2,1 ]:Width )

   oBrwColors:RefreshCurrent()
   oBrwColors:ForceStable()

   SetCursor( SC_NORMAL )
   @ Row(), Col() + 15 GET cColor COLOR SubStr( ::ClrModal(), 5 ) ;
      VALID iif( Type( cColor ) != "C", ( Alert( "Must be string" ), .f. ), .t. )

   READ
   SetCursor( SC_NONE )

   Set( _SET_SCOREBOARD, lPrevScore )
   Set( _SET_EXIT, lPrevExit )

   if LastKey() != K_ESC
      ::aColors[ nColor ] := &cColor
   endif

   oBrwColors:RefreshCurrent()
   oBrwColors:ForceStable()

return nil

METHOD EditSet( nSet, oBrwSets ) CLASS TDebugger

   local GetList    := {}
   local lPrevScore := Set( _SET_SCOREBOARD, .f. )
   local lPrevExit  := Set( _SET_EXIT, .t. )
   local cSet       := PadR( ValToStr( Set( nSet ) ), oBrwSets:aColumns[ 2,1 ]:Width )

   oBrwSets:RefreshCurrent()
   oBrwSets:ForceStable()

   SetCursor( SC_NORMAL )
   @ Row(), Col() GET cSet COLOR SubStr( ::ClrModal(), 5 )

   READ
   SetCursor( SC_NONE )

   Set( _SET_SCOREBOARD, lPrevScore )
   Set( _SET_EXIT, lPrevExit )

   if LastKey() != K_ESC
      Set( nSet, &cSet )
   endif

   oBrwSets:RefreshCurrent()
   oBrwSets:ForceStable()

return nil

METHOD EditVar( nVar ) CLASS TDebugger

   local cVarName   := ::aVars[ nVar ][ 1 ]
   local uVarValue  := ::aVars[ nVar ][ 2 ]
   local cVarType   := ::aVars[ nVar ][ 3 ]
   local nProcLevel := 1
   local aArray
   local cVarStr

   if ::aVars[ nVar ][ 3 ] == "Local"
      while ProcName( nProcLevel ) != ::aVars[ nVar ][ 4 ]
         nProcLevel++
      end
      uVarValue := __vmVarLGet( nProcLevel, ::aVars[ nVar ][ 2 ] )
   endif

   if ::aVars[ nVar ][ 3 ] == "Static"
      uVarValue := __vmVarSGet( ::aVars[ nVar ][ 2 ] )
   endif

   do case
      case ValType( uVarValue ) == "A"
           ::InputBox( cVarName, uVarValue,, .f. )

      case ValType( uVarValue ) == "O"
           ::InputBox( cVarName, uVarValue,, .f. )

      otherwise
           cVarStr := ::InputBox( cVarName, ValToStr( uVarValue ),;
       { | u | If( Type( u ) == "UE", ( Alert( "Expression error" ), .f. ), .t. ) } )
   endcase

   if LastKey() != K_ESC
      do case
         case uVarValue == "{ ... }"
               cVarType := ::aVars[ nVar ][ 3 ]

               do case
                  case cVarType == "Local"
                     aArray := __vmVarLGet( nProcLevel, ::aVars[ nVar ][ 2 ] )

                  case cVarType == "Static"
                     aArray := __vmVarSGet( ::aVars[ nVar ][ 2 ] )

                  otherwise
                     aArray := ::aVars[ nVar ][ 2 ]
               endcase

               if Len( aArray ) > 0
                  __DbgArrays( aArray, cVarName )
               else
                  Alert( "Array is empty" )
               endif

         case Upper( SubStr( uVarValue, 1, 5 ) ) == "CLASS"
              do case
                 case cVarType == "Local"
                    __DbgObject( __vmVarLGet( nProcLevel, ::aVars[ nVar ][ 2 ] ), cVarName )

                 case cVarType == "Static"
                    __DbgObject( __vmVarSGet( ::aVars[ nVar ][ 2 ] ), cVarName )

                 otherwise
                    __DbgObject( ::aVars[ nVar ][ 2 ], cVarName )
              endcase

         otherwise
            do case
               case cVarType == "Local"
                  __vmVarLSet( nProcLevel, ::aVars[ nVar ][ 2 ], &uVarValue )

               case cVarType == "Static"
                  __vmVarSSet( ::aVars[ nVar ][ 2 ], &uVarValue )

               otherwise
                  ::aVars[ nVar ][ 2 ] := &uVarValue
                  &( ::aVars[ nVar ][ 1 ] ) := ::aVars[ nVar ][ 2 ]
            endcase
      endcase
   endif

   ::oBrwVars:RefreshCurrent()
   ::oBrwVars:ForceStable()

return nil

METHOD EndProc() CLASS TDebugger

   if Len( ::aCallStack ) > 1
   
      // free the browse for the exiting procedure, and recover the one from the stack
      ::oBrwText:=nil
      ::oBrwText:=::aCallStack[1][3]
      ADel( ::aCallStack, 1, .t. )
      if ::oBrwStack != nil .and. ! ::lTrace
         ::oBrwStack:RefreshAll()
      endif
   endif

return nil

METHOD HandleEvent() CLASS TDebugger

   local nPopup, oWnd
   local nKey, nMRow, nMCol, oWin

   if ::lAnimate
      if ::nSpeed != 0
         Inkey( ::nSpeed / 10 )
      endif
      if NextKey() == K_ALT_D
         ::lAnimate := .f.
      endif
      KEYBOARD Chr( 255 ) // Forces a Step(). Only 0-255 range is supported
   endif

   ::lEnd := .f.

   while ! ::lEnd

      nKey := InKey( 0, INKEY_ALL )

      if ::oPullDown:IsOpen()
              ::oPullDown:ProcessKey( nKey )
              if ::oPullDown:nOpenPopup == 0 // Closed
                 ::aWindows[ ::nCurrentWindow ]:SetFocus( .t. )
              endif
      else
         Switch nKey
         case K_LDBLCLK
              if MRow() == 0

              elseif MRow() == MaxRow()

              else
                 nMRow := MRow()
                 nMCol := MCol()
                 for each oWin in ::aWindows
                    if oWin:IsOver( nMRow, nMCol )
                       if ! oWin:lFocused
                          ::aWindows[ ::nCurrentWindow ]:SetFocus( .f. )
                          ::nCurrentWindow := HB_EnumIndex()
                          oWin:SetFocus( .t. )
                       endif
                       oWin:LDblClick( nMRow, nMCol )
                       exit
                    endif
                 next
              endif
              exit

         case K_LBUTTONDOWN
              if MRow() == 0
                 if ( nPopup := ::oPullDown:GetItemOrdByCoors( 0, MCol() ) ) != 0
                    SetCursor( SC_NONE )
                    ::oPullDown:ShowPopup( nPopup )
                 endif

              elseif MRow() == MaxRow()

              else
                 nMRow := MRow()
                 nMCol := MCol()
                 for each oWin in ::aWindows
                    if oWin:IsOver( nMRow, nMCol )
                       if ! oWin:lFocused
                          ::aWindows[ ::nCurrentWindow ]:SetFocus( .f. )
                          ::nCurrentWindow := HB_EnumIndex()
                          oWin:SetFocus( .t. )
                       endif
                       oWin:LButtonDown( nMRow, nMCol )
                       exit
                    endif
                 next
              endif
              exit

         case K_RBUTTONDOWN
              exit

         case K_ESC
              ::RestoreAppStatus()
              s_oDebugger := nil
              s_lExit := .T.
              DispEnd()
              ::Exit()
              exit

         case K_UP
         case K_DOWN
         case K_HOME
         case K_END
         case K_ENTER
         case K_PGDN
         case K_PGUP
              oWnd := ::aWindows[ ::nCurrentWindow ]
              oWnd:KeyPressed( nKey )
              exit

         case K_TAB
              ::NextWindow()
              exit

         case K_SH_TAB
              ::PrevWindow()
              exit

         case K_F8
         case 255
              ::Step()
              exit

         case K_F4
              ::ShowAppScreen()
              exit

         case K_F5
              ::Go()
              exit

         case K_F6
              ::ShowWorkAreas()
              exit

         case K_F9
              ::ToggleBreakPoint()
              exit

         case K_F10
              ::Trace()
              exit

         default

              if ::oWndCommand:lFocused .and. nKey < 272 // Alt
                 ::oWndCommand:KeyPressed( nKey )
              elseif ( nPopup := ::oPullDown:GetHotKeyPos( __dbgAltToKey( nKey ) ) ) != 0
                 if ::oPullDown:nOpenPopup != nPopup
                    SetCursor( SC_NONE )
                    ::oPullDown:ShowPopup( nPopup )
                 endif
              endif
         end
      endif
   end

return nil

METHOD Hide() CLASS TDebugger

   RestScreen( ,,,, ::cAppImage )
   ::cAppImage := nil
   SetColor( ::cAppColors )
   SetCursor( ::nAppCursor )

return nil

METHOD MonoDisplay() CLASS TDebugger

   local oWin

   ::lMonoDisplay := ! ::lMonoDisplay

   ::oPullDown:LoadColors()
   ::oPullDown:Refresh()

   ::BarDisplay()

   for each oWin in ::aWindows
      oWin:LoadColors()
      oWin:Refresh()
   next

return nil

METHOD NextWindow() CLASS TDebugger

   local oWnd

   if Len( ::aWindows ) > 0
      oWnd := ::aWindows[ ::nCurrentWindow++ ]
      oWnd:SetFocus( .f. )
      if ::nCurrentWindow > Len( ::aWindows )
         ::nCurrentWindow := 1
      endif
      while ! ::aWindows[ ::nCurrentWindow ]:lVisible
         ::nCurrentWindow++
         if ::nCurrentWindow > Len( ::aWindows )
            ::nCurrentWindow := 1
         endif
      end
      oWnd := ::aWindows[ ::nCurrentWindow ]
      oWnd:SetFocus( .t. )
   endif

return nil

METHOD PrevWindow() CLASS TDebugger

   local oWnd

   if Len( ::aWindows ) > 0
      oWnd := ::aWindows[ ::nCurrentWindow-- ]
      oWnd:SetFocus( .f. )
      if ::nCurrentWindow < 1
         ::nCurrentWindow := Len( ::aWindows )
      endif
      while ! ::aWindows[ ::nCurrentWindow ]:lVisible
         ::nCurrentWindow--
         if ::nCurrentWindow < 1
            ::nCurrentWindow := Len( ::aWindows )
         endif
      end
      oWnd := ::aWindows[ ::nCurrentWindow ]
      oWnd:SetFocus( .t. )
   endif

return nil

METHOD Show() CLASS TDebugger

   ::cAppImage  := SaveScreen()
   ::nAppRow    := Row()
   ::nAppCol    := Col()
   ::cAppColors := SetColor()
   ::nAppCursor := SetCursor( SC_NONE )

   ::oPullDown:Display()
   ::oWndCode:Show( .t. )
   ::oWndCommand:Show()
   DispOutAt( ::oWndCommand:nBottom - 1, ::oWndCommand:nLeft + 1, ">" )

   ::BarDisplay()

return nil

METHOD ShowAppScreen() CLASS TDebugger

   ::cImage := SaveScreen()
   RestScreen( 0, 0, MaxRow(), MaxCol(), ::cAppImage )

   if LastKey() == K_LBUTTONDOWN
      InKey( 0, INKEY_ALL )
      InKey( 0, INKEY_ALL )
   else
      InKey( 0, INKEY_ALL )
   endif

   while LastKey() == K_MOUSEMOVE
      InKey( 0, INKEY_ALL )
   end
   RestScreen( 0, 0, MaxRow(), MaxCol(), ::cImage )

return nil

METHOD ShowCallStack() CLASS TDebugger

   local n := 1
   local oCol

   if ::oWndStack == nil
      ::oWndCode:nRight -= 16
      ::oBrwText:Resize(,,, ::oBrwText:nRight - 16)
      ::oWndCode:SetFocus( .t. )
      ::oWndStack := TDbWindow():New( 1, MaxCol() - 15, MaxRow() - 6, MaxCol(),;
                                     "Calls" )
      AAdd( ::aWindows, ::oWndStack )
      ::oBrwStack := TBrowseNew( 2, MaxCol() - 14, MaxRow() - 7, MaxCol() - 1 )//2
      ::oBrwStack:ColorSpec := ::aColors[ 3 ] + "," + ::aColors[ 4 ] + "," + ::aColors[ 5 ]
      ::oBrwStack:GoTopBlock := { || n := 1 }
      ::oBrwStack:GoBottomBlock := { || n := Len( ::aCallStack ) }
      ::oBrwStack:SkipBlock := { | nSkip, nPos | nPos := n,;
                             n := iif( nSkip > 0, Min( Len( ::aCallStack ), n + nSkip ),;
                             Max( 1, n + nSkip ) ), n - nPos }

      ::oBrwStack:Cargo := 1 // Actual highligthed row
      ::oBrwStack:autolite := .F.
      ::oBrwStack:colPos:=1
      ::oBrwStack:freeze:=1

      ::oBrwStack:AddColumn( oCol:=TBColumnNew( "",  { || PadC( ::aCallStack[ n ][ 1 ], 14 ) } ) )
      ocol:ColorBlock := { || { iif( n == ::oBrwStack:Cargo, 2, 1 ), 3 } }
      ocol:Defcolor :=    { 2,1 }

      ::oWndStack:bPainted := { || ::oBrwStack:ColorSpec := __DbgColors()[ 2 ] + "," + ;
                                  __DbgColors()[ 5 ] + "," + __DbgColors()[ 4 ],;
                                  ::oBrwStack:RefreshAll(), ::oBrwStack:ForceStable() }
      ::oBrwStack:ForceStable()
      ::oWndStack:Show( .f. )
   endif

return nil

METHOD LoadSettings() CLASS TDebugger

   local cInfo := MemoRead( ::cSettingsFileName )
   local n, cLine, nColor

   for n := 1 to MLCount( cInfo )
      cLine := MemoLine( cInfo, 120, n )
      do case
         case Upper( SubStr( cLine, 1, 14 ) ) == "OPTIONS COLORS"
            cLine := SubStr( cLine, At( "{", cLine ) + 1 )
            nColor := 1
            while nColor < 12
               if At( ",", cLine ) != 0
                  ::aColors[ nColor ] := ;
                     StrTran( SubStr( cLine, 1, At( ",", cLine ) - 1 ), '"', "" )
                  cLine := SubStr( cLine, At( ",", cLine ) + 1 )
               else
                  ::aColors[ nColor ] := ;
                     StrTran( SubStr( cLine, 1, At( "}", cLine ) - 1 ), '"', "" )
               endif
               nColor++
            end
      endcase
   next

return nil

METHOD LoadVars() CLASS TDebugger // updates monitored variables

   local nCount, n, xValue, cName
   local cStaticName, nStaticIndex, nStaticsBase
   local aStatics, aStack

   ::aVars := {}

   if ::lShowPublics
      nCount := __mvDbgInfo( HB_MV_PUBLIC )
      for n := nCount to 1 step -1
         xValue := __mvDbgInfo( HB_MV_PUBLIC, n, @cName )
         if cName != "__DBGSTATICS"  // reserved public used by the debugger
            AAdd( ::aVars, { cName, xValue, "Public" } )
         endif
      next
   endif

   if ::lShowPrivates
      nCount := __mvDbgInfo( HB_MV_PRIVATE )
      for n := nCount to 1 step -1
         xValue := __mvDbgInfo( HB_MV_PRIVATE, n, @cName )
         AAdd( ::aVars, { cName, xValue, "Private" } )
      next
   endif

   if ::lShowStatics
      if Type( "__DbgStatics" ) == "A"
         for each aStatics in __DbgStatics
            for each cStaticName in aStatics[ 2 ]
               nStaticIndex := aStatics[1] + HB_EnumIndex()
               AAdd( ::aVars, { cStaticName, nStaticIndex, "Static" } )
            next
         next
      endif
   endif

   if ::lShowLocals
      for each aStack in ::aCallStack[ 1 ][ 2 ]
         AAdd( ::aVars, aStack )
      next
   endif

   if ::lSortVars
      ::Sort()
   endif

return nil

METHOD ShowVars() CLASS TDebugger

   local n
   local nWidth
   Local oCol
   local lRepaint := .f.

   if ::lGo
      return nil
   endif

   if ::oWndVars == nil

      n := 1

      ::LoadVars()
      ::oWndVars := TDbWindow():New( 1, 0, Min( 7, Len( ::aVars ) + 2 ),;
         MaxCol() - iif( ::oWndStack != nil, ::oWndStack:nWidth(), 0 ),;
         "Monitor:" + iif( ::lShowLocals, " Local", "" ) + ;
         iif( ::lShowStatics, " Static", "" ) + iif( ::lShowPrivates, " Private", "" ) + ;
         iif( ::lShowPublics, " Public", "" ) )

      ::oWndCode:nTop += ::oWndVars:nBottom
      ::oBrwText:Resize( ::oBrwText:nTop + ::oWndVars:nBottom )
      ::oBrwText:RefreshAll()
      ::oWndCode:SetFocus( .t. )

      ::oWndVars:Show( .f. )
      AAdd( ::aWindows, ::oWndVars )
      ::oWndVars:bLButtonDown := { | nMRow, nMCol | ::WndVarsLButtonDown( nMRow, nMCol ) }
      ::oWndVars:bLDblClick   := { | nMRow, nMCol | ::EditVar( n ) }
      ::oBrwVars := TBrowseNew( 2, 1, ::oWndVars:nBottom - 1, MaxCol() - iif( ::oWndStack != nil,;
                               ::oWndStack:nWidth(), 0 ) - 1 )

      ::oBrwVars:Cargo :={ 1,{}} // Actual highligthed row
      ::oBrwVars:ColorSpec := ::aColors[ 2 ] + "," + ::aColors[ 5 ] + "," + ::aColors[ 3 ]
      ::oBrwVars:GOTOPBLOCK := { || ::oBrwVars:cargo[ 1 ] := Min( 1, Len( ::aVars ) ) }
      ::oBrwVars:GoBottomBlock := { || ::oBrwVars:cargo[ 1 ] := Len( ::aVars ) }
      ::oBrwVars:SkipBlock = { | nSkip, nOld | nOld := ::oBrwVars:Cargo[ 1 ],;
                               ::oBrwVars:Cargo[ 1 ] += nSkip,;
                               ::oBrwVars:Cargo[ 1 ] := Min( Max( ::oBrwVars:Cargo[ 1 ], 1 ),;
                                                             Len( ::aVars ) ),;
                               If( Len( ::aVars ) > 0, ::oBrwVars:Cargo[ 1 ] - nOld, 0 ) }

      nWidth := ::oWndVars:nWidth() - 1
      ::oBrwVars:AddColumn( oCol:=TBColumnNew( "",  { || If( Len( ::aVars ) > 0, AllTrim( Str( ::oBrwVars:Cargo[1] -1 ) ) + ") " + ;
         PadR( GetVarInfo( ::aVars[ Max( ::oBrwVars:Cargo[1], 1 ) ] ),;
         ::oWndVars:nWidth() - 5 ), "" ) } ) )
      AAdd(::oBrwVars:Cargo[2],::avars)
      oCol:DefColor:={1,2}
      if Len( ::aVars ) > 0
         ::oBrwVars:ForceStable()
      endif

      ::oWndVars:bPainted     := { || if(Len( ::aVars ) > 0, ( ::obrwVars:ForceStable(),RefreshVarsS(::oBrwVars) ),) }

      ::oWndVars:bKeyPressed := { | nKey | ( iif( nKey == K_DOWN ;
      , ::oBrwVars:Down(), nil ), iif( nKey == K_UP, ::oBrwVars:Up(), nil ) ;
      , iif( nKey == K_PGDN, ::oBrwVars:PageDown(), nil ) ;
      , iif( nKey == K_PGUP, ::oBrwVars:PageUp(), nil ) ;
      , iif( nKey == K_HOME, ::oBrwVars:GoTop(), nil ) ;
      , iif( nKey == K_END, ::oBrwVars:GoBottom(), nil ) ;
      , iif( nKey == K_ENTER, ::EditVar( ::oBrwVars:Cargo[1] ), nil ), ::oBrwVars:ForceStable() ) }

   else
      ::LoadVars()

      ::oWndVars:cCaption := "Monitor:" + ;
      iif( ::lShowLocals, " Local", "" ) + ;
      iif( ::lShowStatics, " Static", "" ) + ;
      iif( ::lShowPrivates, " Private", "" ) + ;
      iif( ::lShowPublics, " Public", "" )

      if Len( ::aVars ) == 0
         if ::oWndVars:nBottom - ::oWndVars:nTop > 1
            ::oWndVars:nBottom := ::oWndVars:nTop + 1
            lRepaint := .t.
         endif
      endif
      if Len( ::aVars ) > ::oWndVars:nBottom - ::oWndVars:nTop - 1
         ::oWndVars:nBottom := ::oWndVars:nTop + Min( Len( ::aVars ) + 1, 7 )
         ::oBrwVars:nBottom := ::oWndVars:nBottom - 1
         ::oBrwVars:Configure()
         lRepaint := .t.
      endif
      if Len( ::aVars ) < ::oWndVars:nBottom - ::oWndVars:nTop - 1
         ::oWndVars:nBottom := ::oWndVars:nTop + Len( ::aVars ) + 1
         ::oBrwVars:nBottom := ::oWndVars:nBottom - 1
         ::oBrwVars:Configure()
         lRepaint := .t.
      endif
      if ! ::oWndVars:lVisible
         ::oWndCode:nTop := ::oWndVars:nBottom + 1
         ::oBrwText:Resize( ::oWndVars:nBottom + 2 )
         ::oWndVars:Show()
      else
         if lRepaint
            ::oWndCode:nTop := ::oWndVars:nBottom + 1
            ::oBrwText:Resize( ::oWndCode:nTop + 1 )
            ::oWndCode:Refresh()
            ::oWndVars:Refresh()
         endif
      endif
      if Len( ::aVars ) > 0
         ::oBrwVars:RefreshAll()
         ::oBrwVars:ForceStable()
      endif
   endif

return nil

static function GetVarInfo( aVar )

   local nProcLevel := 1

   do case
      case aVar[ 3 ] == "Local"
           while ProcName( nProcLevel ) != aVar[ 4 ]
              nProcLevel++
           end
           return aVar[ 1 ] + " <Local, " + ;
                  ValType( __vmVarLGet( nProcLevel, aVar[ 2 ] ) ) + ;
                  ">: " + ValToStr( __vmVarLGet( nProcLevel, aVar[ 2 ] ) )

      case aVar[ 3 ] == "Public" .or. aVar[ 3 ] == "Private"
           return aVar[ 1 ] + " <" + aVar[ 3 ] + ", " + ValType( aVar[ 2 ] ) + ;
                  ">: " + ValToStr( aVar[ 2 ] )

      case aVar[ 3 ] == "Static"
           return aVar[ 1 ] + " <Static, " + ;
                  ValType( __vmVarSGet( aVar[ 2 ] ) ) + ;
                  ">: " + ValToStr( __vmVarSGet( aVar[ 2 ] ) )
   endcase

return ""

static function CompareLine( Self )

return { | a | a[ 1 ] == Self:oBrwText:nRow }  // it was nLine

METHOD ShowCode( cModuleName ) CLASS TDebugger

   // always treat filename as lower case - we need it consistent for comparisons   
   local cFunction := SubStr( cModuleName, RAt( ":", cModuleName ) + 1 )
   local cPrgName  := lower(SubStr( cModuleName, 1, RAt( ":", cModuleName ) - 1 ))

   AIns( ::aCallStack, 1, { cFunction, {} , ::oBrwText}, .t. ) // function name and locals array
                                                               // and the code window browse
if !::lGo
   if ::oWndStack != nil
      ::oBrwStack:RefreshAll()
   endif

   if cPrgName != ::cPrgName
      ::cPrgName := cPrgName
      
      // warn if the source code file can't be found
      IF .NOT. file(::cPathForFiles+::cPrgName)
        Alert("Can't find source file "+::cPathForFiles+::cPrgName+" check path!")
      ELSE
        // store the open files in a stack, so we can
        // easily get back to the correct file on procedure exit
        ::oBrwText := TBrwText():New( ::oWndCode:nTop + 1, ::oWndCode:nLeft + 1,;
                     ::oWndCode:nBottom - 1, ::oWndCode:nRight - 1, ::cPathForFiles+::cPrgName,;
                     __DbgColors()[ 2 ] + "," + __DbgColors()[ 5 ] + "," + ;
                     __DbgColors()[ 3 ] + "," + __DbgColors()[ 6 ] )
        ::oWndCode:SetCaption( ::cPathForFiles+::cPrgName )
      ENDIF
   endif
endif
return nil

METHOD Open() CLASS TDebugger
   local cFileName := ::InputBox( "Please enter the filename", Space( 30 ) )
  if (cFileName != ::cPrgName .OR. valtype(::cPrgName)=='U')
      ::cPrgName := cFileName
      ::oBrwText := TBrwText():New( ::oWndCode:nTop + 1, ::oWndCode:nLeft + 1,;
                   ::oWndCode:nBottom - 1, ::oWndCode:nRight - 1, ::cPathForFiles+::cPrgName,;
                   __DbgColors()[ 2 ] + "," + __DbgColors()[ 5 ] + "," + ;
                   __DbgColors()[ 3 ] + "," + __DbgColors()[ 6 ] )

      ::oWndCode:SetCaption( ::cPrgName )
   endif
return nil

METHOD OSShell() CLASS TDebugger

   local cImage := SaveScreen()
   local cColors := SetColor()
   local cOs    := Upper( OS() )
   local cShell := GetEnv("COMSPEC")
   local bLastHandler := ErrorBlock({ |objErr| BREAK (objErr) })
   local oE

   SET COLOR TO "W/N"
   CLS
   SetCursor( SC_NORMAL )

   begin sequence
      if At("WINDOWS", cOs) != 0 .OR. At("DOS", cOs) != 0 .OR. At("OS/2", cOs) != 0
         RUN ( cShell )

      else
         Alert( "Not implemented yet!" )

      endif

   recover using oE
      Alert("Error: " + oE:description)

   end sequence

   ErrorBlock(bLastHandler)

   SetCursor( SC_NONE )
   RestScreen( ,,,, cImage )
   SetColor( cColors )

return nil

METHOD HideCallStack() CLASS TDebugger

   ::lShowCallStack = .f.

   if ::oWndStack != nil
      DispBegin()
      ::oWndStack:Hide()
      ::RemoveWindow( ::oWndStack )
      ::oWndStack = nil
      ::oWndCode:Hide()
      ::oWndCode:nRight += 16
      ::oWndCode:Show( .t. )
      ::oBrwText:Resize( ,,, ::oBrwText:nRight + 16 )
      ::oBrwText:GotoLine( ::oBrwText:nActiveLine )
      if ::oWndVars != nil
         ::oWndVars:Hide()
         ::oWndVars:nRight += 16
         ::oWndVars:Show( .f. )
      endif
      DispEnd()
      ::nCurrentWindow = 1
   endif
return nil

METHOD HideVars() CLASS TDebugger

   ::oWndVars:Hide()
   ::oWndCode:nTop := 1
   ::oWndCode:SetFocus( .t. )
   ::oBrwText:Resize( 2 )

return nil

METHOD InputBox( cMsg, uValue, bValid, lEditable ) CLASS TDebugger

   local nTop    := ( MaxRow() / 2 ) - 5
   local nLeft   := ( MaxCol() / 2 ) - 25
   local nBottom := ( MaxRow() / 2 ) - 3
   local nRight  := ( MaxCol() / 2 ) + 25
   local cType   := ValType( uValue )
   local uTemp   := PadR( uValue, nRight - nLeft - 1 )
   local GetList := {}
   local nOldCursor
   local lScoreBoard := Set( _SET_SCOREBOARD, .f. )
   local lExit
   local oWndInput := TDbWindow():New( nTop, nLeft, nBottom, nRight, cMsg,;
                                       ::oPullDown:cClrPopup )

   DEFAULT lEditable TO .t.
   oWndInput:lShadow := .t.
   oWndInput:Show()
   tracelog( cMsg, uValue, bValid, lEditable )
   if lEditable
      if bValid == nil
         @ nTop + 1, nLeft + 1 GET uTemp COLOR "," + __DbgColors()[ 5 ]
         else
            @ nTop + 1, nLeft + 1 GET uTemp VALID Eval( bValid, uTemp ) ;
              COLOR "," + __DbgColors()[ 5 ]
         endif

      nOldCursor := SetCursor( SC_NORMAL )
      READ
      SetCursor( nOldCursor )
   else
      @ nTop + 1, nLeft + 1 GET uTemp VALID bValid COLOR "," + __DbgColors()[ 5 ]
      SetPos( nTop + 1, nLeft + 1 )
      nOldCursor := SetCursor( SC_NONE )

      lExit = .f.

      while ! lExit
         Inkey( 0 )

         do case
            case LastKey() == K_ESC
               lExit = .t.

            case LastKey() == K_ENTER
               if cType == "A"
                  if Len( uValue ) == 0
                     Alert( "Array is empty" )
                  else
                     __DbgArrays( uValue, cMsg )
                  endif

               elseif cType == "O"
                  __DbgObject( uValue, cMsg )

               else
                  Alert( "Value cannot be edited" )
               endif

            otherwise
               Alert( "Value cannot be edited" )
         endcase
      end

      SetCursor( nOldCursor )
   endif

   nOldCursor := SetCursor( SC_NORMAL )
   READ
   SetCursor( nOldCursor )
   oWndInput:Hide()
   Set( _SET_SCOREBOARD, lScoreBoard )

   Switch cType
      case "C"
           uTemp := AllTrim( uTemp )
           exit

      case "D"
           uTemp := CToD( uTemp )
           exit

      case "N"
           uTemp := Val( uTemp )
           exit

   end

return iif( LastKey() != K_ESC, uTemp, uValue )

// test to see if this line in the currently running file has a breakpoint set
METHOD IsBreakPoint( nLine ) CLASS TDebugger
return AScan( ::aBreakPoints, { | aBreak | (aBreak[ 1 ] == nLine) .AND. (aBreak [ 2 ] == ::cPrgName) } ) != 0


METHOD GotoLine( nLine ) CLASS TDebugger

   local nRow, nCol

   if ::oBrwVars != nil
      ::ShowVars()
   endif

   ::oBrwText:GotoLine( nLine )
   nRow = Row()
   nCol = Col()

   // no source code line stored yet
   if ::oBrwStack != nil .and. Len( ::aCallStack ) > 0 .and. ;
      ::aCallStack[ ::oBrwStack:Cargo ][ 3 ] == nil
      ::aCallStack[ ::oBrwStack:Cargo ][ 3 ] = nLine
   endif
   if ::oBrwStack != nil .and. ! ::oBrwStack:Stable
      ::oBrwStack:ForceStable()
   endif
   if ::oWndCode:lFocused .and. SetCursor() != SC_SPECIAL1
      SetPos( nRow, nCol )
      SetCursor( SC_SPECIAL1 )
   endif

return nil

METHOD Local() CLASS TDebugger

   ::lShowLocals := ! ::lShowLocals

   if ::lShowPublics .or. ::lShowPrivates .or. ::lShowStatics .or. ::lShowLocals
      ::ShowVars()
   else
      ::HideVars()
   endif

return nil

METHOD Private() CLASS TDebugger

   ::lShowPrivates := ! ::lShowPrivates

   if ::lShowPublics .or. ::lShowPrivates .or. ::lShowStatics .or. ::lShowLocals
      ::ShowVars()
   else
      ::HideVars()
   endif

return nil

METHOD Public() CLASS TDebugger

   ::lShowPublics := ! ::lShowPublics

   if ::lShowPublics .or. ::lShowPrivates .or. ::lShowStatics .or. ::lShowLocals
      ::ShowVars()
   else
      ::HideVars()
   endif

return nil

METHOD RestoreAppStatus() CLASS TDebugger

   ::cImage := SaveScreen()
   DispBegin()
   RestScreen( 0, 0, MaxRow(), MaxCol(), ::cAppImage )
   SetPos( ::nAppRow, ::nAppCol )
   SetColor( ::cAppColors )
   SetCursor( ::nAppCursor )
   DispEnd()		// own up! who forgot this?
   
return nil

METHOD RestoreSettings() CLASS TDebugger

   local oWin

   ::cSettingsFileName := ::InputBox( "File name", ::cSettingsFileName )

   if LastKey() != K_ESC
      ::LoadSettings()

      ::oPullDown:LoadColors()
      ::oPullDown:Refresh()
      ::BarDisplay()

      for each oWin in ::aWindows
        oWin:LoadColors()
        oWin:Refresh()
      next
   endif

return nil

METHOD SaveAppStatus() CLASS TDebugger

   ::cAppImage  := SaveScreen()
   ::nAppRow    := Row()
   ::nAppCol    := Col()
   ::cAppColors := SetColor()
   ::nAppCursor := SetCursor()
   RestScreen( 0, 0, MaxRow(), MaxCol(), ::cImage )
   // SetCursor( SC_NONE )
   DispEnd()

return nil

METHOD SaveSettings() CLASS TDebugger

   local cInfo := "", nLen, oWnd, cColor , n

   ::cSettingsFileName := ::InputBox( "File name", ::cSettingsFileName )

   if LastKey() != K_ESC

      if ! Empty( ::cPathForFiles )
         cInfo += "Options Path " + ::cPathForFiles + Chr( 13 ) + Chr( 10 )
      endif

      cInfo += "Options Colors {"
      nLen  := Len( ::aColors )

      for each cColor in ::aColors
         cInfo += '"' + cColor + '"'
         if HB_EnumIndex() < nLen
            cInfo += ","
         endif
      next

      cInfo += "}" + Chr( 13 ) + Chr( 10 )

      if ::lMonoDisplay
         cInfo += "Options mono " + Chr( 13 ) + Chr( 10 )
      endif

      if ::nSpeed != 0
         cInfo += "Run Speed " + AllTrim( Str( ::nSpeed ) ) + Chr( 13 ) + Chr( 10 )
      endif

      for each oWnd in ::aWindows
         cInfo += "Window Size " + AllTrim( Str( oWnd:nBottom - oWnd:nTop + 1 ) ) + " "
         cInfo += AllTrim( Str( oWnd:nRight - oWnd:nLeft + 1 ) ) + Chr( 13 ) + Chr( 10 )
         cInfo += "Window Move " + AllTrim( Str( oWnd:nTop ) ) + " "
         cInfo += AllTrim( Str( oWnd:nLeft ) ) + Chr( 13 ) + Chr( 10 )
         cInfo += "Window Next" + Chr( 13 ) + Chr( 10 )
      next

      if ::nTabWidth != 4
         cInfo += "Options Tab " + AllTrim( Str( ::nTabWidth ) ) + Chr( 13 ) + Chr( 10 )
      endif

      if ::lShowStatics
         cInfo += "Monitor Static" + HB_OsNewLine()
      endif

      if ::lShowPublics
         cInfo += "Monitor Public" + HB_OsNewLine()
      endif

      if ::lShowLocals
         cInfo += "Monitor Local" + HB_OsNewLine()
      endif

      if ::lShowPrivates
         cInfo += "Monitor Private" + HB_OsNewLine()
      endif

      if ::lSortVars
         cInfo += "Monitor Sort" + HB_OsNewLine()
      endif

      if ::lShowCallStack
         cInfo += "View CallStack" + HB_OsNewLine()
      endif

      if ! Empty( ::aBreakPoints )
         for n := 1 to Len( ::aBreakPoints )
            cInfo += "BP " + AllTrim( Str( ::aBreakPoints[ n ][ 1 ] ) ) + " " + ;
                     AllTrim( ::aBreakPoints[ n ][ 2 ] ) + HB_OsNewLine()
         next
      endif
      MemoWrit( ::cSettingsFileName, cInfo )
   endif

return nil
METHOD Stack() CLASS TDebugger

   if ::lShowCallStack := ! ::lShowCallStack
      ::ShowCallStack()
   else
      ::HideCallStack()
   endif

return nil

METHOD Static() CLASS TDebugger

   ::lShowStatics := ! ::lShowStatics

   if ::lShowPublics .or. ::lShowPrivates .or. ::lShowStatics .or. ::lShowLocals
      ::ShowVars()
   else
      ::HideVars()
   endif

return nil

// Toggle a breakpoint at the cursor position in the currently viewed file
// which may be different from the file in which execution was broken
METHOD ToggleBreakPoint() CLASS TDebugger
   // look for a breakpoint which matches both line number and program name
   local nAt := AScan( ::aBreakPoints, { | aBreak | aBreak[ 1 ] == ;
                       ::oBrwText:nRow ;
                       .AND. aBreak [ 2 ] == ::cPrgName} ) // it was nLine

   if nAt == 0
      // didn't find one, so add a new one
      AAdd( ::aBreakPoints, { ::oBrwText:nRow, ::cPrgName } )     // it was nLine
      ::oBrwText:ToggleBreakPoint(::oBrwText:nRow, .T.)
   else
      // remove the matching one
      ADel( ::aBreakPoints, nAt, .t. )
      ::oBrwText:ToggleBreakPoint(::oBrwText:nRow, .F.)
   endif

   ::oBrwText:RefreshCurrent()

return nil

METHOD ViewSets() CLASS TDebugger

   local oWndSets := TDbWindow():New( 1, 8, MaxRow() - 2, MaxCol() - 8,;
                                      "System Settings[1..47]", ::ClrModal() )
   local aSets := { "Exact", "Fixed", "Decimals", "DateFormat", "Epoch", "Path",;
                    "Default", "Exclusive", "SoftSeek", "Unique", "Deleted",;
                    "Cancel", "Debug", "TypeAhead", "Color", "Cursor", "Console",;
                    "Alternate", "AltFile", "Device", "Extra", "ExtraFile",;
                    "Printer", "PrintFile", "Margin", "Bell", "Confirm", "Escape",;
                    "Insert", "Exit", "Intensity", "ScoreBoard", "Delimeters",;
                    "DelimChars", "Wrap", "Message", "MCenter", "ScrollBreak",;
                    "EventMask", "VideoMode", "MBlockSize", "MFileExt",;
                    "StrictRead", "Optimize", "Autopen", "Autorder", "AutoShare" }

   local oBrwSets := TBrowseNew( oWndSets:nTop + 1, oWndSets:nLeft + 1,;
                                 oWndSets:nBottom - 1, oWndSets:nRight - 1 )
   local n := 1
   local nWidth := oWndSets:nRight - oWndSets:nLeft - 1
   local oCol
   oBrwSets:Cargo :={ 1,{}} // Actual highligthed row
   oBrwSets:autolite:=.f.
   oBrwSets:ColorSpec := ::ClrModal()
   oBrwSets:GOTOPBLOCK := { || oBrwSets:cargo[ 1 ]:= 1 }
   oBrwSets:GoBottomBlock := { || oBrwSets:cargo[ 1 ]:= Len(oBrwSets:cargo[ 2 ][ 1 ])}
   oBrwSets:SKIPBLOCK := { |nPos| ( nPos:= ArrayBrowseSkip(nPos, oBrwSets), oBrwSets:cargo[ 1 ]:= ;
   oBrwSets:cargo[ 1 ] + nPos,nPos ) }
   oBrwSets:AddColumn( ocol := TBColumnNew( "", { || PadR( aSets[ oBrwSets:cargo[ 1 ] ], 12 ) } ) )
   aadd(oBrwSets:Cargo[2],asets)
   ocol:defcolor:={1,2}
   oBrwSets:AddColumn( oCol := TBColumnNew( "",;
                       { || PadR( ValToStr( Set( oBrwSets:cargo[ 1 ]  ) ), nWidth - 13 ) } ) )
   ocol:defcolor:={1,3}
   ocol:width:=40
   oWndSets:bPainted    := { || oBrwSets:ForceStable(),RefreshVarsS(oBrwSets)}
   oWndSets:bKeyPressed := { | nKey | SetsKeyPressed( nKey, oBrwSets, Len( aSets ),;
                            oWndSets, "System Settings",;
                            { || ::EditSet( n, oBrwSets ) } ) }

   SetCursor( SC_NONE )
   oWndSets:ShowModal()

return nil

METHOD WndVarsLButtonDown( nMRow, nMCol ) CLASS TDebugger

   if nMRow > ::oWndVars:nTop .and. ;
      nMRow < ::oWndVars:nBottom .and. ;
      nMCol > ::oWndVars:nLeft .and. ;
      nMCol < ::oWndVars:nRight
      if nMRow - ::oWndVars:nTop >= 1 .and. ;
         nMRow - ::oWndVars:nTop <= Len( ::aVars )
         while ::oBrwVars:RowPos > nMRow - ::oWndVars:nTop
            ::oBrwVars:Up()
            ::oBrwVars:ForceStable()
         end
         while ::oBrwVars:RowPos < nMRow - ::oWndVars:nTop
            ::oBrwVars:Down()
            ::oBrwVars:ForceStable()
         end
      endif
   endif

return nil

static procedure SetsKeyPressed( nKey, oBrwSets, nSets, oWnd, cCaption, bEdit )


   local nSet := oBrwSets:cargo[1]
   local cTemp:=str(nSet,4)

   Local nRectoMove

   Switch nKey
      case K_UP
              oBrwSets:Up()
              exit
      case K_DOWN
              oBrwSets:Down()
              exit
      case K_HOME
      case K_CTRL_PGUP
      case K_CTRL_HOME
              oBrwSets:GoTop()
              exit
      case K_END
      case K_CTRL_PGDN
      case K_CTRL_END
              oBrwSets:GoBottom()
              exit
      Case K_PGDN
              oBrwSets:pageDown()
              exit
      Case K_PGUP
              oBrwSets:PageUp()
              exit

      case K_ENTER
           if bEdit != nil
              Eval( bEdit )
           endif
           if LastKey() == K_ENTER
              KEYBOARD Chr( K_DOWN )
           endif
           exit

   end

   RefreshVarsS(oBrwSets)

   oWnd:SetCaption( cCaption + "[" + AllTrim( Str( oBrwSets:Cargo[1] ) ) + ;
                       ".." + AllTrim( Str( nSets ) ) + "]" )
return

static procedure SetsKeyVarPressed( nKey, oBrwSets, nSets, oWnd, bEdit )
   Local nRectoMove
   local nSet := oBrwSets:Cargo[1]

   Switch nKey
      case K_UP
              oBrwSets:Up()
              exit

      case K_DOWN
              oBrwSets:Down()
              exit

      case K_HOME
      case K_CTRL_PGUP
      case K_CTRL_HOME
              oBrwSets:GoTop()
              exit

      case K_END
      case K_CTRL_PGDN
      case K_CTRL_END
              oBrwSets:GoBottom()
              exit

      Case K_PGDN
              oBrwSets:pageDown()
              exit

      Case K_PGUP
              oBrwSets:PageUp()
              exit

      case K_ENTER
           if bEdit != nil
              Eval( bEdit )
           endif
           if LastKey() == K_ENTER
              KEYBOARD Chr( K_DOWN )
           endif
           exit

   end

return


static function ValToStr( uVal )

   local cType := ValType( uVal )
   local cResult := "U"

   Switch cType
      case "U"
           cResult := "NIL"
           exit

      case "A"
           cResult := "{ ... }"
           exit

      Case "B"
           cResult:= "{ || ... }"
           exit

      case "C"
      case "M"
           cResult := '"' + uVal + '"'
           exit

      case "L"
           cResult := iif( uVal, ".T.", ".F." )
           exit

      case "D"
           cResult := DToC( uVal )
           exit

      case "N"
           cResult := AllTrim( Str( uVal ) )
           exit

      case "O"
           cResult := "Class " + uVal:ClassName() + " object"
           exit
   end

return cResult


METHOD LineNumbers() CLASS TDebugger

   ::oBrwText:lLineNumbers := !::oBrwText:lLineNumbers
   ::oBrwText:RefreshAll()

return Self

METHOD Locate( nMode ) CLASS TDebugger

   local cValue

   DEFAULT nMode TO 0

   cValue := ::InputBox( "Search string", ::cSearchString )

   if empty( cValue )
      return nil
   endif

   ::cSearchString := cValue

return ::oBrwText:Search( ::cSearchString, ::lCaseSensitive, 0 )

METHOD FindNext() CLASS TDebugger

   local lFound

   if Empty( ::cSearchString )
      lFound := ::Locate( 1 )
   else
      lFound := ::oBrwText:Search( ::cSearchString, ::lCaseSensitive, 1 )
   endif

return lFound

METHOD FindPrevious() CLASS TDebugger

   local lFound

   if Empty( ::cSearchString )
      lFound := ::Locate( 2 )
   else
      lFound := ::oBrwText:Search( ::cSearchString, ::lCaseSensitive, 2 )
   endif

return lFound

METHOD RemoveWindow( oWnd ) CLASS TDebugger

  local n := AScan( ::aWindows, { | o | o == oWnd } )

  if n != 0
     ::aWindows = ADel ( ::aWindows, n, .t. )
  endif

  ::nCurrentWindow = 1

return nil

METHOD SearchLine() CLASS TDebugger

   local cLine

   cLine := ::InputBox( "Line number", "1" )

   if Val( cLine ) > 0
      ::oBrwText:GotoLine ( Val( cLine ) )
   endif

return nil

function __DbgColors()

return iif( ! s_oDebugger:lMonoDisplay, s_oDebugger:aColors,;
           { "W+/N", "W+/N", "N/W", "N/W", "N/W", "N/W", "W+/N",;
             "N/W", "W+/W", "W/N", "W+/N" } )

function __Dbg()

return s_oDebugger

static function myColors( oBrowse, aColColors )
   local nColPos := oBrowse:colpos
   local nCol

   for each nCol in aColColors
      oBrowse:colpos := nCol
      oBrowse:hilite()
   next

   oBrowse:colpos := nColPos
return Nil

static procedure RefreshVarsS( oBrowse )

   local nLen := Len(oBrowse:aColumns)

   if ( nLen == 2 )
      oBrowse:dehilite():colpos:=2
   endif
   oBrowse:dehilite():forcestable()
   if ( nLen == 2 )
      oBrowse:hilite():colpos:=1
   endif
   oBrowse:hilite()
   return

static function ArrayBrowseSkip( nPos, oBrwSets,n )

   return iif( oBrwSets:cargo[ 1 ] + nPos < 1, 0 - oBrwSets:cargo[ 1 ] + 1 , ;
      iif( oBrwSets:cargo[ 1 ] + nPos > Len(oBrwSets:cargo[ 2 ][ 1 ]), ;
      Len(oBrwSets:cargo[ 2 ][ 1 ]) - oBrwSets:cargo[ 1 ], nPos ) )
      
static function DoCommand( o,cCommand )
   local bLastHandler, cResult, nLocals := len( o:aCallStack[1][2] )
   local nProcLevel := 1, oE, i, vtmp

   if nLocals > 0
      while ProcName( nProcLevel ) != o:aCallStack[1][2][1][4]
         nProcLevel++
      enddo
      for i := 1 to nLocals
         __mvPrivate( o:aCallStack[1][2][i][1] )
         __mvPut( o:aCallStack[1][2][i][1], ;
              __vmVarLGet( nProcLevel, o:aCallStack[1][2][i][2] ) )
      next
   endif

   bLastHandler := ErrorBlock({ |objErr| BREAK (objErr) })

   if SubStr( LTrim( cCommand ), 1, 3 ) == "?? "

      begin sequence
         o:Inspect( AllTrim( SubStr( LTrim( cCommand ), 4 ) ),;
                    &( AllTrim( SubStr( LTrim( cCommand ), 4 ) ) ) )
         cResult := ""
      recover using oE
         cResult = "Command error: " + oE:description
      end sequence

   elseif SubStr( LTrim( cCommand ), 1, 2 ) == "? "

      begin sequence
          cResult := ValToStr( &( AllTrim( SubStr( LTrim( cCommand ), 3 ) ) ) )

      recover using oE
          cResult := "Command error: " + oE:description

      end sequence

   else
      cResult := "Command error"

   endif

   ErrorBlock(bLastHandler)

   for i := 1 to nLocals
      vtmp := __mvGet( o:aCallStack[1][2][i][1] )
      if !(Valtype( vtmp ) $ "AO")
         __vmVarLSet( nProcLevel, o:aCallStack[1][2][i][2], vtmp )
      endif
   next

Return cResult

METHOD ShowHelp( nTopic ) CLASS TDebugger

   local nCursor := SetCursor( SC_NONE )

   __dbgHelp( nTopic )
   SetCursor( nCursor )

return nil

METHOD Inspect( uValue, cValueName ) CLASS TDebugger

   uValue = ::InputBox( uValue, cValueName,, .f. )

return nil

