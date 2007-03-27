/*
 * $Id: hbdate.c,v 1.4 2007/03/26 15:25:17 ronpinkas Exp $
 */

/*
 * Harbour Project source code:
 * The Date conversion module
 *
 * Copyright 1999 Antonio Linares <alinares@fivetech.com>
 * www - http://www.harbour-project.org
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

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 1999-2001 Viktor Szakats <viktor.szakats@syenar.hu>
 *    hb_dateEncStr()
 *    hb_dateDecStr()
 *    hb_dateStrPut()
 *    hb_dateStrGet()
 *
 * Copyright 1999 David G. Holm <dholm@jsd-llc.com>
 *    hb_dateFormat()
 *
 * Copyright 1999 Jose Lalin <dezac@corevia.com>
 *    hb_dateDOW()
 *
 * Copyright 2007 Walter Negro <anegro@overnet.com.ar>
 *    Support DateTime
 *    hb_timeFormat()
 *    hb_datetimeFormat()
 *    hb_timeEncodeSec()
 *    hb_timeEncode()
 *    hb_timeDecodeSec()
 *    hb_timeDecode()
 *    hb_datetimeEncode()
 *    hb_datetimeDecode()
 *    hb_timeEncStr()
 *    hb_timeDecStr()
 *    hb_datetimeEncStr()
 *    hb_datetimeDecStr()
 *    hb_datetimePack()
 *    hb_datetimePackInSec()
 *    hb_datetimeUnpack()
 *    hb_comp_datetimeEncStr()
 *    hb_comp_datetimeDecStr()
 *    hb_comp_datetimeEncode()
 *
 * See doc/license.txt for licensing terms.
 *
 */

#define HB_OS_WIN_32_USED

#include <time.h>

#include "hbapi.h"
#include "hbdate.h"
#include "hbmath.h"
#include "hbcomp.h"

#ifdef HB_C52_STRICT
   #define HB_DATE_YEAR_LIMIT    2999
#else
   #define HB_DATE_YEAR_LIMIT    9999
#endif

#define HB_STR_DATE_BASE      1721060     /* 0000/01/01 */

HB_EXPORT LONG hb_dateEncode( int iYear, int iMonth, int iDay )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dateEncode(%d, %d, %d)", iYear, iMonth, iDay));

   /* Perform date validation */
   if( iYear >= 0 && iYear <= HB_DATE_YEAR_LIMIT &&
       iMonth >= 1 && iMonth <= 12 &&
       iDay >= 1 )
   {
      /* Month, year, and lower day limits are simple,
         but upper day limit is dependent upon month and leap year */
      static int auiDayLimit[ 12 ] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

      if( iDay <= auiDayLimit[ iMonth - 1 ] ||
          ( iDay == 29 && iMonth == 2 &&
            ( iYear & 3 ) == 0 && ( iYear % 100 != 0 || iYear % 400 == 0 ) ) )
      {
         int iFactor = ( iMonth < 3 ) ? -1 : 0;

         return ( ( LONG )( iFactor + 4800 + iYear ) * 1461 / 4 ) +
                ( ( LONG )( iMonth - 2 - ( iFactor * 12 ) ) * 367 ) / 12 -
                ( ( LONG )( ( iFactor + 4900 + iYear ) / 100 ) * 3 / 4 ) +
                ( LONG ) iDay - 32075;
      }
   }

   return 0;
}

HB_EXPORT void hb_dateDecode( LONG lJulian, int *piYear, int *piMonth, int *piDay )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dateDecode(%ld, %p, %p, %p)", lJulian, piYear, piMonth, piDay));

   if( lJulian >= HB_STR_DATE_BASE )
   {
      LONG U, V, W, X;

      lJulian += 68569;
      W = ( lJulian * 4 ) / 146097;
      lJulian -= ( ( 146097 * W ) + 3 ) / 4;
      X = 4000 * ( lJulian + 1 ) / 1461001;
      lJulian -= ( ( 1461 * X ) / 4 ) - 31;
      V = 80 * lJulian / 2447;
      U = V / 11;

      *piYear  = (int) ( X + U + ( W - 49 ) * 100 );
      *piMonth = (int) ( V + 2 - ( U * 12 ) );
      *piDay   = (int) ( lJulian - ( 2447 * V / 80 ) );
   }
   else
   {
      *piYear  =
      *piMonth =
      *piDay   = 0;
   }
}

HB_EXPORT void hb_dateStrPut( char * szDate, int iYear, int iMonth, int iDay )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dateStrPut(%p, %d, %d, %d)", szDate, iYear, iMonth, iDay));

   if( iYear >= 0 && iMonth > 0 && iDay > 0 )
   {
      szDate[ 0 ] = ( ( iYear / 1000 ) % 10 ) + '0';
      szDate[ 1 ] = ( ( iYear / 100 ) % 10 ) + '0';
      szDate[ 2 ] = ( ( iYear / 10 ) % 10 ) + '0';
      szDate[ 3 ] = ( iYear % 10 ) + '0';

      szDate[ 4 ] = ( iMonth / 10 ) + '0';
      szDate[ 5 ] = ( iMonth % 10 ) + '0';

      szDate[ 6 ] = ( iDay / 10 ) + '0';
      szDate[ 7 ] = ( iDay % 10 ) + '0';
   }
   else
   {
      memset( szDate, '0', 8 );
   }
}

HB_EXPORT void hb_dateStrGet( const char * szDate, int * piYear, int * piMonth, int * piDay )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dateStrGet(%s, %p, %p, %p)", szDate, piYear, piMonth, piDay));

   if( szDate && szDate[ 8 ] == '\0' )
   {
      /* Date string has correct length, so attempt to convert */
      *piYear  = ( ( USHORT ) ( szDate[ 0 ] - '0' ) * 1000 ) +
                 ( ( USHORT ) ( szDate[ 1 ] - '0' ) * 100 ) +
                 ( ( USHORT ) ( szDate[ 2 ] - '0' ) * 10 ) +
                   ( USHORT ) ( szDate[ 3 ] - '0' );
      *piMonth = ( ( szDate[ 4 ] - '0' ) * 10 ) + ( szDate[ 5 ] - '0' );
      *piDay   = ( ( szDate[ 6 ] - '0' ) * 10 ) + ( szDate[ 7 ] - '0' );
   }
   else
   {
      /* Date string missing or bad length, so force an empty date */
      *piYear  =
      *piMonth =
      *piDay   = 0;
   }
}

/* This function always closes the date with a zero byte, so it needs a
   9 character long buffer. */

HB_EXPORT char * hb_dateDecStr( char * szDate, LONG lJulian )
{
   int iYear, iMonth, iDay;

   HB_TRACE(HB_TR_DEBUG, ("hb_dateDecStr(%p, %ld)", szDate, lJulian));

   if( lJulian <= 0 )
   {
      memset( szDate, ' ', 8 );
   }
   else
   {
      hb_dateDecode( lJulian, &iYear, &iMonth, &iDay );
      hb_dateStrPut( szDate, iYear, iMonth, iDay );
   }
   szDate[ 8 ] = '\0';

   return szDate;
}

HB_EXPORT LONG hb_dateEncStr( const char * szDate )
{
   int  iYear, iMonth, iDay;

   HB_TRACE(HB_TR_DEBUG, ("hb_dateEncStr(%s)", szDate));

   hb_dateStrGet( szDate, &iYear, &iMonth, &iDay );

   return hb_dateEncode( iYear, iMonth, iDay );
}

/* NOTE: szFormattedDate must be an at least 11 chars wide buffer */

char HB_EXPORT * hb_dateFormat( const char * szDate, char * szFormattedDate, const char * szDateFormat )
{
   /*
    * NOTE: szFormattedDate must point to a buffer of at least 11 bytes.
    *       szDateFormat must point to a buffer holding the date format to use.
    */
   int format_count, digit_count, size;

   HB_TRACE(HB_TR_DEBUG, ("hb_dateFormat(%s, %p, %s)", szDate, szFormattedDate, szDateFormat));

   /*
    * Determine the maximum size of the formatted date string
    */
   size = strlen( szDateFormat );
   if( size > 10 ) size = 10;

   if( szDate && szFormattedDate && strlen( szDate ) == 8 ) /* A valid date is always 8 characters */
   {
      const char * szPtr;
      int digit;
      BOOL used_d, used_m, used_y;

      format_count = 0;
      used_d = used_m = used_y = FALSE;
      szPtr = szDateFormat;

      while( format_count < size )
      {
         digit = toupper( ( UCHAR ) *szPtr );
         szPtr++;
         digit_count = 1;
         while( toupper( ( UCHAR ) *szPtr ) == digit && format_count < size )
         {
            szPtr++;
            if( format_count + digit_count < size ) digit_count++;
         }
         switch( digit )
         {
            case 'D':
               switch( digit_count )
               {
                  case 4:
                     if( ! used_d && format_count < size )
                     {
/*                        szFormattedDate[ format_count++ ] = '0'; */
                        szFormattedDate[ format_count++ ] = szDate[ 6 ];
                        digit_count--;
                     }
                  case 3:
                     if( ! used_d && format_count < size )
                     {
/*                        szFormattedDate[ format_count++ ] = '0'; */
                        szFormattedDate[ format_count++ ] = szDate[ 6 ];
                        digit_count--;
                     }
                  case 2:
                     if( ! used_d && format_count < size )
                     {
                        szFormattedDate[ format_count++ ] = szDate[ 6 ];
                        digit_count--;
                     }
                  default:
                     if( ! used_d && format_count < size )
                     {
                        szFormattedDate[ format_count++ ] = szDate[ 7 ];
                        digit_count--;
                     }
                     while( digit_count-- > 0 && format_count < size ) szFormattedDate[ format_count++ ] = digit;
               }
               used_d = TRUE;
               break;

            case 'M':
               switch ( digit_count )
               {
                  case 4:
                     if( ! used_m && format_count < size )
                     {
                        /* szFormattedDate[ format_count++ ] = '0'; */
                        szFormattedDate[ format_count++ ] = szDate[ 4 ];
                        digit_count--;
                     }
                  case 3:
                     if( ! used_m && format_count < size )
                     {
                        /* szFormattedDate[ format_count++ ] = '0'; */
                        szFormattedDate[ format_count++ ] = szDate[ 4 ];
                        digit_count--;
                     }
                  case 2:
                     if( ! used_m && format_count < size )
                     {
                        szFormattedDate[ format_count++ ] = szDate[ 4 ];
                        digit_count--;
                     }
                  default:
                     if( ! used_m && format_count < size )
                     {
                        szFormattedDate[ format_count++ ] = szDate[ 5 ];
                        digit_count--;
                     }
                     while( digit_count-- > 0 && format_count < size ) szFormattedDate[ format_count++ ] = digit;
               }
               used_m = TRUE;
               break;

            case 'Y':
               switch( digit_count )
               {
                  case 4:
                     if( ! used_y && format_count < size )
                     {
                        szFormattedDate[ format_count++ ] = szDate[ 0 ];
                        digit_count--;
                     }

                  case 3:
                     if( ! used_y && format_count < size )
                     {
                        szFormattedDate[ format_count++ ] = szDate[ 1 ];
                        digit_count--;
                     }

                  case 2:
                     if( ! used_y && format_count < size )
                     {
                        szFormattedDate[ format_count++ ] = szDate[ 2 ];
                        digit_count--;
                     }

                  default:
                     if( ! used_y && format_count < size )
                     {
                        szFormattedDate[ format_count++ ] = szDate[ 3 ];
                        digit_count--;
                     }
                     while( digit_count-- > 0 && format_count < size ) szFormattedDate[ format_count++ ] = digit;
               }
               used_y = TRUE;
               break;

            default:
               while( digit_count-- > 0 && format_count < size ) szFormattedDate[ format_count++ ] = digit;
         }
      }
   }
   else
   {
      /* Not a valid date string, so return a blank date with separators */
      format_count = size; /* size is either 8 or 10 */
      strncpy( szFormattedDate, szDateFormat, size );

      for( digit_count = 0; digit_count < size; digit_count++ )
      {
         switch( szFormattedDate[ digit_count ] )
         {
            case 'D':
            case 'd':
            case 'M':
            case 'm':
            case 'Y':
            case 'y':
               szFormattedDate[ digit_count ] = ' ';
         }
      }
   }

   szFormattedDate[ format_count ] = '\0';

   return szFormattedDate;
}

HB_EXPORT int hb_dateJulianDOW( LONG lJulian )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dateJulianDOW(%ld)", lJulian));

   if( lJulian >= HB_STR_DATE_BASE )
      return ( int ) ( ( lJulian + 1 ) % 7 ) + 1;
   else
      return 0;
}

/* NOTE: szFormattedTime must be an at least 16 chars wide buffer. hh:mm:ss.ccc pm */

char HB_EXPORT * hb_timeFormat( const char * szTime, char * szFormattedTime, const char * szTimeFormat )
{
   /*
    * NOTE: szFormattedTime must point to a buffer of at least 16 bytes.
    *       szTimeFormat must point to a buffer holding the time format to use.
    */
   int format_count, digit_count, size;

   HB_TRACE(HB_TR_DEBUG, ("hb_timeFormat(%s, %p, %s)", szTime, szFormattedTime, szTimeFormat));

   /*
    * Determine the maximum size of the formatted time string
    */
   size = strlen( szTimeFormat );
   if( size > 15 ) size = 15;

   if( szTime && szFormattedTime && strlen( szTime ) == (7 + HB_DATETIMEDECIMALS) )
   {
      const char * szPtr;
      int digit, pos_pm = 0, pos_h = -1;
      BOOL used_h, used_m, used_s, used_c, used_pm;

      format_count = 0;
      used_h = used_m = used_s = used_c = used_pm = FALSE;
      szPtr = szTimeFormat;

      while( format_count < size )
      {
         digit = toupper( *szPtr );
         szPtr++;
         digit_count = 1;
         while( toupper( *szPtr ) == digit && format_count < size )
         {
            szPtr++;
            if( format_count + digit_count < size ) digit_count++;
         }
         switch( digit )
         {
            case 'H':
               switch( digit_count )
               {
                  case 2:
                     if( ! used_h && format_count < size )
                     {
                        if(pos_h==-1) pos_h = format_count;
                        szFormattedTime[ format_count++ ] = szTime[ 0 ];
                        digit_count--;
                     }
                  default:
                     if( ! used_h && format_count < size )
                     {
                        if(pos_h==-1) pos_h = format_count;
                        szFormattedTime[ format_count++ ] = szTime[ 1 ];
                        digit_count--;
                     }
                     while( digit_count-- > 0 && format_count < size ) szFormattedTime[ format_count++ ] = digit;
               }
               used_h = TRUE;
               break;

            case 'M':
               if( pos_pm && format_count-1 == pos_pm )
               {
                  used_pm = TRUE;
                  szFormattedTime[ format_count++ ] = 'M';
                  break;
               }
               switch ( digit_count )
               {
                  case 2:
                     if( ! used_m && format_count < size )
                     {
                        szFormattedTime[ format_count++ ] = szTime[ 2 ];
                        digit_count--;
                     }
                  default:
                     if( ! used_m && format_count < size )
                     {
                        szFormattedTime[ format_count++ ] = szTime[ 3 ];
                        digit_count--;
                     }
                     while( digit_count-- > 0 && format_count < size ) szFormattedTime[ format_count++ ] = digit;
               }
               used_m = TRUE;
               break;

            case 'S':
               switch( digit_count )
               {
                  case 2:
                     if( ! used_s && format_count < size )
                     {
                        szFormattedTime[ format_count++ ] = szTime[ 4 ];
                        digit_count--;
                     }

                  default:
                     if( ! used_s && format_count < size )
                     {
                        szFormattedTime[ format_count++ ] = szTime[ 5 ];
                        digit_count--;
                     }
                     while( digit_count-- > 0 && format_count < size ) szFormattedTime[ format_count++ ] = digit;
               }
               used_s = TRUE;
               break;

            case 'C':
            {
               int digit_c = 7;

               switch( digit_count )
               {
                  case 4:
                     if( ! used_c && format_count < size )
                     {
//                        szFormattedTime[ format_count++ ] = '0';
                        szFormattedTime[ format_count++ ] = szTime[ digit_c++ ];
                        digit_count--;
                     }
                  case 3:
                     if( ! used_c && format_count < size && digit_c - 9 < HB_DATETIMEDECIMALS )
                     {
//                        szFormattedTime[ format_count++ ] = '0';
                        szFormattedTime[ format_count++ ] = szTime[ digit_c++ ];
                        digit_count--;
                     }
                  case 2:
                     if( ! used_c && format_count < size && digit_c - 9 < HB_DATETIMEDECIMALS )
                     {
                        szFormattedTime[ format_count++ ] = szTime[ digit_c++ ];
                        digit_count--;
                     }
                  default:
                     if( ! used_c && format_count < size && digit_c - 9 < HB_DATETIMEDECIMALS )
                     {
                        szFormattedTime[ format_count++ ] = szTime[ digit_c ];
                        digit_count--;
                     }
                     while( digit_count-- > 0 && format_count < size ) szFormattedTime[ format_count++ ] = digit;
               }
               used_c = TRUE;
               break;
            }

            case 'P':
               szFormattedTime[ format_count ] = digit;
               pos_pm = format_count++;
               break;

            default:
               while( digit_count-- > 0 && format_count < size ) szFormattedTime[ format_count++ ] = digit;
         }
      }
      if( used_pm && pos_h >= 0 )
      {
         if( szFormattedTime[ pos_h ] == '2' || ( szFormattedTime[ pos_h ] == '1' && szFormattedTime[ pos_h + 1 ] > '1' ) )
         {
            szFormattedTime[ pos_h ]--;
            szFormattedTime[ pos_h + 1 ] -= (char) 2;
            szFormattedTime[ pos_pm ] = 'P';
         }
         else
         {
            szFormattedTime[ pos_pm ] = 'A';
         }
         szFormattedTime[ pos_pm + 1 ] = 'M';

         if( szFormattedTime[ pos_h ] == '0' && szFormattedTime[ pos_h + 1 ] == '0' )
         {
            szFormattedTime[ pos_h ] = '1';
            szFormattedTime[ pos_h + 1 ] = '2';
         }
      }
   }
   else
   {
      /* Not a valid time string, so return a 00:00:00 time with separators */
      format_count = size;
      strncpy( szFormattedTime, szTimeFormat, size );

      for( digit_count = 0; digit_count < size; digit_count++ )
      {
         switch( szFormattedTime[ digit_count ] )
         {
            case 'H':
            case 'h':
            case 'M':
            case 'm':
            case 'S':
            case 's':
            case 'C':
            case 'c':
               szFormattedTime[ digit_count ] = '0';
         }
      }
   }

   szFormattedTime[ format_count ] = '\0';

   return szFormattedTime;
}

/* NOTE: szFormattedDateTime must be an at least 26 chars wide buffer */

char HB_EXPORT * hb_datetimeFormat( const char * szDateTime, char * szFormattedDateTime, const char * szDateFormat, const char * szTimeFormat )
{
   int n;
   char szDate[9];

   if( szDateFormat )
   {
      memcpy( szDate, szDateTime, 8 );
      szDate[8] = '\0';
      hb_dateFormat( szDate, szFormattedDateTime, szDateFormat );
      n = strlen( szFormattedDateTime );
      szFormattedDateTime[ n ] = ' ';
   }
   else
   {
      n = -1;
   }
   hb_timeFormat( szDateTime+8 , szFormattedDateTime+n+1, szTimeFormat );
   return szFormattedDateTime;
}

HB_EXPORT int hb_dateDOW( int iYear, int iMonth, int iDay )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dateDOW(%d, %d, %d)", iYear, iMonth, iDay));

   if( iMonth < 3 )
   {
      iMonth += 13;
      iYear--;
   }
   else
      iMonth++;

   return ( iDay + 26 * iMonth / 10 +
            iYear + iYear / 4 - iYear / 100 + iYear / 400 + 6 ) % 7 + 1;
}

HB_EXPORT void hb_dateToday( int * piYear, int * piMonth, int * piDay )
{
#if defined(HB_OS_WIN_32)

   SYSTEMTIME st;
   GetLocalTime( &st );

   *piYear  = st.wYear;
   *piMonth = st.wMonth;
   *piDay   = st.wDay;

#else

   time_t t;
   struct tm * oTime;

   time( &t );
   oTime = localtime( &t );

   *piYear  = oTime->tm_year + 1900;
   *piMonth = oTime->tm_mon + 1;
   *piDay   = oTime->tm_mday;

#endif
}

/* NOTE: The passed buffer must be at least 9 chars long */

HB_EXPORT void hb_dateTimeStr( char * pszTime )
{
#if defined(HB_OS_WIN_32)
   {
      SYSTEMTIME st;
      GetLocalTime( &st );
      snprintf( pszTime, 9, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond );
   }
#else
   {
      time_t t;
      struct tm * oTime;

      time( &t );
      oTime = localtime( &t );
      snprintf( pszTime, 9, "%02d:%02d:%02d", oTime->tm_hour, oTime->tm_min, oTime->tm_sec );
   }
#endif
}

void HB_EXPORT hb_dateTime( int * piHour, int * piMinute, double * pdSeconds )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_dateTime(%p,%p,%p)", piHour, piMinute, pdSeconds ));

#if defined(HB_OS_WIN_32)
   {
      SYSTEMTIME st;
      GetLocalTime( &st );
      *piHour    = (int) st.wHour;
      *piMinute  = (int) st.wMinute;
      *pdSeconds = (double) st.wSecond;
   }
#else
   {
      time_t t;
      struct tm * oTime;

      time( &t );
      oTime = localtime( &t );
      *piHour    = (int) oTime->tm_hour;
      *piMinute  = (int) oTime->tm_min;
      *pdSeconds = (double) oTime->tm_sec;
   }
#endif
}

double HB_EXPORT hb_timeEncodeSec( int iHour, int iMinute, double dSeconds )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_timeEncode(%d, %d, %f)", iHour, iMinute, dSeconds));
   //printf( "hb_timeEncode(%d, %d, %f)", lHour, lMinute, dSeconds);

   if( iHour >= 0 && iHour <= 23 && iMinute >= 0 && iMinute <= 59 && dSeconds >= 0 && dSeconds < 60 )
   {
      return (double) (iHour * 3600 + iMinute * 60) + dSeconds;
   }

   return 0;
}

LONG HB_EXPORT hb_timeEncode( int iHour, int iMinute, double dSeconds )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_timeEncode(%d, %d, %f)", iHour, iMinute, dSeconds));
   //printf( "hb_timeEncode(%d, %d, %f)", lHour, lMinute, dSeconds);

   if( iHour >= 0 && iHour <= 23 && iMinute >= 0 && iMinute <= 59 && dSeconds >= 0 && dSeconds < 60 )
   {
      return (LONG) (iHour * 3600 * HB_DATETIMEINSEC) + (LONG) (iMinute * 60 * HB_DATETIMEINSEC) + (LONG) (dSeconds * HB_DATETIMEINSEC);
   }

   return 0;
}

void HB_EXPORT hb_timeDecode( LONG lTime, int * piHour, int * piMinute, double * pdSeconds )
{
   int iHour = 0, iMin = 0;
   double dSec = 0.0;

   HB_TRACE(HB_TR_DEBUG, ("hb_timeDecode(%d, %p, %p %p)", dTime, piHour, piMinute, pdSeconds));

   if( lTime > 0 )
   {
      div_t result = div( (int) lTime, (int) ( 3600 * HB_DATETIMEINSEC ) );
      iHour  = result.quot;
      lTime  = result.rem;
      result = div( (int) lTime, (int) ( 60 * HB_DATETIMEINSEC ) );
      iMin   = result.quot;
      lTime  = result.rem;
      dSec   = ( double ) lTime  / HB_DATETIMEINSEC;
      //TraceLog(NULL,"iHour=%d iMin=%d dSec=%f\n",iHour,iMin,dSec);
   }

   if( piHour )
   {
      *piHour = iHour;
   }

   if( piMinute )
   {
      *piMinute = iMin;
   }

   if( pdSeconds )
   {
      *pdSeconds = dSec;
   }
}

void HB_EXPORT hb_timeDecodeSec( double dTime, int * piHour, int * piMinute, double * pdSeconds )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_timeDecode(%d, %p, %p %p)", dTime, piHour, piMinute, pdSeconds));

   if( dTime > 0 )
   {
      LONG lTime = (LONG) (dTime * HB_DATETIMEINSEC);
      hb_timeDecodeSec( lTime, piHour, piMinute, pdSeconds );
   }
}

void HB_EXPORT hb_datetimeEncode( LONG *plDate, LONG *plTime, int iYear, int iMonth, int iDay,
                                    int iHour, int iMinute, double dSeconds, int iAmPm, int * piOk )
{
   LONG lDate;
   BOOL iOk

   HB_TRACE(HB_TR_DEBUG, ("hb_datetimeEncode(%d, %d, %d, %d, %d, %f, %d, %p)", iYear, iMonth, iDay, iHour, iMinute, dSeconds, iAmPm, iOk));
   //printf( "hb_datetimeEncode(%d, %d, %d, %d, %d, %f, %d, %p) Que pasa???\n", iYear, iMonth, iDay, iHour, iMinute, dSeconds, iAmPm, iOk);

   lDate = ( LONG ) hb_dateEncode( iYear, iMonth, iDay );
   iOk   = FALSE;

   if( iAmPm == 0 )
   {
      iOk = TRUE;
   }
   else if( iAmPm == 1 )
   {
      if( iHour <= 12 )
      {
         iOk = TRUE;
         iHour %= 12;
      }
      else
      {
         iHour = 0;
         iMinute = 0;
         dSeconds = 0;
      }
   }
   else if( iAmPm == 2 )
   {
      if( iHour <= 12 )
      {
         iOk = TRUE;
         iHour %= 12;
         iHour += 12;
      }
      else
      {
         iHour = 0;
         iMinute = 0;
         dSeconds = 0;
      }
   }

   if( lDate && iHour >= 0 && iHour <= 23 && iMinute >= 0 && iMinute <= 59 && dSeconds >= 0 && dSeconds < 60 )
   {
      if( plDate )
      {
         *plDate = lDate;
      }
      if( plTime )
      {
         //printf( "iHour=%d, iMinute=%d, dSeconds=%f)\n", iHour, iMinute, dSeconds);
         *plTime = hb_timeEncode( iHour, iMinute, dSeconds );
      }
   }
   if( piOk )
   {
      *piOk = iOk;
   }

   return;
}

void HB_EXPORT hb_datetimeDecode( LONG lDate, LONG lTime, int * piYear, int * piMonth, int * piDay,
                                                    int * piHour, int * piMinute, double * pdSeconds )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_datetimeDecode( %d, %d %p, %p, %p, %p, %p, %p)", lDate, lTime, piYear, piMonth, piDay, piHour, piMinute, pdSeconds));

   hb_dateDecode( lDate, piYear, piMonth, piDay );
   hb_timeDecode( lTime, piHour, piMinute, pdSeconds );
}

LONG HB_EXPORT hb_timeEncStr( const char * szTime )
{
   LONG lTime;

   HB_TRACE(HB_TR_DEBUG, ("hb_timeEncStr(%s)", szTime));

   if( szTime )
   {
      ULONG ulLen = strlen( szTime );

      if( ulLen >= 4 )
      {
         lTime  = (LONG) hb_strVal( szTime, 2 ) * 3600 * HB_DATETIMEINSEC +
                  (LONG) hb_strVal( szTime + 2, 2 ) * 60 * HB_DATETIMEINSEC +
                  (LONG)(hb_strVal( szTime + 4, ulLen - 4 ) * HB_DATETIMEINSEC);
      }
   }
   else
   {
      lTime = 0;
   }

   return lTime;
}

char HB_EXPORT * hb_timeDecStr( char * szTime, LONG lSeconds )
{
   int iHour, iMinute;
   double dSeconds;

   HB_TRACE(HB_TR_DEBUG, ("hb_timeDecStr(%s,%f)", szTime,dSeconds));

   hb_timeDecode( lSeconds, &iHour, &iMinute, &dSeconds );

   //printf( "Origen %lf, Hora: %d, Minutos: %d, Segundos: %f\n", dSeconds, iHour, iMinute, dSeconds );
   snprintf( szTime, 8 + HB_DATETIMEDECIMALS, "%02d%02d%0*.*f", iHour, iMinute, HB_DATETIMEDECIMALS + 3, HB_DATETIMEDECIMALS, dSeconds );
   //printf( "Final %lf, Hora: %d, Minutos: %d, Segundos: %f, %s\n", dSeconds, iHour, iMinute, dSeconds, szTime );

   return szTime;
}

void HB_EXPORT hb_datetimeEncStr( const char * szDateTime, LONG *plDate, LONG *plTime )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_datetimeEncStr(%s,%p,%p)", szDateTime,plDate,plTime));

   if( plDate )
   {
      char szDate[9];
      szDate[8] = '\0';
      memcpy( szDate, szDateTime, 8 );
      *plDate = hb_dateEncStr( szDate );
   }
   if( plTime )
   {
      *plTime = hb_timeEncStr( szDateTime+8 );
   }
}

char HB_EXPORT * hb_datetimeDecStr( char * szDateTime, LONG lDate, LONG lTime )
{
   HB_TRACE(HB_TR_DEBUG, ("hb_datetimeDecStr(%s,%d,%d)", szDateTime, lDate, lTime));

   hb_dateDecStr( szDateTime, lDate );
   hb_timeDecStr( szDateTime+8, lTime );

   return szDateTime;
}

#undef hb_datetimePack
double HB_EXPORT hb_datetimePack( LONG lJulian, LONG lTime )
{
   return (double) lJulian + ((double)lTime / (double) (86400 * HB_DATETIMEINSEC));
}

#undef hb_datetimePackInSec
double HB_EXPORT hb_datetimePackInSec( LONG lJulian, LONG lTime )
{
   return (double) (lJulian * 86400 ) + ((double)lTime / (double) (HB_DATETIMEINSEC));
}

void hb_datetimeUnpack( double dDateTime, LONG * plDate, LONG * plTime )
{
   double dDate, dTime;
   dTime = modf( dDateTime, &dDate );

   if( plDate )
   {
      *plDate = (LONG)dDate;
   }
   if( plTime )
   {
      dTime  *= (double)(86400 * HB_DATETIMEINSEC);
      *plTime = (LONG)dTime;
   }
}

#define hb_datetimePack( lJulian, lTime )   (double)((double) lJulian + ((double)lTime / (double) (86400 * HB_DATETIMEINSEC)))
#define hb_datetimePackInSec( lJulian, lTime )   (double)((double) (lJulian * 86400 ) + ((double)lTime / (double) (HB_DATETIMEINSEC)))

/*-------------------------------------------------------------------------------*/

double hb_comp_datetimeEncStr( const char * szDateTime )
{
   LONG lDate, lTime;

   hb_datetimeEncStr( szDateTime, &lDate, &lTime );

   return hb_datetimePack( lDate, lTime );
}

char * hb_comp_datetimeDecStr( char * szDateTime, double dDateTime )
{
   LONG lDate, lTime;

   hb_datetimeUnpack( dDateTime, &lDate, &lTime );

   return hb_datetimeDecStr( szDateTime, lDate, lTime );
}

void hb_comp_datetimeEncode( LONG *plDate, LONG *plTime, int iYear, int iMonth, int iDay,
                              int iHour, int iMinute, double dSeconds, int iAmPm, int * piOk )
{
   hb_datetimeEncode( plDate, plTime, iYear, iMonth, iDay, iHour, iMinute, dSeconds, iAmPm, piOk );
}

/*-------------------------------------------------------------------------------*/