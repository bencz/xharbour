/* $CATEGORY$SQLRDD/Parser$FILES$sql.lib$
* SQLPARSER
* SQL Parser Actions
* Copyright (c) 2003 - Marcelo Lombardo  <lombardo@uol.com.br>
* All Rights Reserved
*/

#include "hbsql.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "hbapi.h"
#include "hbdate.h"
#include "hbapiitm.h"
#include "hbfast.h"
#include "hbapierr.h"
#include "hashapi.h"

#include "sqly.h"          /* Bison-generated include */
#include "msg.ch"
#include "sqlrdd.h"
#include "sqlrddsetup.ch"

#define MAX_FIELD_NAME_LEN    63

/* Prototypes */

int SqlParse( sql_stmt* stmt, const char* query, int queryLen );
int sql_yyparse( void* stmt );

/* PRG Level Functions */

HB_FUNC( SR_SQLPARSE )     /* SqlParse( cCommand, @nError, @nErrorPos ) */
{
   ULONG uLenPhrase = hb_parclen( 1 );
   sql_stmt * stmt;
   const char * sqlPhrase;
   const char * sqlIniPos;

   if( uLenPhrase )
   {
      stmt   = (sql_stmt *) hb_xgrab( sizeof( sql_stmt ) );

      stmt->numParam = 0;
      stmt->pTemp = NULL;

      sqlPhrase = hb_parc( 1 );
      sqlIniPos = sqlPhrase;

      if (SqlParse( stmt, sqlPhrase, PARSE_ALL_QUERY ))
      {
         // printf("Parse OK. Retornado array de %i posicoes.\n", stmt->pArray->item.asArray.value->ulLen );
      }
      else
      {
         stmt->pArray = hb_itemArrayNew(0);
         // printf("Parse ERROR. Retornado array de %i posicoes.\n", stmt->pArray->item.asArray.value->ulLen );

         if ( ISBYREF( 2 ) ) {
            hb_itemPutNI( (PHB_ITEM) hb_param( 2, HB_IT_ANY ), stmt->errMsg );
         }

         if ( ISBYREF( 3 ) ) {
            hb_itemPutNI( (PHB_ITEM) hb_param( 3, HB_IT_ANY ), (int) ( stmt->queryPtr - sqlIniPos ) );
        }
      }
   }
   hb_itemRelease( hb_itemReturnForward( stmt->pArray ) );
   hb_xfree( stmt );
}

/*
*
* Parser Entry Point
*
*/

int SqlParse( sql_stmt* stmt, const char* query, int queryLen )
{
   if (!query) {
      stmt->errMsg = SQL_PARSER_ERROR_PARSE;
      stmt->errPtr = "";
      return 0;
   }
   if (!queryLen) {
      queryLen = strlen( query )+1;
   }

   stmt->query = (char *) query;
   stmt->queryLen = queryLen;
   stmt->queryPtr = stmt->errPtr = (char *) query;
   stmt->errMsg = 0;

   if( sql_yyparse( (void*) stmt ) || stmt->errMsg || stmt->command == -1 )
   {
      // printf( "parse error in sql_yyparse\n" );
      if (!stmt->errMsg)
      {
         stmt->errMsg = SQL_PARSER_ERROR_PARSE;
      }
      return 0;
   }

   return 1;
}

/*
*
* pCode Generation and handling
*
*/

PHB_ITEM SQLpCodeGenInt( int code )
{
   HB_ITEM intItem;
   PHB_ITEM pArray;

   pArray = hb_itemArrayNew(0);
   intItem.type = HB_IT_INTEGER;
   intItem.item.asInteger.value  = (int)code;
   intItem.item.asInteger.length = 6;
   hb_arrayAddForward( pArray, &intItem );

   return pArray;
}

PHB_ITEM SQLpCodeGenIntItem( int code, PHB_ITEM value )
{
   HB_ITEM intItem;
   PHB_ITEM pArray;

   pArray = hb_itemArrayNew(0);

   intItem.type = HB_IT_INTEGER;
   intItem.item.asInteger.value  = (int)code;
   intItem.item.asInteger.length = 6;

   hb_arrayAddForward( pArray, &intItem );
   hb_arrayAddForward( pArray, value );

   hb_itemRelease( value );

   return pArray;
}

PHB_ITEM SQLpCodeGenItemInt( PHB_ITEM value, int code )
{
   HB_ITEM intItem;
   PHB_ITEM pArray;

   pArray = hb_itemArrayNew(0);

   intItem.type = HB_IT_INTEGER;
   intItem.item.asInteger.value  = (int)code;
   intItem.item.asInteger.length = 6;

   hb_arrayAddForward( pArray, value );
   hb_arrayAddForward( pArray, &intItem );

   return pArray;
}

PHB_ITEM SQLpCodeGenIntItem2( int code, PHB_ITEM value, int code2, PHB_ITEM value2 )
{
   HB_ITEM intItem;
   HB_ITEM intItem2;
   PHB_ITEM pArray;

   pArray = hb_itemArrayNew(0);

   intItem.type = HB_IT_INTEGER;
   intItem.item.asInteger.value  = (int)code;
   intItem.item.asInteger.length = 6;

   intItem2.type = HB_IT_INTEGER;
   intItem2.item.asInteger.value  = (int)code2;
   intItem2.item.asInteger.length = 6;

   hb_arrayAddForward( pArray, &intItem );
   hb_arrayAddForward( pArray, value );
   hb_arrayAddForward( pArray, &intItem2 );
   hb_arrayAddForward( pArray, value2 );

   hb_itemRelease( value );
   hb_itemRelease( value2 );

   return pArray;
}

PHB_ITEM SQLpCodeGenArrayJoin( PHB_ITEM pArray1, PHB_ITEM pArray2 )
{
   USHORT n;
   PHB_ITEM pToCopy;

   if ( !HB_IS_ARRAY(pArray1) )
   {
      printf( "SQLpCodeGenArrayJoin Invalid param 1\n" );
   }

   if ( !HB_IS_ARRAY(pArray2) )
   {
      printf( "SQLpCodeGenArrayJoin Invalid param 2\n" );
   }

   for (n=1;n <= (USHORT) pArray2->item.asArray.value->ulLen; n++)
   {
      pToCopy = hb_itemArrayGet( pArray2, n );
      hb_arrayAddForward( pArray1, pToCopy);
      hb_itemRelease( pToCopy );
   }
   hb_itemRelease( pArray2 );
   return pArray1;
}

PHB_ITEM SQLpCodeGenArrayItem( PHB_ITEM pArray, PHB_ITEM value )
{
   hb_arrayAddForward( pArray, value );
   hb_itemRelease( value );
   return pArray;
}

PHB_ITEM SQLpCodeGenArrayInt( PHB_ITEM pArray, int code )
{
   HB_ITEM intItem;

   intItem.type = HB_IT_INTEGER;
   intItem.item.asInteger.value  = (int)code;
   intItem.item.asInteger.length = 6;

   hb_arrayAddForward( pArray, &intItem );

   return pArray;
}

PHB_ITEM SQLpCodeGenArrayIntInt( PHB_ITEM pArray, int code, int code2 )
{
   HB_ITEM intItem;
   HB_ITEM intItem2;

   intItem.type = HB_IT_INTEGER;
   intItem.item.asInteger.value  = (int)code;
   intItem.item.asInteger.length = 6;

   intItem2.type = HB_IT_INTEGER;
   intItem2.item.asInteger.value  = (int)code2;
   intItem2.item.asInteger.length = 6;

   hb_arrayAddForward( pArray, &intItem );
   hb_arrayAddForward( pArray, &intItem2 );

   return pArray;
}

PHB_ITEM SQLpCodeGenIntArray( int code, PHB_ITEM pArray )
{
   HB_ITEM intItem;
   PHB_ITEM newItem;

   intItem.type = HB_IT_INTEGER;
   intItem.item.asInteger.value  = (int)code;
   intItem.item.asInteger.length = 6;

   newItem = hb_itemNew(NULL);

   hb_arrayAdd( pArray, newItem );
   hb_arrayIns( pArray, 1 );
   hb_arraySetForward( pArray, 1, &intItem );
   hb_itemRelease( newItem );

   return pArray;
}

HB_FUNC( SR_STRTOHEX )
{
   char *outbuff;
   const char *cStr;
   char *c;
   USHORT iNum;
   int i, len;
   int iCipher;

   if( ! ISCHAR(1) )
   {
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, "SR_STRTOHEX", 1, hb_param(1,HB_IT_ANY) );
      return;
   }

   cStr = hb_parc( 1 );
   len = (int) hb_parclen( 1 );
   outbuff = (char *) hb_xgrab( (len * 2) + 1 );
   c = outbuff;

   for( i = 0; i < len; i++ )
   {

      iNum = (int) cStr[i];
      c[0] = '0';
      c[1] = '0';

      iCipher = (int) (iNum % 16);

      if ( iCipher < 10 )
      {
         c[1] = '0' + iCipher;
      }
      else
      {
         c[1] = 'A' + (iCipher - 10 );
      }
      iNum >>=4;

      iCipher = iNum % 16;
      if ( iCipher < 10 )
      {
         c[0] = '0' + iCipher;
      }
      else
      {
         c[0] = 'A' + (iCipher - 10 );
      }

      c+=2;
   }

   outbuff[len*2] = '\0';
   hb_retc( outbuff );
   hb_xfree( outbuff );
}

char * sr_Hex2Str( char * cStr, int len, int * lenOut )
{
   char *outbuff;
   char c;
   int i, nalloc;
   int iCipher, iNum;

   nalloc = (int) (len/2);
   outbuff = (char *) hb_xgrab( nalloc + 1 );

   for( i = 0; i < nalloc; i++ )
   {
      // First byte

      c = *cStr;
      iNum = 0;
      iNum <<= 4;
      iCipher = 0;

      if ( c >= '0' && c <= '9' )
      {
         iCipher = (ULONG) ( c - '0' );
      }
      else if ( c >= 'A' && c <= 'F' )
      {
         iCipher = (ULONG) ( c - 'A' )+10;
      }
      else if ( c >= 'a' && c <= 'f' )
      {
         iCipher = (ULONG) ( c - 'a' )+10;
      }

      iNum += iCipher;
      cStr++;

      // Second byte

      c = *cStr;
      iNum <<= 4;
      iCipher = 0;

      if ( c >= '0' && c <= '9' )
      {
         iCipher = (ULONG) ( c - '0' );
      }
      else if ( c >= 'A' && c <= 'F' )
      {
         iCipher = (ULONG) ( c - 'A' )+10;
      }
      else if ( c >= 'a' && c <= 'f' )
      {
         iCipher = (ULONG) ( c - 'a' )+10;
      }

      iNum += iCipher;
      cStr++;
      outbuff[i] = iNum;
   }

   outbuff[nalloc] = '\0';

   * lenOut = nalloc;

   return( outbuff );
}

HB_FUNC( SR_HEXTOSTR )
{
   char *outbuff;
   int nalloc;

   if( ! ISCHAR(1) )
   {
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, "SR_HEXTOSTR", 1, hb_param(1,HB_IT_ANY) );
      return;
   }

   outbuff = sr_Hex2Str( (char *) hb_parc( 1 ), hb_parclen( 1 ), &nalloc );
   hb_retclenAdopt( outbuff, nalloc );
}

//---------------------------------------------------------------------------//

static ULONG escape_mysql( char *to, const char *from, ULONG length )
{
  const char *to_start=to;
  const char *end;
  for (end=from+length; from != end ; from++)
  {
    switch (*from)
    {
    case 0:            /* Must be escaped for 'mysql' */
      *to++= '\\';
      *to++= '0';
      break;
    case '\n':            /* Must be escaped for logs */
      *to++= '\\';
      *to++= 'n';
      break;
    case '\r':
      *to++= '\\';
      *to++= 'r';
      break;
    case '\\':
      *to++= '\\';
      *to++= '\\';
      break;
    case '\'':
      *to++= '\\';
      *to++= '\'';
      break;
    case '"':            /* Better safe than sorry */
      *to++= '\\';
      *to++= '"';
      break;
    case '\032':         /* This gives problems on Win32 */
      *to++= '\\';
      *to++= 'Z';
      break;
    default:
      *to++= *from;
    }
  }
  *to=0;
  return (ULONG) (to-to_start);
}

static ULONG escape_single( char *to, const char *from, ULONG length )
{
  const char *to_start=to;
  const char *end;
  for (end=from+length; from != end ; from++)
  {
    switch (*from)
    {
    case '\'':
      *to++= '\'';
      *to++= '\'';
      break;
    default:
      *to++= *from;
    }
  }
  *to=0;
  return (ULONG) (to-to_start);
}

static ULONG escape_firebird( char *to, const char *from, ULONG length )
{
  const char *to_start=to;
  const char *end;
  for (end=from+length; from != end ; from++)
  {
    switch (*from)
    {
    case '\'':
      *to++= '\'';
      *to++= '\'';
      break;
    case 0:
      *to++= ' ';
      break;
    default:
      *to++= *from;
    }
  }
  *to=0;
  return (ULONG) (to-to_start);
}

static ULONG escape_db2( char *to, const char *from, ULONG length )
{
  const char *to_start=to;
  const char *end;
  for (end=from+length; from != end ; from++)
  {
    switch (*from)
    {
    case '\'':
      *to++= '\'';
      *to++= '\'';
      break;
    case 0:
      *to++= ' ';
      break;
    default:
      *to++= *from;
    }
  }
  *to=0;
  return (ULONG) (to-to_start);
}

static ULONG escape_pgs( char *to, const char *from, ULONG length )
{
  const char *to_start=to;
  const char *end;
  for (end=from+length; from != end ; from++)
  {
    switch (*from)
    {
    case '\'':
      *to++= '\'';
      *to++= '\'';
      break;
    case '\\':
      *to++= '\\';
      *to++= '\\';
      break;
    case 0:
      break;
    default:
      *to++= *from;
    }
  }
  *to=0;
  return (ULONG) (to-to_start);
}

static ULONG escape_oci( char *to, const char *from, ULONG length )
{
  const char *to_start=to;
  const char *end;
  for (end=from+length; from != end ; from++)
  {
    switch (*from)
    {
    case '\'':
      *to++= '\'';
      *to++= '\'';
      break;
    case 0:
      break;

    default:
      *to++= *from;
    }

  }
  *to=0;
  return (ULONG) (to-to_start);
}

HB_FUNC( SR_ESCAPESTRING )
{
   const char *FromBuffer;
   int iSize;
   int idatabase;
   char *ToBuffer;

   iSize= hb_parclen(1);
   idatabase = hb_parni(2);

   if( ! (ISCHAR(1) && ISNUM(2)) )
   {
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, "SR_ESCAPESTRING", 2, hb_param(1,HB_IT_ANY), hb_param(2,HB_IT_ANY) );
      return;
   }

   if ( iSize )
   {
      FromBuffer = hb_parc( 1 );
      ToBuffer = ( char *) hb_xgrab( ( iSize*2 ) + 1 );
      if ( ToBuffer )
      {
         switch (idatabase)
         {
         case SYSTEMID_MYSQL:
            iSize = escape_mysql( ToBuffer, FromBuffer, iSize );
            break;
         case SYSTEMID_FIREBR:
            iSize = escape_firebird( ToBuffer, FromBuffer, iSize );
            break;
         case SYSTEMID_ORACLE:
         case SYSTEMID_CACHE:
            iSize = escape_oci( ToBuffer, FromBuffer, iSize );
            break;
         case SYSTEMID_MSSQL7:
         case SYSTEMID_INGRES:
         case SYSTEMID_SYBASE:
         case SYSTEMID_ADABAS:
         case SYSTEMID_INFORM:
         case SYSTEMID_OTERRO:
         case SYSTEMID_PERVASIVE:
            iSize = escape_single( ToBuffer, FromBuffer, iSize );
            break;
         case SYSTEMID_POSTGR:
            iSize = escape_pgs( ToBuffer, FromBuffer, iSize );
            break;
         case SYSTEMID_IBMDB2:
            iSize = escape_db2( ToBuffer, FromBuffer, iSize );
            break;
         }
      }
      hb_retclenAdopt( ( char *) ToBuffer, iSize );
   }
   else
   {
      hb_retc( "" );
   }
}

char * QuoteTrimEscapeString( char * FromBuffer, ULONG iSize, int idatabase, BOOL bRTrim, ULONG * iSizeOut )
{
   char * ToBuffer;

   ToBuffer = (char *) hb_xgrab( ( iSize*2 ) + 3 );

   ToBuffer[0] = '\'';
   ToBuffer++;

   switch (idatabase)
   {
   case SYSTEMID_MYSQL:
      iSize = escape_mysql( ToBuffer, FromBuffer, iSize );
      break;
   case SYSTEMID_FIREBR:
      iSize = escape_firebird( ToBuffer, FromBuffer, iSize );
      break;
   case SYSTEMID_ORACLE:
   case SYSTEMID_CACHE:
      iSize = escape_oci( ToBuffer, FromBuffer, iSize );
      break;
   case SYSTEMID_MSSQL7:
   case SYSTEMID_INGRES:
   case SYSTEMID_SYBASE:
   case SYSTEMID_ADABAS:
   case SYSTEMID_INFORM:
   case SYSTEMID_OTERRO:
   case SYSTEMID_PERVASIVE:
      iSize = escape_single( ToBuffer, FromBuffer, iSize );
      break;
   case SYSTEMID_POSTGR:
      iSize = escape_pgs( ToBuffer, FromBuffer, iSize );
      break;
   case SYSTEMID_IBMDB2:
      iSize = escape_db2( ToBuffer, FromBuffer, iSize );
      break;
   }

   iSize++;
   ToBuffer--;

   while (bRTrim && iSize > 1 && ToBuffer[iSize-1] == ' ')
   {
      iSize--;
   }

   ToBuffer[iSize] = '\'';
   iSize++;
   ToBuffer[iSize] = '\0';
   * iSizeOut = iSize;
   return ( ToBuffer );
}


HB_FUNC( SR_ESCAPENUM )
{
   const char *FromBuffer;
   char *ToBuffer;
   char SciNot[5] = {'\0','\0','\0','\0','\0'};
   int iSize, iPos, iDecPos;
   BOOL bInteger = TRUE;
   ULONG len, dec;
   double dMultpl;

   iSize= hb_parclen(1);
   FromBuffer = hb_parc( 1 );

   if( ! (ISCHAR(1) && ISNUM(2)  && ISNUM(3)) )
   {
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, "SR_ESCAPENUM", 3, hb_param(1,HB_IT_ANY), hb_param(2,HB_IT_ANY), hb_param(3,HB_IT_ANY) );
      return;
   }

   ToBuffer = ( char *) hb_xgrab( ( iSize ) + 33 );
   memset( ToBuffer, 0, ( iSize ) + 33 );

   len = hb_parnl( 2 );
   dec = hb_parnl( 3 );

   if ( dec > 0 )
   {
      len -= (dec + 1);
   }

   dMultpl = 0;
   iDecPos = 0;

   for( iPos=0;iPos<iSize;iPos++ )
   {
      if (FromBuffer[iPos] == ',')
      {
         ToBuffer[iPos] = '.';
         iDecPos = iPos;
      }
      else
      {
         ToBuffer[iPos] = FromBuffer[iPos];
      }

      if (ToBuffer[iPos] == '.')
      {
         bInteger = FALSE;
         iDecPos = iPos;
      }

      if (ToBuffer[iPos] == 'E' && (iPos+2) <= iSize)    // 1928773.3663E+003
      {
         bInteger = FALSE;
         if( FromBuffer[iPos+1] == '-' )
         {
            SciNot[0] = FromBuffer[iPos+1];
         }
         else
         {
            SciNot[0] = '0';
         }
         SciNot[1] = FromBuffer[iPos+2];

         if( (iPos+3) <= iSize )
         {
            SciNot[2] = FromBuffer[iPos+3];
            if( (iPos+4) <= iSize )
            {
               SciNot[3] = FromBuffer[iPos+4];
            }
         }

         iSize = iPos;
         dMultpl = hb_strVal( SciNot, 5 );

         break;
      }
   }

   // Moves the decimal point dMultpl positions

   ToBuffer[iSize] = '\0';

   if( dMultpl > 0 )
   {
      for( iPos=iDecPos;iPos<iSize+32;iPos++ )
      {
         if( ToBuffer[iPos] == '.' && dMultpl > 0 && (iPos+1) <= iSize+32 )
         {
            ToBuffer[iPos]   = ToBuffer[iPos+1];
            ToBuffer[iPos+1] = '.';
            dMultpl--;
            if( ToBuffer[iPos] == '\0' || iPos > iSize )
            {
               ToBuffer[iPos] = '0';
            }
            if( dMultpl == 0 && ToBuffer[iPos+2] == '\0' )
            {
               ToBuffer[iPos+1] = '\0';
               break;
            }
         }
      }
      iSize = strlen( ToBuffer );
   }
   else if( dMultpl < 0 )
   {
      // Not implemented
   }

   if( bInteger )
   {
#ifndef HB_LONG_LONG_OFF
      LONGLONG lValue;
#else
      LONG lValue;
#endif
      int iOverflow;
      lValue = hb_strValInt( ToBuffer, &iOverflow );

      if( !iOverflow )
      {
         double dValue = (double) lValue;
         hb_retnlen( dValue, len, dec );
      }
      else
      {
         double dValue = hb_strVal( ToBuffer, iSize );
         hb_retnlen( dValue, len, dec );
      }
   }
   else
   {
      double dValue = hb_strVal( ToBuffer, iSize );
      hb_retnlen( dValue, len, dec );
   }
   hb_xfree( ToBuffer );
}

PHB_ITEM sr_escapeNumber( char *FromBuffer, ULONG len, ULONG dec, PHB_ITEM pRet )
{
   char *ToBuffer;
   char SciNot[5] = {'\0','\0','\0','\0','\0'};
   int iSize, iPos, iDecPos;
   BOOL bInteger = TRUE;
   double dMultpl;

   iSize    = strlen( FromBuffer );
   ToBuffer = ( char *) hb_xgrab( ( iSize ) + 33 );
   memset( ToBuffer, 0, ( iSize ) + 33 );

   if ( dec > 0 )
   {
      len -= (dec + 1);
   }

   dMultpl = 0;
   iDecPos = 0;

   for( iPos=0;iPos<iSize;iPos++ )
   {
      if (FromBuffer[iPos] == ',')
      {
         ToBuffer[iPos] = '.';
         iDecPos = iPos;
      }
      else
      {
         ToBuffer[iPos] = FromBuffer[iPos];
      }

      if (ToBuffer[iPos] == '.')
      {
         bInteger = FALSE;
         iDecPos = iPos;
      }

      if (ToBuffer[iPos] == 'E' && (iPos+2) <= iSize)    // 1928773.3663E+003
      {
         bInteger = FALSE;
         if( FromBuffer[iPos+1] == '-' )
         {
            SciNot[0] = FromBuffer[iPos+1];
         }
         else
         {
            SciNot[0] = '0';
         }
         SciNot[1] = FromBuffer[iPos+2];

         if( (iPos+3) <= iSize )
         {
            SciNot[2] = FromBuffer[iPos+3];
            if( (iPos+4) <= iSize )
            {
               SciNot[3] = FromBuffer[iPos+4];
            }
         }

         iSize = iPos;
         dMultpl = hb_strVal( SciNot, 5 );

         break;
      }
   }

   // Moves the decimal point dMultpl positions

   ToBuffer[iSize] = '\0';

   if( dMultpl > 0 )
   {
      for( iPos=iDecPos;iPos<iSize+32;iPos++ )
      {
         if( ToBuffer[iPos] == '.' && dMultpl > 0 && (iPos+1) <= iSize+32 )
         {
            ToBuffer[iPos]   = ToBuffer[iPos+1];
            ToBuffer[iPos+1] = '.';
            dMultpl--;
            if( ToBuffer[iPos] == '\0' || iPos > iSize )
            {
               ToBuffer[iPos] = '0';
            }
            if( dMultpl == 0 && ToBuffer[iPos+2] == '\0' )
            {
               ToBuffer[iPos+1] = '\0';
               break;
            }
         }
      }
      iSize = strlen( ToBuffer );
   }
   else if( dMultpl < 0 )
   {
      // Not implemented
   }

   if( bInteger )
   {
#ifndef HB_LONG_LONG_OFF
      LONGLONG lValue;
#else
      LONG lValue;
#endif
      int iOverflow;
      lValue = hb_strValInt( ToBuffer, &iOverflow );

      if( !iOverflow )
      {
         double dValue = (double) lValue;
         hb_itemPutNLen( pRet, dValue, len, dec );
      }
      else
      {
         double dValue = hb_strVal( ToBuffer, iSize );
         hb_itemPutNLen( pRet, dValue, len, dec );
      }
   }
   else
   {
      double dValue = hb_strVal( ToBuffer, iSize );
      hb_itemPutNLen( pRet, dValue, len, dec );
   }
   hb_xfree( ToBuffer );
   return( pRet );
}

HB_FUNC( SR_DBQUALIFY )
{
   PHB_ITEM pText = hb_param( 1, HB_IT_STRING );
   int ulDb = hb_parni(2);

   if( pText )
   {
      char * szOut;
      char * pszBuffer;
      ULONG ulLen, i;

      pszBuffer = pText->item.asString.value;
      ulLen = pText->item.asString.length;
      szOut = (char * ) hb_xgrab( ulLen + 3 );

      // Firebird, DB2, ADABAS and Oracle must be uppercase
      // Postgres, MySQL and Ingres must be lowercase
      // Others, doesn't matter column case

      switch (ulDb)
      {
      case SYSTEMID_ORACLE:
      case SYSTEMID_FIREBR:
      case SYSTEMID_IBMDB2:
      case SYSTEMID_ADABAS:
         szOut[0] = '"';
         for( i = 0; i < ulLen; i++ )
         {
            szOut[ i+1 ] = toupper( (BYTE) pszBuffer[ i ] );
         }
         szOut[i+1] = '"';
         break;
      case SYSTEMID_INGRES:
      case SYSTEMID_POSTGR:
         szOut[0] = '"';
         for( i = 0; i < ulLen; i++ )
         {
            szOut[ i+1 ] = tolower( (BYTE) pszBuffer[ i ] );
         }
         szOut[i+1] = '"';
         break;
      case SYSTEMID_MSSQL7:
         szOut[0] = '[';
         for( i = 0; i < ulLen; i++ )
         {
            szOut[ i+1 ] = (BYTE) pszBuffer[ i ];
         }
         szOut[i+1] = ']';
         break;
      case SYSTEMID_MYSQL:
      case SYSTEMID_OTERRO:
         szOut[0] = '`';
         for( i = 0; i < ulLen; i++ )
         {
            szOut[ i+1 ] = tolower( (BYTE) pszBuffer[ i ] );
         }
         szOut[i+1] = '`';
         break;
      case SYSTEMID_INFORM:
         for( i = 0; i < ulLen; i++ )
         {
            szOut[ i ] = tolower( (BYTE) pszBuffer[ i ] );
         }
         ulLen -=2;
         break;
      default:
         szOut[0] = '"';
         for( i = 0; i < ulLen; i++ )
         {
            szOut[ i+1 ] = (BYTE) pszBuffer[ i ];
         }
         szOut[i+1] = '"';
      }
      hb_retclenAdopt( szOut, ulLen + 2 );
   }
   else
   {
      hb_errRT_BASE_SubstR( EG_ARG, 1102, NULL, "SR_DBQUALIFY", 1, hb_paramError( 1 ) );
   }
}

/*------------------------------------------------------------------------*/

#ifdef SQLRDD_COMPAT_PRE_1_1

BOOL hb_arraySetNL( PHB_ITEM pArray, ULONG ulIndex, LONG lVal )
{
   BOOL ret;
   PHB_ITEM pItem = hb_errNew();
   hb_itemPutNL( pItem, lVal );
   ret = hb_arraySetForward( pArray, ulIndex, pItem );
   hb_itemRelease( pItem );
   return( ret );
}

/*------------------------------------------------------------------------*/

BOOL hb_arraySetL( PHB_ITEM pArray, ULONG ulIndex, BOOL lVal )
{
   BOOL ret;
   PHB_ITEM pItem = hb_errNew();
   hb_itemPutL( pItem, lVal );
   ret = hb_arraySetForward( pArray, ulIndex, pItem );
   hb_itemRelease( pItem );
   return( ret );
}

#endif

//-----------------------------------------------------------------------------//

BOOL SR_itemEmpty( PHB_ITEM pItem )
{
   switch( hb_itemType( pItem ) )
   {
      case HB_IT_ARRAY:
         return( pItem->item.asArray.value->ulLen == 0 );

      case HB_IT_STRING:
      case HB_IT_MEMO:
         return( hb_strEmpty( pItem->item.asString.value, pItem->item.asString.length ) );

      case HB_IT_INTEGER:
         return( pItem->item.asInteger.value == 0 );

      case HB_IT_LONG:
         return( pItem->item.asLong.value == 0l );

      case HB_IT_DOUBLE:
         return( pItem->item.asDouble.value == 0.0 );

      case HB_IT_DATE:
#ifdef SQLRDD_COMPAT_PRE_1_1
         return( pItem->item.asDate.value == 0 );
#else
         return( pItem->item.asDate.value == 0 && pItem->item.asDate.time == 0 );
#endif

      case HB_IT_POINTER:
         return( pItem->item.asPointer.value == NULL );

      case HB_IT_LOGICAL:
         return( ! pItem->item.asLogical.value );

      case HB_IT_BLOCK:
         return( FALSE );

      case HB_IT_HASH:
         return( pItem->item.asHash.value->ulTotalLen == 0 );

      default:
         return( TRUE );
   }
}

//-----------------------------------------------------------------------------//

char * quotedNull( PHB_ITEM pFieldData, PHB_ITEM pFieldLen, PHB_ITEM pFieldDec, BOOL bNullable, int nSystemID, BOOL bTCCompat, BOOL bMemo, BOOL * bNullArgument )
{
   char * sValue, sDate[9];
   ULONG iSizeOut;
   int iTrim, iPos, iSize;
   sValue = NULL;

   * bNullArgument = FALSE;

   if( SR_itemEmpty( pFieldData ) && (!(    HB_IS_ARRAY( pFieldData )
                                         || HB_IS_OBJECT( pFieldData )
                                         || HB_IS_HASH( pFieldData ) ))
                                  && (      ( (nSystemID == SYSTEMID_POSTGR) && HB_IS_DATE( pFieldData ) )
                                         || ( (nSystemID != SYSTEMID_POSTGR) && ( !HB_IS_LOGICAL( pFieldData ) ) ) ) )
   {
      if( bNullable || HB_IS_DATE( pFieldData ) )
      {
         sValue = (char *) hb_xgrab( 5 );
         sValue[0] = 'N';
         sValue[1] = 'U';
         sValue[2] = 'L';
         sValue[3] = 'L';
         sValue[4] = '\0';
         * bNullArgument = TRUE;

         return (sValue);
      }
      else
      {
         if( HB_IS_STRING( pFieldData ) && bTCCompat )
         {
            sValue = QuoteTrimEscapeString( pFieldData->item.asString.value,
                                            pFieldData->item.asString.length,
                                            nSystemID, FALSE, &iSizeOut );
            return (sValue);
         }
         else if( HB_IS_STRING( pFieldData ) )
         {
            sValue = (char *) hb_xgrab( 4 );
            sValue[0] = '\'';
            sValue[1] = ' ';
            sValue[2] = '\'';
            sValue[3] = '\0';
            return (sValue);
         }
         else if( HB_IS_NUMBER( pFieldData ) )
         {
            sValue = (char *) hb_xgrab( 2 );
            sValue[0] = '0';
            sValue[1] = '\0';
            return (sValue);
         }
      }
   }

   if( HB_IS_STRING( pFieldData ) )
   {
      sValue = QuoteTrimEscapeString( pFieldData->item.asString.value,
                                      pFieldData->item.asString.length,
                                      nSystemID, !bTCCompat, &iSizeOut );
   }
   else if( HB_IS_NUMBER( pFieldData ) )
   {
      sValue = hb_itemStr( pFieldData, pFieldLen, pFieldDec );
      iTrim = 0;
      iSize = 15;
      while ( sValue[iTrim] == ' ' )
      {
         iTrim++;
      }
      if( iTrim > 0 )
      {
         for ( iPos = 0; iPos + iTrim < iSize; iPos++ )
         {
            sValue[iPos] = sValue[iPos + iTrim];
         }
         sValue[iPos] = '\0';
      }
   }
   else if( HB_IS_DATE( pFieldData ) )
   {
      hb_dateDecStr( sDate, pFieldData->item.asDate.value );
      sValue = (char *) hb_xgrab( 30 );
      switch ( nSystemID )
      {
         case SYSTEMID_ORACLE:
         {
            if (!bMemo)
            {
               sprintf( sValue, "TO_DATE(\'%s\',\'YYYYMMDD\')", sDate );
               return (sValue);
            }
         }
         default:
         {
            if (!bMemo)
            {
               sprintf( sValue, "\'%s\'", sDate );
               return (sValue);
            }
         }
      }
   }
   else if( HB_IS_LOGICAL( pFieldData ) )
   {
      sValue = (char *) hb_xgrab( 6 );
      if( hb_itemGetL( pFieldData ) )
      {
         if( nSystemID == SYSTEMID_POSTGR )
         {
            sValue[0] = 't';
            sValue[1] = 'r';
            sValue[2] = 'u';
            sValue[3] = 'e';
            sValue[4] = '\0';
         }
         else if( nSystemID == SYSTEMID_INFORM )
         {
            sValue[0] = '\'';
            sValue[1] = 't';
            sValue[2] = '\'';
            sValue[3] = '\0';
         }
         else
         {
            sValue[0] = '1';
            sValue[1] = '\0';
         }
      }
      else
      {
         if( nSystemID == SYSTEMID_POSTGR )
         {
            sValue[0] = 'f';
            sValue[1] = 'a';
            sValue[2] = 'l';
            sValue[3] = 's';
            sValue[4] = 'e';
            sValue[5] = '\0';
         }
         else if( nSystemID == SYSTEMID_INFORM )
         {
            sValue[0] = '\'';
            sValue[1] = 'f';
            sValue[2] = '\'';
            sValue[3] = '\0';
         }
         else
         {
            sValue[0] = '0';
            sValue[1] = '\0';
         }
      }
   }
   return( sValue );
}

