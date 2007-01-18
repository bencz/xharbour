/*
 * $Id: alert.prg,v 1.23 2007/01/11 11:08:34 modalsist Exp $
 */

/*
 * Harbour Project source code:
 * ALERT() function
 *
 * Released to Public Domain by Vladimir Kazimirchik <v_kazimirchik@yahoo.com>
 * www - http://www.harbour-project.org
 *
 */

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 1999-2001 Viktor Szakats <viktor.szakats@syenar.hu>
 *    Changes for higher Clipper compatibility, console mode, extensions
 *    __NONOALERT()
 *
 * See doc/license.txt for licensing terms.
 *
 */

#include "hbsetup.ch"

#include "box.ch"
#include "common.ch"
#include "inkey.ch"
#include "setcurs.ch"

/* TOFIX: Clipper defines a clipped window for Alert() [vszakats] */

/* NOTE: Clipper will return NIL if the first parameter is not a string, but
         this is not documented. This implementation converts the first
         parameter to a string if another type was passed. You can switch back
         to Clipper compatible mode by defining constant
         HB_C52_STRICT. [vszakats] */

/* NOTE: Clipper handles these buttons { "Ok", "", "Cancel" } in a buggy way.
         This is fixed. [vszakats] */

/* NOTE: nDelay parameter is a Harbour extension. */

#define INRANGE( xLo, xVal, xHi )       ( xVal >= xLo .AND. xVal <= xHi )

#ifdef HB_C52_UNDOC
STATIC s_lNoAlert
#endif


FUNCTION Alert( xMessage, aOptions, cColorNorm, nDelay )

   LOCAL nChoice
   LOCAL aSay, nPos, nWidth, nOpWidth, nInitRow, nInitCol, nEval
   LOCAL nKey, aPos, nCurrent, aHotkey, aOptionsOK, cEval
   LOCAL cColorHigh

   LOCAL nOldRow
   LOCAL nOldCol
   LOCAL nOldCursor
   LOCAL cOldScreen

   LOCAL nOldDispCount
   LOCAL nCount
   LOCAL nLen, sCopy
   LOCAL lWhile

   LOCAL cColorStr,cColorPair1,cColorPair2,cColor11,cColor12,cColor21,cColor22
   LOCAL nCommaSep,nSlash

#ifdef HB_COMPAT_C53
   LOCAL nMRow, nMCol
#endif

   /* TOFIX: Clipper decides at runtime, whether the GT is linked in,
             if it is not, the console mode is choosen here. [vszakats] */
   LOCAL lConsole := .F.

   /* Variables to control CT Windows  */
   LOCAL lCtWin   := ( Ct_WNum() != 0 )  // There is a CT Window active ?

   LOCAL cCtColor
   LOCAL nCol


#ifdef HB_C52_UNDOC

   DEFAULT s_lNoAlert TO hb_argCheck( "NOALERT" )

   IF s_lNoAlert
      RETURN NIL
   ENDIF

#endif

   aSay := {}

#ifdef HB_C52_STRICT

   IF !ISCHARACTER( xMessage )
      RETURN NIL
   ENDIF

   DO WHILE ( nPos := At( ';', xMessage ) ) != 0
      AAdd( aSay, Left( xMessage, nPos - 1 ) )
      xMessage := SubStr( xMessage, nPos + 1 )
   ENDDO
   AAdd( aSay, xMessage )

#else

   IF PCount() == 0
      RETURN NIL
   ENDIF

   IF ISARRAY( xMessage )

      FOR EACH cEval IN xMessage
         IF ISCHARACTER( cEval )
            AAdd( aSay, cEval )
         ENDIF
      NEXT

   ELSE

      SWITCH ValType( xMessage )
         CASE "C"
         CASE "M"
            EXIT

         CASE "N"
            xMessage := LTrim( Str( xMessage ) )
            EXIT

         CASE "D"
            xMessage := DToC( xMessage )
            EXIT

         CASE "L"
            xMessage := iif( xMessage, ".T.", ".F." )
            EXIT

         CASE "O"
            xMessage := xMessage:className + " Object"
            EXIT

         CASE "B"
            xMessage := "{||...}"
            EXIT

         DEFAULT
            xMessage := "NIL"
      END

      DO WHILE ( nPos := At( ';', xMessage ) ) != 0
         AAdd( aSay, Left( xMessage, nPos - 1 ) )
         xMessage := SubStr( xMessage, nPos + 1 )
      ENDDO
      AAdd( aSay, xMessage )

      FOR EACH xMessage IN aSay

         IF ( nLen := Len( xMessage ) ) > 58
            FOR nPos := 58 TO 1 STEP -1
               IF xMessage[nPos] IN ( " " + Chr( 9 ) )
                  EXIT
               ENDIF
            NEXT

            IF nPos == 0
               nPos := 58
            ENDIF

            sCopy := xMessage
            aSay[ HB_EnumIndex() ] := RTrim( Left( xMessage, nPos ) )

            IF Len( aSay ) == HB_EnumIndex()
               aAdd( aSay, SubStr( sCopy, nPos + 1 ) )
            ELSE
               aIns( aSay, HB_EnumIndex() + 1, SubStr( sCopy, nPos + 1 ), .T. )
            ENDIF
        ENDIF
      NEXT

   ENDIF

#endif

   IF !ISARRAY( aOptions )
      aOptions := {}
   ENDIF

   IF !ISCHARACTER( cColorNorm ) .or. EMPTY( cColorNorm )
      cColorNorm := "W+/R" // first pair color (Box line and Text)
      cColorHigh := "W+/B" // second pair color (Options buttons)
   ELSE

      /* NOTE: Clipper Alert does not handle second color pair properly.
               If we inform the second color pair, xHarbour alert will consider it.
               if we not inform the second color pair, then xHarbour alert will behave
               like Clipper.  2004/Sep/16 - Eduardo Fernandes <modalsist> */

      cColor11 := cColor12 := cColor21 := cColor22 := ""

      cColorStr := alltrim( StrTran( cColorNorm," ","") )
      nCommaSep := At(",",cColorStr)

      if nCommaSep > 0 // exist more than one color pair.
         cColorPair1 := SubStr( cColorStr, 1, nCommaSep - 1 )
         cColorPair2 := SubStr( cColorStr, nCommaSep + 1 )
      else
         cColorPair1 := cColorStr
         cColorPair2 := ""
      endif

      nSlash := At("/",cColorPair1)

      if nSlash > 1
         cColor11 := SubStr( cColorPair1,1,nSlash-1)
         cColor12 := SubStr( cColorPair1,nSlash+1)
      else
         cColor11 := cColorPair1
         cColor12 := "R"
      endif

      if ColorValid(cColor11) .and. ColorValid(cColor12)

        cColorNorm := cColor11

        if !empty(cColor12)

            cColorNorm := cColor11+"/"+cColor12

        endif

      else
         cColor11 := "W+"
         cColor12 := "R"
         cColorNorm := cColor11+"/"+cColor12
      endif


      // if second color pair exist, then xHarbour alert will handle properly.
      if !empty( cColorPair2 )

         nSlash := At("/",cColorPair2)

         if nSlash > 1
            cColor21 := SubStr( cColorPair2,1,nSlash-1)
            cColor22 := SubStr( cColorPair2,nSlash+1)
         else
            cColor21 := cColorPair2
            cColor22 := "B"
         endif

         if ColorValid(cColor21) .and. ColorValid(cColor22)


            cColorHigh := cColor21

            if !empty(cColor22)

                // extracting color attributes from background color.
                cColor22 := StrTran( cColor22, "+", "" )
                cColor22 := StrTran( cColor22, "*", "" )
                cColorHigh := cColor21+"/"+cColor22

            endif

         else
            cColorHigh := "W+/B"
         endif

      else // if does not exist the second color pair, xHarbour alert will behave like Clipper
         if empty(cColor11) .or. empty(cColor12)
            cColor11 := "B"
            cColor12 := "W+"
         else
            cColor11 := StrTran( cColor11, "+", "" )
            cColor11 := StrTran( cColor11, "*", "" )
         endif
         cColorHigh := cColor12+"/"+cColor11
      endif

   ENDIF

   IF nDelay == NIL
      nDelay := 0
   ENDIF

   /* The longest line */
   nWidth := 0
   AEval( aSay, {| x | nWidth := Max( Len( x ), nWidth ) } )

   /* Cleanup the button array */
   aOptionsOK := {}
   FOR EACH cEval IN aOptions
      IF ISCHARACTER( cEval ) .AND. !Empty( cEval )
         AAdd( aOptionsOK, cEval )
      ENDIF
   NEXT

   IF Len( aOptionsOK ) == 0
      aOptionsOK := { 'Ok' }
#ifdef HB_C52_STRICT
   /* NOTE: Clipper allows only four options [vszakats] */
   ELSEIF Len( aOptionsOK ) > 4
      aSize( aOptionsOK, 4 )
#endif
   ENDIF

   /* Total width of the botton line (the one with choices) */
   nOpWidth := 0
   AEval( aOptionsOK, {| x | nOpWidth += Len( x ) + 4 } )

   /* what's wider ? */
   //nWidth := Max( nWidth + 2 + iif( Len( aSay ) == 1, 4, 0 ), nOpWidth + 2 )
   nWidth := Max( nWidth + iif( Len( aSay ) == 1, 4, 0 ), nOpWidth ) + iif(!lCtWin,2,iif(nWidth<5,1,2))

   /* box coordinates */
   if !lCtWin
      nInitRow := Int( ( ( MaxRow() - ( Len( aSay ) + 4 ) ) / 2 ) + .5 )
      nInitCol := Int( ( ( MaxCol() - ( nWidth + 2 ) ) / 2 ) + .5 )
   else
      nInitRow := 0
      nInitCol := 0
   endif

   /* detect prompts positions */
   aPos := {}
   aHotkey := {}
   nCurrent := nInitCol + Int( ( nWidth - nOpWidth ) / 2 ) + iif(!lCtWin,2,1)
   AEval( aOptionsOK, {| x | AAdd( aPos, nCurrent ), AAdd( aHotKey, Upper( Left( x, 1 ) ) ), nCurrent += Len( x ) + 4 } )

   nChoice := 1

   IF lConsole

      nCount := Len( aSay )
      FOR EACH cEval IN aSay
         OutStd( cEval )
         IF HB_EnumIndex() < nCount
            OutStd( hb_OSNewLine() )
         ENDIF
      NEXT

      OutStd( " (" )
      nCount := Len( aOptionsOK )
      FOR EACH cEval IN aOptionsOK
         OutStd( cEval )
         IF HB_EnumIndex() < nCount
            OutStd( ", " )
         ENDIF
      NEXT
      OutStd( ") " )

      /* choice loop */
      lWhile := .T.
      DO WHILE lWhile

         nKey := Inkey( nDelay, INKEY_ALL )

         SWITCH nKey
            CASE 0
               lWhile := .F.
               EXIT

            CASE K_ESC

               nChoice := 0
               lWhile  := .F.
               EXIT

            DEFAULT
               IF Upper( Chr( nKey ) ) IN aHotkey
                  nChoice := aScan( aHotkey, {| x | x == Upper( Chr( nKey ) ) } )
                  lWhile  := .F.
               ENDIF

         END

      ENDDO

      OutStd( Chr( nKey ) )

   ELSE

      /* PreExt */
      nCount := nOldDispCount := DispCount()

      DO WHILE nCount-- != 0
         DispEnd()
      ENDDO

      /* save status */
      nOldRow := Row()
      nOldCol := Col()
      nOldCursor := SetCursor( SC_NONE )


      /* draw box */
      IF !lCTWin
         cOldScreen := SaveScreen( nInitRow, nInitCol, nInitRow + Len( aSay ) + 3, nInitCol + nWidth + 1 )
         DispBox( nInitRow, nInitCol, nInitRow + Len( aSay ) + 3, nInitCol + nWidth + 1, B_SINGLE + ' ', cColorNorm )
      ELSE
         cCtColor := SetColor( cColorNorm )
         Ct_WOpen( nInitRow, nInitCol, nInitRow + Len( aSay ) + 3, nInitCol + nWidth + 1 , .T. )
         Ct_WBox( B_SINGLE )
      ENDIF


      FOR EACH cEval IN aSay

          nCol := nInitCol + iif(!lCtWin,1,0) + Int( ( ( nWidth - Len( cEval ) ) / 2 ) + .5 )

          if !lCtWin
             DispOutAt( nInitRow + HB_EnumIndex(), nCol , cEval, cColorNorm )
          else
             SetPos( nInitRow + HB_EnumIndex() - 1 , nCol )
             QQOut( cEval  )
          endif

      NEXT

      /* choice loop */
      lWhile := .T.

      DO WHILE lWhile

         nCount := Len( aSay )
         FOR EACH cEval IN aOptionsOK
            if !lCtWin
               DispOutAt( nInitRow + nCount + 2, aPos[ HB_EnumIndex() ], " " + cEval + " ", cColorNorm )
            else
               SetPos( nInitRow + nCount + 2, aPos[ HB_EnumIndex() ] )
               QQOut( " " + cEval + " "  )
            endif
         NEXT

         if !lCtWin
            DispOutAt( nInitRow + nCount + 2, aPos[ nChoice ], " " + aOptionsOK[ nChoice ] + " ", cColorHigh, TRUE )
         else
            SetColor( cColorHigh )
            SetPos( nInitRow + nCount + 2, aPos[ nChoice ] )
            QQOut( " " + aOptionsOK[ nChoice ] + " "  )
            SetColor( cColorNorm )
            Ct_WCenter(.t.)
         endif

         nKey := Inkey( nDelay, INKEY_ALL )

         SWITCH nKey
            CASE K_ENTER
            CASE K_SPACE
            CASE 0
               lWhile := .F.
               EXIT

            CASE K_ESC
               nChoice := 0
               lWhile  := .F.
               EXIT

#ifdef HB_COMPAT_C53

            CASE K_LBUTTONDOWN

               nMRow  := MRow()
               nMCol  := MCol()
               nPos   := 0
               nCount := Len( aSay )

               FOR EACH cEval IN aOptionsOK
                  IF nMRow == nInitRow + nCount + 2 .AND. ;
                       INRANGE( aPos[ HB_EnumIndex() ], nMCol, aPos[ HB_EnumIndex() ] + Len( cEval ) + 2 - 1 )
                     nPos := HB_EnumIndex()
                     EXIT
                  ENDIF
               NEXT

               IF nPos > 0
                  nChoice := nPos
                  lWhile := .F.
               ENDIF

               EXIT

#endif

            CASE K_LEFT
            CASE K_SH_TAB
               IF Len( aOptionsOK ) > 1

                  nChoice--
                  IF nChoice == 0
                     nChoice := Len( aOptionsOK )
                  ENDIF

                  nDelay := 0
               ENDIF
               EXIT

            CASE K_RIGHT
            CASE K_TAB
               IF Len( aOptionsOK ) > 1

                  nChoice++
                  IF nChoice > Len( aOptionsOK )
                     nChoice := 1
                  ENDIF

                  nDelay := 0
               ENDIF
               EXIT

            DEFAULT
               IF Upper( Chr( nKey ) ) IN aHotkey

                  nChoice := aScan( aHotkey, {| x | x == Upper( Chr( nKey ) ) } )
                  lWhile  := .F.
               ENDIF

         END

      ENDDO


      /* Restore status */
      IF !lCtWin
         RestScreen( nInitRow, nInitCol, nInitRow + Len( aSay ) + 3, nInitCol + nWidth + 1, cOldScreen )
      ELSE
         Ct_WClose()
         SetColor( cCtColor )
      ENDIF

      SetCursor( nOldCursor )
      SetPos( nOldRow, nOldCol )

      /* PostExt */
      DO WHILE nOldDispCount-- != 0
         DispBegin()
      ENDDO

   ENDIF

   RETURN nChoice

#ifdef HB_C52_UNDOC

PROCEDURE __NONOALERT()

   s_lNoAlert := .F.

   RETURN

#endif


//-----------------------------------//
// 2004/Setp/15 - Eduardo Fernandes
// Test vality of the color string
STATIC FUNCTION COLORVALID( cColor )

if !IsCharacter( cColor )
   Return .F.
endif

cColor := StrTran( cColor, " ","" )
cColor := StrTran( cColor, "*","" )
cColor := StrTran( cColor, "+","" )
cColor := Upper( cColor )

Return cColor IN { "0","1","2","3","4","5","6","7","8","9","10","11","12",;
                   "13","14","15","B","BG","G","GR","N","R","RB","W"}

/* 2007/JAN/10 - E.F. - The functions below were copied from source\ct\ctwin.c
                        to avoid calling ct lib functions. At this way, the
                        alert function is free of ct.lib linking by users if used
                        only for default console mode, instead ct windows mode.
                        Therefore, these functions can be obsolete if anyone 
                        create other way to use these functions directly from a
                        gt driver or anything else.
*/

#pragma BEGINDUMP

#include "hbapi.h"
#include "hbapigt.h"


HB_FUNC_STATIC( CT_WNUM ) /* Get the highest windows handle */
{
   hb_retni( hb_ctWNum() );
}

HB_FUNC_STATIC( CT_WOPEN )
{
   SHORT iwnd;

   iwnd = hb_ctWOpen( hb_parni( 1 ), hb_parni( 2 ), hb_parni( 3 ),
                      hb_parni( 4 ), hb_parl( 5 ) );

   hb_retni( iwnd );

}

HB_FUNC_STATIC( CT_WBOX )     /* Places a frame around the active window */
{
   char        * cBox, cBox2[ 10 ], c;
   HB_CT_WND   * wnd;
   int         i, j;
   static char * cWBOX[] = {
            _B_DOUBLE,        // 0  WB_DOUBLE_CLEAR
            _B_SINGLE,        // 1  WB_SINGLE_CLEAR
            _B_DOUBLE_SINGLE, // 2  WB_DOUBLE_SINGLE_CLEAR
            _B_SINGLE_DOUBLE, // 3  WB_SINGLE_DOUBLE_CLEAR

            _B_DOUBLE,        // 4  WB_DOUBLE
            _B_SINGLE,        // 5  WB_SINGLE
            _B_DOUBLE_SINGLE, // 6  WB_DOUBLE_SINGLE
            _B_SINGLE_DOUBLE, // 7  WB_SINGLE_DOUBLE

            "��������",       // 8  WB_HALF_FULL_CLEAR
            "��������",       // 9  WB_HALF_CLEAR
            "��������",       // 10 WB_FULL_HALF_CLEAR
            "��������",       // 11 WB_FULL_CLEAR

            "��������",       // 12 WB_HALF_FULL
            "��������",       // 13 WB_HALF
            "��������",       // 14 WB_FULL_HALF
            "��������" };     // 15 WB_FULL

   wnd = hb_ctWCurrent();

   if( ISCHAR( 1 ) )
   {
      i = 4;
      cBox = hb_parcx( 1 );
   }
   else if( ISNUM( 1 ) )
   {
      i = hb_parni( 1 );
      if( i < 0 || i > 15 ) i = 0;
      cBox = cWBOX[ i ];
   }
   else
   {
      i = 0;
      cBox = cWBOX[ i ];
   }

   c = ' ';
   for( j = 0; j < 9; j++ )
   {
      c = cBox[ j ];
      if( !c ) break;
      cBox2[ j ] = c;
   }
   for( ; j < 8; j++ ) cBox2[ j ] = c;

   if( ( i % 8 ) < 4 && j < 9 ) cBox2[ j++ ] = (char) hb_ctGetClearB();
   cBox2[ j ] = '\0';

   if( wnd->ULRow - wnd->UFRow <= 1 || wnd->ULCol - wnd->UFCol <= 1 )
   {
      hb_retni( wnd->NCur );
      return;
   }

   hb_gtBox( 0, 0, wnd->ULRow - wnd->UFRow, wnd->ULCol - wnd->UFCol,
             ( BYTE * ) cBox2 );

   if( wnd->NCur == 0 )
   {
      if( wnd->iRow < 1 ) wnd->iRow = 1;
      if( wnd->iCol < 1 ) wnd->iCol = 1;
      hb_gtSetPos( wnd->iRow, wnd->iCol );
   }
   else
   {
      hb_ctWFormat( 1, 1, 1, 1 );
      hb_gtSetPos( 0, 0 );
   }

   hb_retni( wnd->NCur );
}
/****************************************************************************/
HB_FUNC_STATIC( CT_WCENTER )  /* Returns a window to the visible area, or centers it */
{
   hb_retni( hb_ctWCenter( hb_parl( 1 ) ) );
}
/****************************************************************************/
HB_FUNC_STATIC( CT_WCLOSE ) /* Close the active window */
{
   hb_retni( hb_ctWClose() );
}

#pragma BEGINEND

