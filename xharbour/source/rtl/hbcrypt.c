/*
 * $Id: hbcrypt.c,v 1.8 2003/07/15 09:15:34 andijahja Exp $
 */

/*
 * xHarbour Project source code:
 * Cryptography for xharbour
 *
 * Copyright 2003 Giancarlo Niccolai <giancarlo@niccolai.ws>
 * www - http://www.xharbour.org
 * SEE ALSO COPYRIGHT NOTICE FOR NXS BELOW.
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

/***************************************************************
* NXS aglorithm is FREE SOFTWARE. It can be reused for any
* purpose, provided that this copiright notice is still present
* in the software.
*
* This program is distributed WITHOUT ANY WARRANTY that it can
* fit any particular need.
*
* NXS author is Giancarlo Niccolai <giancarlo@niccolai.ws>
*
* Adler 32 CRC is copyrighted by Martin Adler (see note below)
**************************************************************/


#include "hbapi.h"
#include "hbapiitm.h"
#include "hbfast.h"
#include "hbstack.h"
#include "hbdefs.h"
#include "hbvm.h"
#include "hbapierr.h"
#include "hbnxs.h"
#include "hbcompress.h"

#define BASE 65521L /* largest prime smaller than 65536 */
ULONG adler32( ULONG adler, const BYTE *buf, UINT len);


/* Giancarlo Niccolai's x scrambler algorithm
* Prerequisites:
* 1) source must be at least 8 bytes long.
* 2) key must be at least 6 characters long.
*    Optimal lenght is about 12 to 16 bytes. Maximum keylen is 512 bytes
* 3) cipher must be preallocated with srclen bytes
*/

void nxs_crypt(
   const unsigned char *source, unsigned long srclen,
   const unsigned char *key, unsigned long keylen,
   unsigned char *cipher )
{

   if(keylen > NXS_MAX_KEYLEN )
   {
      keylen = NXS_MAX_KEYLEN;
   }

   // debug
   //memcpy( cipher, source, srclen );
   // end debug

   /* pass one: scramble the source using the key */
   nxs_scramble( source, srclen, key, keylen, cipher );

   /* pass two: xor the source with the key
      threebit mutual shift is done also here */
   nxs_xorcode( cipher, srclen, key, keylen );

   /* pass three: xor the source with the cyclic key */
   nxs_xorcyclic( cipher, srclen, key, keylen );
}

/*decrypting the buffer */

void nxs_decrypt(
   const unsigned char *cipher, unsigned long cipherlen,
   const unsigned char *key, unsigned long keylen,
   unsigned char *result )
{
   if(keylen > NXS_MAX_KEYLEN )
   {
      keylen = NXS_MAX_KEYLEN;
   }

   memcpy( result, cipher, cipherlen );

   /* pass one: xor the source with the cyclic key */
   nxs_xorcyclic( result, cipherlen, key, keylen );

   /* pass two: xor the source with the key
      threebit mutual shift is done also here */
   nxs_xordecode( result, cipherlen, key, keylen );

   /* pass three: unscramble the source using the key */
   nxs_unscramble( result, cipherlen, key, keylen );
}

/* This function scrambles the source using the letter ordering in the
* key. */
void nxs_scramble(
      const unsigned char *source, unsigned long srclen,
      const unsigned char *key, unsigned long keylen,
      unsigned char *cipher )
{
   int scramble[ NXS_MAX_KEYLEN ];
   unsigned long len;

   if ( keylen > NXS_MAX_KEYLEN )
   {
      keylen = NXS_MAX_KEYLEN;
   }

   if ( keylen > srclen )
   {
      keylen = srclen;
   }

   /* First step: find key ordering */
   nxs_make_scramble( scramble, key, keylen );

   /* Leave alone the last block */
   len = (srclen / keylen) * keylen;
   nxs_partial_scramble( source, cipher, scramble, len, keylen );

   // last pos was not done.
   //memcpy( cipher +len , source + len , srclen - len );

   keylen = srclen - len;
   nxs_make_scramble( scramble, key, keylen );
   nxs_partial_scramble( source + len, cipher + len, scramble, keylen, keylen );
}


void nxs_partial_scramble(
   const unsigned char *source, unsigned char *cipher,
   int *scramble,
   unsigned long len, unsigned long keylen )
{
   unsigned long pos;
   unsigned short kpos;

   pos = 0l;
   kpos = 0;
   while( pos + kpos < len )
   {
      cipher[ pos + scramble[ kpos ] ] = source[ pos + kpos ];
      kpos++;
      if ( kpos >= (unsigned short) keylen )
      {
         kpos = 0;
         pos += keylen;
      }
   }
}

/* Reversing scramble process */
void nxs_unscramble(
      unsigned char *cipher, unsigned long cipherlen,
      const unsigned char *key, unsigned long keylen)
{
   int scramble[ NXS_MAX_KEYLEN ];
   unsigned long len;

   if ( keylen > NXS_MAX_KEYLEN )
   {
      keylen = NXS_MAX_KEYLEN;
   }

   if ( keylen > cipherlen )
   {
      keylen = cipherlen;
   }

   /* First step: find key ordering */
   nxs_make_scramble( scramble, key, keylen );

   /* Leave alone the last block */
   len = (cipherlen / keylen) * keylen;
   nxs_partial_unscramble( cipher, scramble, len , keylen );

   keylen = cipherlen - len;
   nxs_make_scramble( scramble, key, keylen );
   nxs_partial_unscramble( cipher+len, scramble, keylen, keylen );
}


void nxs_partial_unscramble(
   unsigned char *cipher,
   int *scramble,
   unsigned long len, unsigned long keylen )
{
   unsigned long pos;
   unsigned short kpos;
   unsigned char buf[ NXS_MAX_KEYLEN ];

   pos = 0l;
   kpos = 0;
   while( pos + kpos < len )
   {
      buf[ kpos ] = cipher[ pos + scramble[ kpos ]  ];
      kpos++;
      if ( kpos >= (unsigned short) keylen )
      {
         memcpy( cipher + pos, buf, keylen );
         kpos = 0;
         pos += keylen;
      }
   }
}

/* pass two: xor the source with the key
   threebit mutual shift is done also here */
void nxs_xorcode(
   unsigned char *cipher, unsigned long cipherlen,
   const unsigned char *key, unsigned long keylen )
{
   unsigned long pos = 0l;
   unsigned short keypos = 0;
   unsigned char c_bitrest;

   c_bitrest = cipher[ 0 ] >>5;

   while ( pos < cipherlen )
   {
      cipher[pos] <<= 3;

      if (keypos == (unsigned short) keylen-1 || pos == cipherlen -1 )
      {
         cipher[pos] |= c_bitrest;
      }
      else
      {
         cipher[pos] |= cipher[pos+1] >> 5;
      }

      cipher[pos] ^= key[ keypos ];
      keypos ++;
      pos++;

      if (keypos == (unsigned short) keylen )
      {
         keypos = 0;
         c_bitrest = cipher[ pos ] >>5;
      }
   }
}

void nxs_xordecode(
   unsigned char *cipher, unsigned long cipherlen,
   const unsigned char *key, unsigned long keylen )
{
   unsigned long pos = 0l;
   unsigned short keypos = 0;
   unsigned char c_bitrest, c_bitleft;

   // A very short block?
   if ( keylen > cipherlen - pos )
   {
      keylen = ( USHORT ) ( cipherlen - pos);
   }
   c_bitleft = ( cipher[ keylen -1 ] ^ key[ keylen -1 ])<< 5;

   while ( pos < cipherlen )
   {
      cipher[pos] ^= key[ keypos ];

      c_bitrest = cipher[ pos ] <<5;
      cipher[pos] >>= 3;
      cipher[pos] |= c_bitleft;
      c_bitleft = c_bitrest;

      keypos ++;
      pos ++;

      if (keypos == (unsigned short) keylen )
      {
         keypos = 0;
         // last block
         if ( keylen > cipherlen - pos )
         {
            keylen = ( USHORT ) (cipherlen - pos);
         }

         c_bitleft = ( cipher[ pos + keylen -1 ] ^ key[ keylen -1 ])<< 5;
      }
   }
}

/* pass three: xor the source with the cyclic key */
void nxs_xorcyclic(
   unsigned char *cipher, unsigned long cipherlen,
   const unsigned char *key, unsigned long keylen )
{
   unsigned long pos=0l, crcpos=0l;
   unsigned long crc1, crc2, crc3;
   unsigned long crc1l, crc2l, crc3l;

   /* Build the cyclic key seed */
   crc1 = adler32( 0, key, keylen - 2 );
   crc2 = adler32( 0, key + 2 , keylen-4 );
   crc3 = adler32( 0, key + 1, keylen - 2 );

   crc1l = crc1 = nxs_cyclic_sequence( crc1 );
   crc2l = crc2 = nxs_cyclic_sequence( crc2 );
   crc3l = crc3 = nxs_cyclic_sequence( crc3 );

   while ( pos < cipherlen)
   {
      if ( crcpos < 4 )
      {
         /* this ensures portability across platforms */
         cipher[ pos ] ^= (unsigned char) (crc1l % 256 );
         crc1l /= 256l;
      }
      else if ( crcpos < 8 )
      {
         cipher[ pos ] ^= (unsigned char) (crc2l % 256 );
         crc2l /= 256l;
      }
      else {
         cipher[ pos ] ^= (unsigned char) (crc3l % 256 );
         crc3l /= 256l;
      }
      crcpos++;
      pos++;

      if (crcpos == 12 )
      {
         crcpos = 0;
         crc1l = crc1 = nxs_cyclic_sequence( crc1 );
         crc2l = crc2 = nxs_cyclic_sequence( crc2 );
         crc3l = crc3 = nxs_cyclic_sequence( crc3 );
      }
   }
}

unsigned long nxs_cyclic_sequence( unsigned long input )
{
   unsigned long first = input & 0xffff;
   unsigned long second = input >> 16;
   unsigned long ret = ( ( second * BASE * BASE ) & 0xffff ) |
         ( (first * BASE * BASE) &0xffff0000);

   return ret;
}


void nxs_make_scramble( int *scramble, const unsigned char *key, unsigned long keylen )
{
   unsigned long i,j, tmp;

   for (i = 0; i < keylen; i ++ )
   {
      scramble[ i ] = i;
   }

   for( i = 0; i < keylen; i ++ )
   {
      for( j = i + 1; j < keylen; j ++ )
      {
         if( key[ scramble[ j ] ] < key[ scramble[ i ] ] )
         {
            tmp = scramble[ j ];
            scramble[ j ] = scramble[ i ];
            scramble[ i ] = tmp;
            j = i;
         }
      }
   }
}

/*
* END OF NXS
*/

/***********************************
* XHarbour implementation
************************************/

/*****
* Encrypt a text using a key
* Usage:
* HB_Crypt( cSource, cKey ) --> cCipher
*/

HB_FUNC( HB_CRYPT )
{
   PHB_ITEM pSource = hb_param( 1, HB_IT_STRING );
   PHB_ITEM pKey = hb_param( 2, HB_IT_STRING );
   BYTE *cRes;

   if ( pSource == NULL || pSource->item.asString.length == 0 ||
        pKey == NULL || pKey->item.asString.length == 0 )
   {
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, "HB_CRYPT", 2,
            hb_param(1,HB_IT_ANY), hb_param(2,HB_IT_ANY) );
      return;
   }

   cRes = (BYTE *) hb_xgrab( pSource->item.asString.length + 8);

   nxs_crypt(
      (BYTE *) (pSource->item.asString.value), pSource->item.asString.length,
      (BYTE *) (pKey->item.asString.value), pKey->item.asString.length,
      cRes);

   hb_retclenAdopt( (char *)cRes, pSource->item.asString.length );
}

/*****
* Decrypt a text using a key
* Usage:
* HB_Decrypt( cCrypt, cKey ) --> cSource
*/

HB_FUNC( HB_DECRYPT )
{
   PHB_ITEM pSource = hb_param( 1, HB_IT_STRING );
   PHB_ITEM pKey = hb_param( 2, HB_IT_STRING );
   BYTE *cRes;

   if ( pSource == NULL || pSource->item.asString.length == 0 ||
        pKey == NULL || pKey->item.asString.length == 0 )
   {
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, "HB_DECRYPT", 2,
            hb_param(1,HB_IT_ANY), hb_param(2,HB_IT_ANY) );
      return;
   }

   cRes = (BYTE *) hb_xgrab( pSource->item.asString.length + 8);

   nxs_decrypt(
      (BYTE *) (pSource->item.asString.value), pSource->item.asString.length,
      (BYTE *) (pKey->item.asString.value), pKey->item.asString.length,
      cRes);

   hb_retclenAdopt( (char *)cRes, pSource->item.asString.length );
}

