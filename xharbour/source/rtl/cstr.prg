/*
 * $Id: cstr.prg,v 1.37 2007/04/25 01:37:11 ronpinkas Exp $
 */

/*
 * xHarbour Project source code:
 * CStr( xAnyType ) -> String
 *
 * Copyright 2001 Ron Pinkas <ron@@ronpinkas.com>
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
 * As a special exception, xHarbour license gives permission for
 * additional uses of the text contained in its release of xHarbour.
 *
 * The exception is that, if you link the xHarbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the xHarbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released with this xHarbour
 * explicit exception.  If you add/copy code from other sources,
 * as the General Public License permits, the above exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for xHarbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

#include "error.ch"

/*
   For performance NOT using OS indpendant R/T function,
   this define only used in ValTpPrg() which currently only used in win32.
 */
#undef CRLF
#define CRLF Chr(13) + Chr(10)

REQUEST HB_SaveBlock, HB_RestoreBlock, __ClsInstName

//--------------------------------------------------------------//
FUNCTION CStr( xExp )

   LOCAL cType

   IF xExp == NIL
      RETURN 'NIL'
   ENDIF

   cType := ValType( xExp )

   SWITCH cType
      CASE 'C'
         RETURN xExp

      CASE 'D'
         RETURN dToS( xExp )

      CASE 'L'
         RETURN IIF( xExp, '.T.', '.F.' )

      CASE 'N'
         RETURN Str( xExp )

      CASE 'M'
         RETURN xExp

      CASE 'A'
         RETURN "{ Array of " +  LTrim( Str( Len( xExp ) ) ) + " Items }"

      CASE 'B'
         RETURN '{|| Block }'

      CASE 'O'
         RETURN "{ " + xExp:ClassName() + " Object }"

      CASE 'P'
         RETURN NumToHex( xExp )

      CASE 'H'
         RETURN "{ Hash of " +  LTrim( Str( Len( xExp ) ) ) + " Items }"

      DEFAULT
         RETURN "Type: " + cType
   END

RETURN ""

//--------------------------------------------------------------//
FUNCTION CStrToVal( cExp, cType )

   IF ValType( cExp ) != 'C'
      Throw( ErrorNew( "CSTR", 0, 3101, ProcName(), "Argument error", { cExp, cType } ) )
   ENDIF

   SWITCH cType
      CASE 'C'
         RETURN cExp

      CASE 'P'
         RETURN HexToNum( cExp )

      CASE 'D'
         IF cExp[3] >= '0' .AND. cExp[3] <= '9' .AND. cExp[5] >= '0' .AND. cExp[5] <= '9'
            RETURN sToD( cExp )
         ELSE
            RETURN cToD( cExp )
         ENDIF

      CASE 'L'
         RETURN IIF( cExp[1] == 'T' .OR. cExp[1] == 'Y' .OR. cExp[2] == 'T' .OR. cExp[2] == 'Y', .T., .F. )

      CASE 'N'
         RETURN Val( cExp )

      CASE 'M'
         RETURN cExp

      CASE 'U'
         RETURN NIL

      /*
      CASE 'A'
         Throw( ErrorNew( "CSTR", 0, 3101, ProcName(), "Argument error", { cExp, cType } ) )

      CASE 'B'
         Throw( ErrorNew( "CSTR", 0, 3101, ProcName(), "Argument error", { cExp, cType } ) )

      CASE 'O'
         Throw( ErrorNew( "CSTR", 0, 3101, ProcName(), "Argument error", { cExp, cType } ) )
      */

      DEFAULT
         Throw( ErrorNew( "CSTR", 0, 3101, ProcName(), "Argument error", { cExp, cType } ) )
   END

RETURN NIL

//--------------------------------------------------------------//
FUNCTION StringToLiteral( cString )

   LOCAL lDouble := .F., lSingle := .F.

   IF cString HAS "\n|\r" .OR. ( ( lDouble := '"' IN cString ) .AND. ( lSingle := "'" IN cString ) .AND. cString HAS "\[|\]" )

      cString := StrTran( cString, '"', '\"' )
      cString := StrTran( cString, Chr(10), '\n' )
      cString := StrTran( cString, Chr(13), '\r' )

      //TraceLog( cString )

      RETURN 'E"' + cString + '"'
   ELSEIF lDouble == .F.
      RETURN '"' + cString + '"'
   ELSEIF lSingle == .F.
      RETURN "'" + cString + "'"
   ENDIF

RETURN "[" + cString + "]"

//--------------------------------------------------------------//
FUNCTION ValToPrg( xVal, cName, nPad, aObjs )

   LOCAL cType := ValType( xVal )
   LOCAL aVar, cRet, cPad, nObj

   //TraceLog( xVal, cName, nPad, aObjs )

   SWITCH cType
      CASE 'C'
         RETURN StringToLiteral( xVal )

      CASE 'D'
         RETURN "sToD( '" + dToS( xVal ) + "' )"

      CASE 'L'
         RETURN IIF( xVal, ".T.", ".F." )

      CASE 'N'
         RETURN Str( xVal )

      CASE 'M'
         RETURN StringToLiteral( xVal )

      CASE 'A'
         IF cName == NIL
            nPad := 0
            cName := "M->__ValToPrg_Array"
            aObjs := {}
            cRet  := cName + " := "
         ELSE
            IF ( nObj := aScan( aObjs, {|a| HB_ArrayID( a[1] ) == HB_ArrayID( xVal ) } ) ) > 0
                RETURN aObjs[ nObj ][2] + " /* Cyclic */"
            ENDIF

            cRet := ""
         ENDIF

         aAdd( aObjs, { xVal, cName } )

         cRet  += "Array(" + LTrim( Str( Len( xVal ) ) ) + ")" + CRLF

         nPad += 3
         cPad  := sPace( nPad )

         FOR EACH aVar IN xVal
            cRet += cPad + cName + "[" + LTrim( Str( HB_EnumIndex() ) ) + "] := " + ValToPrg( aVar, cName + "[" + LTrim( Str( HB_EnumIndex() ) ) + "]", nPad, aObjs ) + CRLF
         NEXT

         nPad -=3

         RETURN cRet

      CASE 'H'
         IF Len( xVal ) == 0
            RETURN "Hash()"
         ELSE
            cRet := "{ "

            FOR EACH aVar IN xVal:Keys
               cRet += ValToPrg( aVar ) + " => "
               cRet += ValToPrg( xVal:Values[ HB_EnumIndex() ] ) + ", "
            NEXT

            /* We know for sure xVal isn't empty, and a last ',' is here */
            cRet[ -2 ] := ' '
            cRet[ -1 ] := '}'

            RETURN cRet
         ENDIF

      CASE 'B'
         RETURN ValToPrgExp( xVal )

      CASE 'P'
         RETURN "0x" + NumToHex( xVal )

      CASE 'O'
         /* TODO: Use HBPersistent() when avialable! */
         IF cName == NIL
            cName := "M->__ValToPrg_Object"
            nPad := 0
            aObjs := {}
            cRet  := cName + " := "
         ELSE
            IF ( nObj := aScan( aObjs, {|a| HB_ArrayID( a[1] ) == HB_ArrayID( xVal ) } ) ) > 0
                RETURN aObjs[ nObj ][2] + " /* Cyclic */"
            ENDIF

            cRet := ""
         ENDIF

         aAdd( aObjs, { xVal, cName } )

         cRet += xVal:ClassName + "():New()" + CRLF

         nPad += 3
         cPad  := sPace( nPad )

         FOR EACH aVar IN __objGetValueDiff( xVal )
            cRet += cPad + cName + ":" + aVar[1] + " := " + ValToPrg( aVar[2], cName + ":" + aVar[1], nPad, aObjs ) + CRLF
         NEXT

         nPad -=3
         RETURN cRet

      DEFAULT
         //TraceLog( xVal, cName, nPad )
         IF xVal == NIL
            cRet := "NIL"
         ELSE
            Throw( ErrorNew( "VALTOPRG", 0, 3103, ProcName(), "Unsupported type", { xVal } ) )
         ENDIF
   END

   //TraceLog( cRet )

RETURN cRet

//--------------------------------------------------------------//
FUNCTION ValToPrgExp( xVal, cName, aObjs, lBin )

   LOCAL cType := ValType( xVal )
   LOCAL aVar, cRet, nObj
   LOCAL cChar

   //TraceLog( xVal, cName, aObjs, lBin )

   SWITCH cType
      CASE 'C'
         IF Len( xVal ) == 0
            RETURN '""'
         ELSEIF lBin == .T.
            cRet := ""

            FOR EACH cChar IN xVal
               cRet += " + Chr(" + Str( Asc( cChar ), 3 ) + ")"
            NEXT

            RETURN SubStr( cRet, 3 )
         ELSE
            RETURN StringToLiteral( xVal )
         ENDIF
      CASE 'D'
         RETURN "sToD( '" + dToS( xVal ) + "' )"

      CASE 'L'
         RETURN IIF( xVal, ".T.", ".F." )

      CASE 'N'
         RETURN Ltrim( Str( xVal ) )

      CASE 'M'
         RETURN xVal

      CASE 'A'
         IF cName == NIL
            cName := "M->__ValToPrgExp_Array"
            aObjs := {}
         ELSE
            IF ( nObj := aScan( aObjs, {|a| HB_ArrayID( a[1] ) == HB_ArrayID( xVal ) } ) ) > 0
                RETURN  aObjs[ nObj ][2]
            ENDIF
         ENDIF

         aAdd( aObjs, { xVal, cName } )

         cRet  := "( " + cName + " := Array(" + LTrim( Str( Len( xVal ) ) ) + "), "

         FOR EACH aVar IN xVal
            cRet += cName + "[" + LTrim( Str( HB_EnumIndex() ) ) + "] := " + ValToPrgExp( aVar, cName + "[" + LTrim( Str( HB_EnumIndex() ) ) + "]", aObjs, lBin ) + ", "
         NEXT

         RETURN cRet + cName + " )"

      CASE 'H'
         IF Len( xVal ) == 0
            RETURN "Hash()"
         ELSE
            cRet := "{ "

            FOR EACH aVar IN xVal:Keys
               cRet += ValToPrgExp( aVar, cName, aObjs, lBin ) + " => "
               cRet += ValToPrgExp( xVal:Values[ HB_EnumIndex() ], cName, aObjs, lBin ) + ", "
            NEXT

            /* We know for sure xVal isn't empty, and a last ',' is here */
            cRet[ -2 ] := ' '
            cRet[ -1 ] := '}'

            RETURN cRet
         ENDIF

      CASE 'B'
         TRY
            cRet := "HB_RestoreBlock( "  + ValToPrgExp( HB_SaveBlock( xVal ), cName, aObjs, .T. ) + " )"
         CATCH
            cRet := "{|| Alert( 'Non Persistentable Block.' ) }"
         END

         RETURN cRet

      CASE 'P'
         RETURN "HexToNum('" + NumToHex( xVal ) + "')"

      CASE 'O'
         /* TODO: Use HBPersistent() when avialable! */

         IF cName == NIL
            cName := "M->__ValToPrgExp_Object"
            aObjs := {}
         ELSE
            IF ( nObj := aScan( aObjs, {|a| HB_ArrayID( a[1] ) == HB_ArrayID( xVal ) } ) ) > 0
                RETURN aObjs[ nObj ][2]
            ENDIF
         ENDIF

         aAdd( aObjs, { xVal, cName } )

         cRet  := "( " + cName + " := __ClsInstName( '" +  xVal:ClassName  + "' ), "

         FOR EACH aVar IN __objGetValueDiff( xVal )
            cRet += cName + ":" + aVar[1] + " := " + ValToPrgExp( aVar[2], cName + ":" + aVar[1], aObjs, lBin ) + ", "
         NEXT

         RETURN cRet + cName + " )"

      DEFAULT
         //TraceLog( xVal, cName, nPad )
         IF xVal == NIL
            cRet := "NIL"
         ELSE
            Throw( ErrorNew( "VALTOPRG", 0, 3103, ProcName(), "Unsupported type", { xVal } ) )
         ENDIF
   END

//   TraceLog( cRet )

RETURN cRet

//--------------------------------------------------------------//
FUNCTION PrgExpToVal( cExp )
RETURN &( cExp )

//--------------------------------------------------------------//
