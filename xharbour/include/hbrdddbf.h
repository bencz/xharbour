/*
 * $Id: hbrdddbf.h,v 1.18 2005/02/14 21:14:01 druzus Exp $
 */

/*
 * Harbour Project source code:
 * DBF RDD module
 *
 * Copyright 1999 Bruno Cantero <bruno@issnet.net>
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

#ifndef HB_RDDDBF_H_
#define HB_RDDDBF_H_

#include "hbsetup.h"
#include "hbapirdd.h"
#include "hbdbferr.h"
#ifndef HB_CDP_SUPPORT_OFF
#include "hbapicdp.h"
#endif

HB_EXTERN_BEGIN

/* DBF default file extensions */
#define DBF_TABLEEXT                      ".dbf"

/* DBF locking schemes */
#define DBF_LOCKPOS_CLIP                  1000000000UL
#define DBF_LOCKPOS_CL53                  1000000000UL
#define DBF_LOCKPOS_VFP                   0x40000000UL
#define DBF_LOCKPOS_VFPX                  0x7ffffffeUL
#define DBF_LOCKPOS_CL53EXT               4000000000UL
#define DBF_LOCKPOS_XHB64                 HB_LL( 0x7FFFFFFF00000001 )

#define DBF_LOCKDIR_CLIP                  1
#define DBF_LOCKDIR_CL53                  1
#define DBF_LOCKDIR_VFP                   2  /* lock forward at at record offset */
#define DBF_LOCKDIR_VFPX                  -1
#define DBF_LOCKDIR_CL53EXT               1
#define DBF_LOCKDIR_XHB64                 1

#define DBF_FLCKSIZE_CLIP                 1000000000UL
#define DBF_FLCKSIZE_CL53                 1000000000UL
#define DBF_FLCKSIZE_VFP                  0x3ffffffdUL
#define DBF_FLCKSIZE_VFPX                 0x07ffffffUL
#define DBF_FLCKSIZE_CL53EXT              294967295UL
#define DBF_FLCKSIZE_XHB64                0x7ffffffeUL

#define DBF_RLCKSIZE_CLIP                 1UL
#define DBF_RLCKSIZE_CL53                 1UL
#define DBF_RLCKSIZE_VFP                  1UL
#define DBF_RLCKSIZE_VFPX                 1UL
#define DBF_RLCKSIZE_CL53EXT              1UL
#define DBF_RLCKSIZE_XHB64                1UL

#define IDX_LOCKPOS_CLIP                  1000000000UL
#define IDX_LOCKPOS_CL53                  0xfffeffffUL
#define IDX_LOCKPOS_VFP                   0x7ffffffeUL
#define IDX_LOCKPOS_CL53EXT               0xfffeffffUL
#define IDX_LOCKPOS_XHB64                 HB_LL( 0x7FFFFFFF00000001 )

#define IDX_LOCKPOOL_CLIP                 0UL
#define IDX_LOCKPOOL_CL53                 0x00010000UL
#define IDX_LOCKPOOL_VFP                  0UL
#define IDX_LOCKPOOL_CL53EXT              0x00010000UL
#define IDX_LOCKPOOL_XHB64                0UL

#ifdef OS_UNIX_COMPATIBLE
#define DBF_EXLUSIVE_LOCKPOS              0x7fffffffUL
#define DBF_EXLUSIVE_LOCKSIZE             1UL
#endif

/*
 *  DBF WORKAREA
 *  ------------
 *  The Workarea Structure of DBF RDD
 *
 */

typedef struct _DBFAREA
{
   struct _RDDFUNCS * lprfsHost; /* Virtual method table for this workarea */
   USHORT uiArea;                /* The number assigned to this workarea */
   void * atomAlias;             /* Pointer to the alias symbol for this workarea */
   USHORT uiFieldExtent;         /* Total number of fields allocated */
   USHORT uiFieldCount;          /* Total number of fields used */
   LPFIELD lpFields;             /* Pointer to an array of fields */
   void * lpFieldExtents;        /* Void ptr for additional field properties */
   PHB_ITEM valResult;           /* All purpose result holder */
   BOOL fTop;                    /* TRUE if "top" */
   BOOL fBottom;                 /* TRUE if "bottom" */
   BOOL fBof;                    /* TRUE if "bof" */
   BOOL fEof;                    /* TRUE if "eof" */
   BOOL fFound;                  /* TRUE if "found" */
   DBSCOPEINFO dbsi;             /* Info regarding last LOCATE */
   DBFILTERINFO dbfi;            /* Filter in effect */
   LPDBORDERCONDINFO lpdbOrdCondInfo;
   LPDBRELINFO lpdbRelations;    /* Parent/Child relationships used */
   USHORT uiParents;             /* Number of parents for this area */
   USHORT heap;
   USHORT heapSize;
   USHORT rddID;
   USHORT uiMaxFieldNameLength;

   /*
   *  DBFS's additions to the workarea structure
   *
   *  Warning: The above section MUST match WORKAREA exactly!  Any
   *  additions to the structure MUST be added below, as in this
   *  example.
   */

   FHANDLE hDataFile;            /* Data file handle */
   FHANDLE hMemoFile;            /* Memo file handle */
   USHORT uiHeaderLen;           /* Size of header */
   USHORT uiRecordLen;           /* Size of record */
   ULONG ulRecCount;             /* Total records */
   char * szDataFileName;        /* Name of data file */
   char * szMemoFileName;        /* Name of memo file */
   USHORT uiMemoBlockSize;       /* Size of memo block */
   BYTE bMemoType;               /* MEMO type used in DBF memo fields */
   BOOL fHasMemo;                /* WorkArea with Memo fields */
   BOOL fHasTags;                /* WorkArea with MDX or CDX index */
   BOOL fDataFlush;              /* data was written to DBF and not commited */
   BOOL fMemoFlush;              /* data was written to MEMO and not commited */
   BYTE bVersion;                /* DBF version ID byte */
   BYTE bCodePage;               /* DBF codepage ID */
   BOOL fShared;                 /* Shared file */
   BOOL fReadonly;               /* Read only file */
   USHORT * pFieldOffset;        /* Pointer to field offset array */
   BYTE * pRecord;               /* Buffer of record data */
   BOOL fValidBuffer;            /* State of buffer */
   BOOL fPositioned;             /* Positioned record */
   ULONG ulRecNo;                /* Current record */
   BOOL fRecordChanged;          /* Record changed */
   BOOL fAppend;                 /* TRUE if new record is added */
   BOOL fDeleted;                /* TRUE if record is deleted */
   BOOL fEncrypted;              /* TRUE if record is encrypted */
   BOOL fUpdateHeader;           /* Update header of file */
   BOOL fFLocked;                /* TRUE if file is locked */
   BOOL fHeaderLocked;           /* TRUE if DBF header is locked */
   LPDBRELINFO lpdbPendingRel;   /* Pointer to parent rel struct */
   BYTE bYear;                   /* Last update */
   BYTE bMonth;
   BYTE bDay;
   BYTE bLockType;               /* Type of locking shemes */
   ULONG * pLocksPos;            /* List of records locked */
   ULONG ulNumLocksPos;          /* Number of records locked */
#ifndef HB_CDP_SUPPORT_OFF
   PHB_CODEPAGE cdPage;          /* Area's codepage pointer  */
#endif
} DBFAREA;

typedef DBFAREA * LPDBFAREA;

#ifndef DBFAREAP
#define DBFAREAP LPDBFAREA
#endif

#ifndef HB_EXTERNAL_RDDDBF_USE

/*
 * -- DBF METHODS --
 */

#define SUPERTABLE                         ( &dbfSuper )

static ERRCODE hb_dbfBof( DBFAREAP pArea, BOOL * pBof );
static ERRCODE hb_dbfEof( DBFAREAP pArea, BOOL * pEof );
static ERRCODE hb_dbfFound( DBFAREAP pArea, BOOL * pFound );
static ERRCODE hb_dbfGoBottom( DBFAREAP pArea );
static ERRCODE hb_dbfGoTo( DBFAREAP pArea, ULONG ulRecNo );
static ERRCODE hb_dbfGoToId( DBFAREAP pArea, PHB_ITEM pItem );
static ERRCODE hb_dbfGoTop( DBFAREAP pArea );
#define hb_dbfSeek                                 NULL
static ERRCODE hb_dbfSkip( DBFAREAP pArea, LONG lToSkip );
#define hb_dbfSkipFilter                           NULL
static ERRCODE hb_dbfSkipRaw( DBFAREAP pArea, LONG lToSkip );
static ERRCODE hb_dbfAddField( DBFAREAP pArea, LPDBFIELDINFO pFieldInfo );
static ERRCODE hb_dbfAppend( DBFAREAP pArea, BOOL bUnLockAll );
#define hb_dbfCreateFields                         NULL
static ERRCODE hb_dbfDeleteRec( DBFAREAP pArea );
static ERRCODE hb_dbfDeleted( DBFAREAP pArea, BOOL * pDeleted );
#define hb_dbfFieldCount                           NULL
#define hb_dbfFieldDisplay                         NULL
#define hb_dbfFieldInfo                            NULL
#define hb_dbfFieldName                            NULL
static ERRCODE hb_dbfFlush( DBFAREAP pArea );
static ERRCODE hb_dbfGetRec( DBFAREAP pArea, BYTE ** pBuffer );
static ERRCODE hb_dbfGetValue( DBFAREAP pArea, USHORT uiIndex, PHB_ITEM pItem );
static ERRCODE hb_dbfGetVarLen( DBFAREAP pArea, USHORT uiIndex, ULONG * pLength );
static ERRCODE hb_dbfGoCold( DBFAREAP pArea );
static ERRCODE hb_dbfGoHot( DBFAREAP pArea );
static ERRCODE hb_dbfPutRec( DBFAREAP pArea, BYTE * pBuffer );
static ERRCODE hb_dbfPutValue( DBFAREAP pArea, USHORT uiIndex, PHB_ITEM pItem );
static ERRCODE hb_dbfRecall( DBFAREAP pArea );
static ERRCODE hb_dbfRecCount( DBFAREAP pArea, ULONG * pRecCount );
static ERRCODE hb_dbfRecInfo( DBFAREAP pArea, PHB_ITEM pRecID, USHORT uiInfoType, PHB_ITEM pInfo );
static ERRCODE hb_dbfRecNo( DBFAREAP pArea, PHB_ITEM pRecNo );
static ERRCODE hb_dbfSetFieldExtent( DBFAREAP pArea, USHORT uiFieldExtent );
#define hb_dbfAlias                                NULL
static ERRCODE hb_dbfClose( DBFAREAP pArea );
static ERRCODE hb_dbfCreate( DBFAREAP pArea, LPDBOPENINFO pCreateInfo );
static ERRCODE hb_dbfInfo( DBFAREAP pArea, USHORT uiIndex, PHB_ITEM pItem );
static ERRCODE hb_dbfNewArea( DBFAREAP pArea );
static ERRCODE hb_dbfOpen( DBFAREAP pArea, LPDBOPENINFO pOpenInfo );
#define hb_dbfRelease                              NULL
static ERRCODE hb_dbfStructSize( DBFAREAP pArea, USHORT * uiSize );
static ERRCODE hb_dbfSysName( DBFAREAP pArea, BYTE * pBuffer );
#define hb_dbfEval                                 NULL
static ERRCODE hb_dbfPack( DBFAREAP pArea );
#define hb_dbfPackRec                              NULL
static ERRCODE hb_dbfSort( DBFAREAP pArea, LPDBSORTINFO pSortInfo );
static ERRCODE hb_dbfTrans( DBFAREAP pArea, LPDBTRANSINFO pTransInfo );
static ERRCODE hb_dbfTransRec( DBFAREAP pArea, LPDBTRANSINFO pTransInfo );
static ERRCODE hb_dbfZap( DBFAREAP pArea );
static ERRCODE hb_dbfChildEnd( DBFAREAP pArea, LPDBRELINFO pRelInfo );
static ERRCODE hb_dbfChildStart( DBFAREAP pArea, LPDBRELINFO pRelInfo );
static ERRCODE hb_dbfChildSync( DBFAREAP pArea, LPDBRELINFO pRelInfo );
#define hb_dbfSyncChildren                         NULL
#define hb_dbfClearRel                             NULL
static ERRCODE hb_dbfForceRel( DBFAREAP pArea );
#define hb_dbfRelArea                              NULL
#define hb_dbfRelEval                              NULL
#define hb_dbfRelText                              NULL
#define hb_dbfSetRel                               NULL
#define hb_dbfOrderListAdd                         NULL
#define hb_dbfOrderListClear                       NULL
#define hb_dbfOrderListDelete                      NULL
#define hb_dbfOrderListFocus                       NULL
#define hb_dbfOrderListRebuild                     NULL
#define hb_dbfOrderCondition                       NULL
#define hb_dbfOrderCreate                          NULL
#define hb_dbfOrderDestroy                         NULL
#define hb_dbfOrderInfo                            NULL
#define hb_dbfClearFilter                          NULL
#define hb_dbfClearLocate                          NULL
#define hb_dbfClearScope                           NULL
#define hb_dbfCountScope                           NULL
#define hb_dbfFilterText                           NULL
#define hb_dbfScopeInfo                            NULL
static ERRCODE hb_dbfSetFilter( DBFAREAP pArea, LPDBFILTERINFO pFilterInfo );
#define hb_dbfSetLocate                            NULL
#define hb_dbfSetScope                             NULL
#define hb_dbfSkipScope                            NULL
#define hb_dbfCompile                              NULL
#define hb_dbfError                                NULL
#define hb_dbfEvalBlock                            NULL
static ERRCODE hb_dbfRawLock( DBFAREAP pArea, USHORT uiAction, ULONG lRecNo );
static ERRCODE hb_dbfLock( DBFAREAP pArea, LPDBLOCKINFO pLockInfo );
static ERRCODE hb_dbfUnLock( DBFAREAP pArea, ULONG ulRecNo );
#define hb_dbfCloseMemFile                         NULL
static ERRCODE hb_dbfCreateMemFile( DBFAREAP pArea, LPDBOPENINFO pCreateInfo );
#define hb_dbfGetValueFile                         NULL
static ERRCODE hb_dbfOpenMemFile( DBFAREAP pArea, LPDBOPENINFO pOpenInfo );
#define hb_dbfPutValueFile                         NULL
static ERRCODE hb_dbfReadDBHeader( DBFAREAP pArea );
static ERRCODE hb_dbfWriteDBHeader( DBFAREAP pArea );

#define hb_dbfExit                         NULL
static ERRCODE hb_dbfDrop( PHB_ITEM pItemTable );

#define hb_dbfWhoCares                             NULL

#endif /* HB_EXTERNAL_RDDDBF_USE */

extern BOOL HB_EXPORT  hb_dbfExists( PHB_ITEM pItemTable, PHB_ITEM pItemIndex );
extern ULONG HB_EXPORT hb_dbfGetMemoBlock( DBFAREAP pArea, USHORT uiIndex );
extern void  HB_EXPORT hb_dbfPutMemoBlock( DBFAREAP pArea, USHORT uiIndex, ULONG ulBlock );
extern ERRCODE HB_EXPORT hb_dbfGetEGcode( ERRCODE errCode );
extern BOOL HB_EXPORT hb_dbfLockIdxFile( FHANDLE hFile, BYTE bScheme, USHORT usMode, HB_FOFFSET *pPoolPos );
extern BOOL HB_EXPORT hb_dbfLockIdxGetData( BYTE bScheme, HB_FOFFSET *ulPos, HB_FOFFSET *ulPool );

HB_EXTERN_END

#endif /* HB_RDDDBF_H_ */
