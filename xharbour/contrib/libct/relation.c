/*
 * $Id: relation.c,v 1.1 2003/03/04 21:04:50 lculik Exp $
 */

/*
 * Harbour Project source code:
 *   CHARRELA() and CHARRELREP() CT3 string functions
 *
 * Copyright 2001 IntTec GmbH, Neunlindenstr 32, 79106 Freiburg, Germany
 *        Author: Martin Vogel <vogel@inttec.de>
 *
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


#include "ct.h"


/*  $DOC$
 *  $FUNCNAME$
 *      CHARRELA()
 *  $CATEGORY$
 *      CT3 string functions
 *  $ONELINER$
 *      Character relation of two strings
 *  $SYNTAX$
 *      CHARRELA (<cStringToMatch1>, <cString1>,
 *                <cStringToMatch2>, <cString2>) -> nPosition
 *  $ARGUMENTS$
 *  $RETURNS$
 *  $DESCRIPTION$
 *      TODO: add documentation
 *  $EXAMPLES$
 *  $TESTS$
 *  $STATUS$
 *      Started
 *  $COMPLIANCE$
 *      CHARRELA() is compatible with CT3's CHARRELA().
 *  $PLATFORMS$
 *      All
 *  $FILES$
 *      Source is relation.c, library is libct.
 *  $SEEALSO$
 *      CHARRELREP()
 *  $END$
 */

HB_FUNC (CHARRELA)
{

  if (ISCHAR (1) &&
      ISCHAR (2) &&
      ISCHAR (3) &&
      ISCHAR (4))
  {  

    char *pcStringToMatch1 = hb_parc (1);
    size_t sStrToMatchLen1 = hb_parclen (1);
    char *pcString1 = hb_parc (2);
    size_t sStrLen1 = hb_parclen (2);
    char *pcStringToMatch2 = hb_parc (3);
    size_t sStrToMatchLen2 = hb_parclen (3);
    char *pcString2 = hb_parc (4);
    size_t sStrLen2 = hb_parclen (4);

    char *pc1, *pc2;
    size_t sOffset1, sOffset2;
    size_t sMatchStrLen;

    /* check for empty strings */
    if ((sStrToMatchLen1 == 0) ||
        (sStrToMatchLen2 == 0))
    {
      hb_retnl (0);
      return;
    }

    sOffset1 = 0;
    sOffset2 = 0;

    /* NOTE: this algorithm is not the best since the search that gave
       the larger relative position in the step before is repeated;
       try a search algorithm alternating between both strings */
    while ((sOffset1 < sStrLen1) && (sOffset2 < sStrLen2))
    {
      pc1 = ct_at_exact_forward (pcStringToMatch1, sStrToMatchLen1,
                                 pcString1+sOffset1, sStrLen1-sOffset1,
                                 &sMatchStrLen);
      pc2 = ct_at_exact_forward (pcStringToMatch2, sStrToMatchLen2,
                                 pcString2+sOffset2, sStrLen2-sOffset2,
                                 &sMatchStrLen);
      if ((pc1 != NULL) && (pc2 != NULL))
      {
        if ((pc1-pcString1) == (pc2-pcString2))
        {
          /* correlation found */
          hb_retnl ((pc1-pcString1)+1);
          return;
        }
        else
        {
          if ((pc1-pcString1) > (pc2-pcString2))
          {
            sOffset1 = sOffset2 = (pc1-pcString1);
          }
          else
          {
            sOffset1 = sOffset2 = (pc2-pcString2);
          }
        }
      }
      else
      {
        sOffset1 = sOffset2 = (sStrLen1 < sStrLen2 ? sStrLen1 : sStrLen2);
      }
    
    }
      
    hb_retnl (0);
    return;

  }
  else /* ISCHAR (1) &&
          ISCHAR (2) &&
          ISCHAR (3) &&
          ISCHAR (4)    */
  {
    PHB_ITEM pSubst = NULL;
    int iArgErrorMode = ct_getargerrormode();
    if (iArgErrorMode != CT_ARGERR_IGNORE)
    {
      pSubst = ct_error_subst ((USHORT)iArgErrorMode, EG_ARG, CT_ERROR_CHARRELA,
                               NULL, "CHARRELA", 0, EF_CANSUBSTITUTE, 4,
                               hb_paramError (1), hb_paramError (2),
                               hb_paramError (3), hb_paramError (4));
    }
    
    if (pSubst != NULL)
    {
      hb_itemReturn (pSubst);
      hb_itemRelease (pSubst);
    }
    else
    {
      hb_retnl (0);
    }
  }

  return;

}


/*  $DOC$
 *  $FUNCNAME$
 *      CHARRELREP()
 *  $CATEGORY$
 *      CT3 string functions
 *  $ONELINER$
 *      Relation dependant character replacement 
 *  $SYNTAX$
 *      CHARRELREP (<cStringToMatch1>, <cString1>,
 *                  <cStringToMatch2>, <[@]cString2>,
 *                  <cReplacement>) -> cString
 *  $ARGUMENTS$
 *  $RETURNS$
 *  $DESCRIPTION$
 *      TODO: add documentation
 *  $EXAMPLES$
 *  $TESTS$
 *  $STATUS$
 *      Started
 *  $COMPLIANCE$
 *      CHARRELREP() is compatible with CT3's CHARRELREP().
 *  $PLATFORMS$
 *      All
 *  $FILES$
 *      Source is relation.c, library is libct.
 *  $SEEALSO$
 *      CHARRELA(),CSETREF()
 *  $END$
 */

HB_FUNC (CHARRELREP)
{

  int iNoRet;

  iNoRet = ct_getref() && ISBYREF( 4 );

  if (ISCHAR (1) &&
      ISCHAR (2) &&
      ISCHAR (3) &&
      ISCHAR (4) &&
      ISCHAR (5))
  {  

    char *pcStringToMatch1 = hb_parc (1);
    size_t sStrToMatchLen1 = hb_parclen (1);
    char *pcString1 = hb_parc (2);
    size_t sStrLen1 = hb_parclen (2);
    char *pcStringToMatch2 = hb_parc (3);
    size_t sStrToMatchLen2 = hb_parclen (3);
    char *pcString2 = hb_parc (4);
    size_t sStrLen2 = hb_parclen (4);
    char *pcReplace = hb_parc (5);
    size_t sReplaceLen = hb_parclen (5);

    char *pcRet;

    char *pc1, *pc2;
    size_t sOffset1, sOffset2;
    size_t sMatchStrLen;

    /* check for empty strings */
    if ((sStrToMatchLen1 == 0) ||
        (sStrToMatchLen2 == 0) ||
        (sReplaceLen == 0))
    {
      if (iNoRet)
      {
        hb_ret();
      }
      else
      {
        hb_retclen (pcString2, sStrLen2);
      }
      return;
    }

    pcRet = ( char * ) hb_xgrab (sStrLen2);
    hb_xmemcpy (pcRet, pcString2, sStrLen2);

    sOffset1 = 0;
    sOffset2 = 0;

    /* NOTE: this algorithm is not the best since the search that gave
       the larger relative position in the step before is repeated;
       try a search algorithm alternating between both strings */
    while ((sOffset1 < sStrLen1) && (sOffset2 < sStrLen2))
    {
      pc1 = ct_at_exact_forward (pcStringToMatch1, sStrToMatchLen1,
                                 pcString1+sOffset1, sStrLen1-sOffset1,
                                 &sMatchStrLen);
      pc2 = ct_at_exact_forward (pcStringToMatch2, sStrToMatchLen2,
                                 pcString2+sOffset2, sStrLen2-sOffset2,
                                 &sMatchStrLen);
      if ((pc1 != NULL) && (pc2 != NULL))
      {
        if ((pc1-pcString1) == (pc2-pcString2))
        {
          /* correlation found -> start replacement */
          size_t sCurr;
          
          for (sCurr = 1; sCurr <= sStrToMatchLen1; sCurr++)
          {
            /* check if pcString2 is long enough */
            if ((pc2-pcString2)+sCurr >= sStrLen2)
            {
              size_t sStr2Offset, sReplOffset;
              sStr2Offset = (sStrToMatchLen2 < sCurr ? sStrToMatchLen2 : sCurr);
              sReplOffset = (sReplaceLen < sCurr ? sReplaceLen : sCurr);

              /* do the characters in pcString2 and pcStrToMatch2 match ? */
              if (*(pc2+sCurr-1) == *(pcStringToMatch2+sStr2Offset-1))
              {
                *(pcRet+(pc2-pcString2)+sCurr-1) = *(pcReplace+sReplOffset-1);
              }
            }
          }
          sOffset1 = sOffset2 = (pc1-pcString1)+1;
        }
        else
        {
          if ((pc1-pcString1) > (pc2-pcString2))
          {
            sOffset1 = sOffset2 = (pc1-pcString1);
          }
          else
          {
            sOffset1 = sOffset2 = (pc2-pcString2);
          }
        }
      }
      else
      {
        sOffset1 = sOffset2 = (sStrLen1 < sStrLen2 ? sStrLen1 : sStrLen2);
      }
    
    }

    if (ISBYREF (4))
    {
      hb_storclen (pcRet, sStrLen2, 4);
    }
  
    if (iNoRet)
    {
      hb_ret();
    }
    else
    {
      hb_retclen (pcRet, sStrLen2);
    }

    hb_xfree (pcRet);

  }
  else /* ISCHAR (1) &&
          ISCHAR (2) &&
          ISCHAR (3) &&
          ISCHAR (4) &&
          ISCHAR (5)    */
  {
    PHB_ITEM pSubst = NULL;
    int iArgErrorMode = ct_getargerrormode();
    if (iArgErrorMode != CT_ARGERR_IGNORE)
    {
      pSubst = ct_error_subst ((USHORT)iArgErrorMode, EG_ARG, CT_ERROR_CHARRELREP,
                               NULL, "CHARRELREP", 0, EF_CANSUBSTITUTE, 5, hb_paramError (1),
                               hb_paramError (2), hb_paramError (3),
                               hb_paramError (4), hb_paramError (5));
    }
    
    if (pSubst != NULL)
    {
      hb_itemReturn (pSubst);
      hb_itemRelease (pSubst);
    }
    else
    {
      if (iNoRet)
      {
        hb_ret();
      }
      else
      {
        hb_retc ("");
      }
    }
  }

  return;

}


