/*
 * $Id: trpccli.prg,v 1.27 2003/12/11 14:44:52 jonnymind Exp $
 */

/*
 * xHarbour Project source code:
 * Remote Procedure Call code
 * Client class
 *
 * Copyright 2003 Giancarlo Niccolai <giancarlo@niccolai.ws>
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

#include "hbclass.ch"
#include "hbrpc.ch"

CLASS tRPCClient

   DATA aServers
   DATA aFunctions

   DATA nUdpPort
   DATA nTcpPort

   /* asyncrhonous mode */
   DATA lAsyncMode
   /* block to be called at scan completion */
   DATA bOnScanComplete
   /*block called when there is a progress in the scan */
   DATA bOnScanServersProgress
   DATA bOnScanFunctionsProgress

   /* block to be called at function error */
   DATA bOnFunctionProgress
   /* block to be called at function success */
   DATA bOnFunctionReturn
   /* block to be called at function failure */
   DATA bOnFunctionFail


   METHOD New( cNetwork, nTcpPort, nUdpPort ) CONSTRUCTOR
   METHOD Destroy()

   /* Connection */
   METHOD Connect( cServer, cUserId, cPassword )
   METHOD Disconnect()

   /* Network scan functions */
   METHOD ScanServers( cName )
   METHOD ScanFunctions( cName, cSerial )
   METHOD ScanAgain()            INLINE ::UDPAccept()
   METHOD StopScan()

   /* Function call */
   METHOD CheckServer()    //Checks if a server is ready on tcp
   METHOD SetLoopMode( nMethod, xData, nEnd, nStep )
   METHOD Call()  // variable parameters
   METHOD CallAgain()            INLINE ::TCPAccept()
   METHOD StopCall()

   METHOD SetPeriodCallback()
   METHOD ClearPeriodCallback()


   /* Accessors */
   METHOD SetEncryption( cKey )
   METHOD IsEncrypted()             INLINE ::cCryptKey != NIL
   METHOD GetStatus()               INLINE ::nStatus
   METHOD SetTimeout( nTime )
   METHOD GetTimeout()
   METHOD GetResult()               INLINE ::oResult
   METHOD FoundServers()            INLINE Len( ::aServers ) != 0
   METHOD FoundFunctions()          INLINE Len( ::aFunctions ) != 0

   METHOD HasError()                INLINE ::nErrorCode != 0 .or. ::TcpHasError() .or. ::UdpHasError()
   METHOD GetErrorCode()            INLINE ::nErrorCode

   METHOD TcpHasError()             INLINE IIF( Empty( ::skTCP ), .F., InetErrorCode( ::skTCP ) > 0 )
   METHOD GetTcpErrorCode()         INLINE IIF( Empty( ::skTCP ), 0, InetErrorCode( ::skTCP ) )
   METHOD GetTcpErrorDesc()         INLINE IIF( Empty( ::skTCP ), "", InetErrorDesc( ::skTCP ) )

   METHOD UdpHasError()             INLINE IIF( Empty( ::skUDP ), .F., InetErrorCode( ::skUDP ) > 0 )
   METHOD UdpGetErrorCode()         INLINE IIF( Empty( ::skUDP ), 0, InetErrorCode( ::skUDP ) )
   METHOD UdpGetErrorDesc()         INLINE IIF( Empty( ::skUDP ), "", InetErrorDesc( ::skUDP ) )
   /* Used to retreive data from scans */
   METHOD GetFunctionName( xId )
   METHOD GetServerName( xId )
   METHOD GetServerAddress( xId )

HIDDEN:
   // Automatic initialization of inet support
   CLASSDATA lInit INIT InetInit()

   #ifdef HB_THREAD_SUPPORT
      DATA mtxBusy INIT HB_MutexCreate()
   #endif

   DATA nStatus
   // This RPC protocol breaking error code
   DATA nErrorCode

   /* Network data */
   DATA cServer
   DATA cNetwork
   DATA skUdp
   DATA skTcp

   /* Timeout system */
   DATA nTimeout        INIT -1
   DATA nTimeLimit      INIT -1
   DATA caPerCall

   DATA nUdpTimeBegin   INIT 0
   DATA thUdpAccept     INIT NIL

   DATA nTcpTimeBegin   INIT 0
   DATA thTcpAccept     INIT NIL

   /* XHB RPC Loop system */
   DATA nLoopMode
   DATA aLoopData
   DATA nLoopStart
   DATA nLoopEnd
   DATA nLoopStep

   /* Encryption data */
   DATA bEncrypted
   DATA cCryptKey

   /* Last connection result */
   DATA oResult

   /* Encryption system */
   METHOD Encrypt( cData )
   METHOD Decrypt( cData )
   METHOD BuildChallengePwd( cPassword )
   METHOD ManageChallenge()

   /* Network negotiation system */
   METHOD StartScan()
   METHOD UDPAccept()
   METHOD UDPParse( cData, nLen )
   METHOD TCPAccept()
   METHOD TCPParse( cData )
   METHOD clearTCPBuffer()

   /* internal network send call */
   METHOD SendCall(cFunction,aParams )

   /* event handlers */
   METHOD OnScanComplete()
   METHOD OnScanServersProgress( aLoc )
   METHOD OnScanFunctionsProgress( aLoc )

   METHOD OnFunctionFail( nReason, cReason )
   METHOD OnFunctionReturn( oReturn )
   METHOD OnFunctionProgress( nProgress, oData )

ENDCLASS


METHOD New( cNetwork, nTcpPort, nUdpPort ) CLASS tRPCClient
   ::nStatus := RPC_STATUS_NONE // not connected
   ::nErrorCode := 0 // no RPC error
   ::cServer := NIL // no server

   ::nUdpPort := IIF( nUdpPort == NIL, 1139, nUdpPort )
   ::nTcpPort := IIF( nTcpPort == NIL, 1140, nTcpPort )

   ::skTcp := InetCreate()
   ::skUdp := InetDGram( .T. )
   ::lAsyncMode := .F.
   ::aServers := {}
   ::aFunctions := {}
   ::cNetwork := cNetwork
   ::bEncrypted := .F.

   ::nLoopMode := RPC_LOOP_NONE
RETURN Self


METHOD Destroy() CLASS tRPCClient

#ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
#endif
      ::Disconnect()
      #ifdef HB_THREAD_SUPPORT
      IF IsValidThread( ::thUdpAccept )
         KillThread( ::thUdpAccept )
         ::thUdpAccept := NIL
      ENDIF
      IF IsValidThread( ::thTcpAccept )
         KillThread( ::thTcpAccept )
         ::thTcpAccept := NIL
      ENDIF
      #endif
#ifdef HB_THREAD_SUPPORT
   HB_MutexUnlock( ::mtxBusy )
#endif
RETURN .T.


METHOD SetEncryption( cKey )
   IF .not. Empty( cKey )
      ::bEncrypted := .T.
      ::cCryptKey := cKey
   ELSE
      ::bEncrypted := .F.
   ENDIF
RETURN .T.


METHOD ScanServers(cName) CLASS tRPCClient
   // do not allow asynchronous mode without timeout
   IF .not. ::lAsyncMode .and. ( ::nTimeout == NIL .or. ::nTimeOut <= 0 )
      RETURN .F.
   ENDIF

   #ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   ::aServers = {}
   HB_MutexUnlock( ::mtxBusy )
   #else
   ::aServers = {}
   #endif

   InetDGramSend( ::skUDP, ::cNetwork , ::nUdpPort, "XHBR00" + HB_Serialize( cName ) )
   ::StartScan()

RETURN .F.

METHOD CheckServer( cRemote )
   LOCAL cData, skRemote, nLen, cData2

   cData := "XHBR00"
   IF cRemote == NIL
      cRemote := ::cNetwork
   ENDIF
   skRemote := InetConnect( cRemote, ::nTcpPort )
   IF InetErrorCode( skRemote ) == 0
      InetSetTimeout(skRemote, 10000)
      InetSendAll( skRemote, cData )
      cData := space(256)
      InetRecvAll( skRemote, @cData, 6+9 )
      IF InetErrorCode( skRemote ) == 0
         cData2 := Space(256)
         nLen := HB_GetLen8( substr( cData, 8, 8 ) )
         nLen := InetRecvAll( skRemote, @cData2, nLen )
         IF InetErrorCode( skRemote ) == 0
            cData := Substr( cData + cData2, 7 )
            cData2 := HB_Deserialize( cData )
            AAdd(::aServers, {InetAddress( skRemote ), cData2} )
            RETURN .T.
         ENDIF
      ENDIF
   ENDIF
RETURN .F.

METHOD ScanFunctions(cFunc, cSerial ) CLASS tRPCClient
   // do not allow asynchronous mode without timeout
   IF .not. ::lAsyncMode .and. ( ::nTimeOut == NIL .or. ::nTimeOut <= 0 )
      RETURN .F.
   ENDIF

   IF cSerial == NIL
      cSerial := "00000000.0"
   ENDIF
#ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   ::aFunctions = {}
   ::aServers = {}
   HB_MutexUnlock( ::mtxBusy )
#else
   ::aFunctions = {}
   ::aServers = {}
#endif

   InetDGramSend( ::skUDP, ::cNetwork, ::nUdpPort,;
         "XHBR01" + HB_Serialize( cFunc ) + HB_Serialize( cSerial ))
   ::StartScan()

RETURN .F.


METHOD StartScan()
   // We don't accept sync call without timeout

   IF ::lAsyncMode
      // in async mode, force scanning stop
      ::StopScan()
   ENDIF

   ::nUDPTimeBegin := INT( Seconds() * 1000 )

   // in async mode, just launch the listener
   #ifdef HB_THREAD_SUPPORT
   IF ::lAsyncMode
      HB_MutexLock( ::mtxBusy )
         ::thUdpAccept := StartThread( Self, "UDPAccept" )
      HB_MutexUnlock( ::mtxBusy )
   ELSE
      ::UDPAccept()
   ENDIF
   #else
      ::UDPAccept()
   #endif

RETURN .T.


METHOD UDPAccept() CLASS tRPCClient
   LOCAL nTime, nDatalen, cData

   cData := Space( 1400 )
   // set default socket timeout
   IF ::nTimeout >= 0
      InetSetTimeout( ::skUDP, ::nTimeout )
   ELSE
      InetClearTimeout( ::skUdp )
   ENDIF

   DO WHILE .T.
      nDatalen := InetDGramRecv( ::skUDP, @cData, 1400 )

      IF nDataLen > 0 .and. ::UDPParse( cData, nDatalen )
         EXIT
      ENDIF

      IF ::nTimeout >= 0
         nTime := Int( Seconds() * 1000 )
         // a little tollerance must be added for double roundings
         // in the double INT() functions
         IF nTime - ::nUDPTimeBegin >= ::nTimeout - 5
            EXIT
         ENDIF
      ENDIF

   ENDDO

   ::OnScanComplete()
   // signal that this thread is no longer active
   #ifdef HB_THREAD_SUPPORT
      HB_MutexLock( ::mtxBusy )
      ::thUdpAccept := NIL
      HB_MutexUnlock( ::mtxBusy )
   #else
      ::thUdpAccept := NIL
   #endif

RETURN .T.


METHOD UDPParse( cData, nLen ) CLASS tRPCClient
   LOCAL cCode, cSer, cFunc, cName
   LOCAL aLoc

   IF nLen < 12
      RETURN .F.
   ENDIF

   cCode := Substr( cData, 1, 6 )

   DO CASE
      /* XHRB00 - server scan */
      CASE cCode == "XHBR10"
         cData := Substr( cData, 7 )
         cData := HB_Deserialize( cData, 512 )
         // deserialization error checking
         IF cData != NIL
            aLoc := { InetAddress( ::skUDP ), cData }
            AAdd( ::aServers, aLoc )
            RETURN ::OnScanServersProgress( aLoc )
         ELSE
            RETURN .F.
         ENDIF


      CASE cCode == "XHBR11"
         cData := Substr( cData, 7 )
         cSer := HB_DeserialBegin( cData )
         cName := HB_DeserialNext( cSer, 64 )
         cFunc := HB_DeserialNext( cSer, 64 )
         IF cName != NIL .and. cFunc != NIL
            aLoc := { InetAddress( ::skUDP ), cName, cFunc }
            AAdd( ::aFunctions, aLoc )
            RETURN ::OnScanFunctionsProgress( aLoc )
         ELSE
            RETURN .F.
         ENDIF

   ENDCASE

RETURN .F.


METHOD StopScan() CLASS tRPCClient
#ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   IF IsValidThread( ::thUDPAccept )
      KillThread( ::thUDPAccept )
      ::thUDPAccept := NIL
      HB_MutexUnlock( ::mtxBusy )
      ::OnScanComplete()
   ELSE
      HB_MutexUnlock( ::mtxBusy )
   ENDIF
#else
  ::OnScanComplete()
#endif
RETURN .T.


METHOD Connect( cServer, cUserId, cPassword ) CLASS tRPCClient
   LOCAL cAuth, cReply := Space(8)

   InetConnect( cServer, ::nTcpPort, ::skTcp  )

   IF InetErrorCode( ::skTcp ) == 0
      ::nStatus := RPC_STATUS_CONNECTED // Connected
      IF ::bEncrypted
         cAuth := ::BuildChallengePwd( cPassword )
         cAuth := cUserId + ":" + cAuth
         InetSendAll( ::skTcp, "XHBR93" + HB_CreateLen8( Len( cAuth ) ) + cAuth )
      ELSE
         cAuth := cUserId + ":" + cPassword
         InetSendAll( ::skTcp, "XHBR90" + HB_CreateLen8( Len( cAuth ) ) + cAuth )
      ENDIF

      IF InetErrorCode( ::skTcp ) == 0
         IF .not. ::bEncrypted
            InetRecvAll( ::skTcp, @cReply )
            IF InetErrorCode( ::skTcp ) == 0 .and. cReply == "XHBR91OK"
               ::nStatus := RPC_STATUS_LOGGED // Logged in
               RETURN .T.
            ENDIF
         ELSE
            RETURN ::ManageChallenge()
         ENDIF

      ENDIF
   ENDIF

   ::skTcp := NIL
   ::nStatus := RPC_STATUS_NONE
RETURN .F.


METHOD BuildChallengePwd( cPassword ) CLASS tRPCClient
   LOCAL nLen, nCount, cRet

   nLen := 10 + INT( HB_Random( 1, 60 ) )

   cRet := ""

   FOR nCount := 1 TO nLen
      cRet += Chr( Int( HB_Random( 2, 254 ) ) )
   NEXT
   cRet += "PASSWORD:" + cPassword + ":"

   DO WHILE Len( cRet ) < 100
      cRet += Chr( Int( HB_Random( 2, 254 ) ) )
   ENDDO

   cRet := ::Encrypt( cRet )
RETURN cRet


METHOD ManageChallenge() CLASS tRPCClient
   LOCAL cCode, cLen, nLen
   LOCAL cData, nChallenge

   cCode := Space( 6 )
   IF InetRecvAll( ::skTCP, @cCode ) != 6
      RETURN .F.
   ENDIF

   IF cCode != "XHBR94"
      RETURN .F.
   ENDIF

   cLen := Space( 8 )
   IF InetRecvAll( ::skTCP, @cLen ) != 8
      RETURN .F.
   ENDIF

   nLen := HB_GetLen8( cLen )
   cData := Space( nLen )
   IF InetRecvAll( ::skTCP, @cData, nLen ) != nLen
      RETURN .F.
   ENDIF

   cData := HB_Decrypt( cData, ::cCryptKey )
   nChallenge := HB_Checksum( cData )
   InetSendAll( ::skTCP, "XHBR95" + HB_CreateLen8( nChallenge ) )
   //IF InetErrorCode( ::skTCP ) != 0
   //   RETURN .F.
   //ENDIF

   cCode := Space( 8 )
   InetRecvAll( ::skTCP, @cCode )
   IF InetErrorCode( ::skTCP ) != 0 .or. cCode != "XHBR91OK"
      RETURN .F.
   ENDIF
   /* SUCCESS! */
   ::nStatus := RPC_STATUS_LOGGED

RETURN .T.


METHOD Disconnect() CLASS tRPCClient

   IF ::nStatus >= RPC_STATUS_LOGGED
      #ifdef HB_THREAD_SUPPORT
         HB_MutexLock( ::mtxBusy )
      #endif
      ::nStatus :=  RPC_STATUS_NONE
      InetSendAll( ::skTcp, "XHBR92" )
      InetClose( ::skTcp )
      #ifdef HB_THREAD_SUPPORT
         HB_MutexUnlock( ::mtxBusy )
      #endif
      RETURN .T.
   ENDIF

RETURN .F.



METHOD SetLoopMode( nMethod, xData, nEnd, nStep ) CLASS tRPCClient

   IF nMethod == RPC_LOOP_NONE
      ::nLoopMode := RPC_LOOP_NONE
      ::aLoopData := NIL
      RETURN .T.
   ENDIF

   IF ValType( xData ) == "A"
      ::aLoopData := xData
   ELSE
      IF ValType( xData ) == "NI"
         // this is to allow garbage collecting
         ::aLoopData := NIL
         ::nLoopStart := xData
         ::nLoopEnd := nEnd
         IF ValType( nStep ) == "NI"
            ::nLoopStep := nStep
         ELSE
            ::nLoopStep := 1
         ENDIF
      ELSE
         RETURN .F.
      ENDIF
   ENDIF

   ::nLoopMode := nMethod

RETURN .T.

METHOD ClearTCPBuffer() CLASS tRPCClient
   LOCAL cDummy := Space( 512 )

   IF ::skTCP == NIL .or. ::nStatus < RPC_STATUS_LOGGED
      RETURN .F.
   ENDIF

   DO WHILE InetDataReady( ::skTCP ) > 0
      // InetRecv reads only the available data
      InetRecv( ::skTCP, @cDummy )
   ENDDO
RETURN .T.


METHOD Call( ... ) CLASS tRPCClient
   LOCAL oCalling, xParam
   LOCAL cFunction, aParams
   LOCAL nCount

   IF Pcount() == 0
      RETURN NIL
   ENDIF

   ::oResult := NIL

   // do not allow asynchronous mode without timeout
   IF .not. ::lAsyncMode .and. ( ::nTimeOut == NIL .or. ::nTimeOut <= 0 )
      RETURN NIL
   ENDIF

   oCalling := HB_PValue( 1 )
   IF ValType( oCalling ) == "A"
      cFunction := oCalling[1]
      ADel( oCalling, 1 )
      ASize( oCalling, Len( oCalling ) -1 )
      aParams := oCalling
   ELSE
      cFunction := oCalling
      aParams := Array( Pcount() -1 )
      FOR nCount := 2 TO Pcount()
         aParams[nCount - 1] :=  HB_PValue( nCount )
      NEXT
   ENDIF

   // clear eventual pending data
   ::ClearTcpBuffer()

   // The real call
   #ifdef HB_THREAD_SUPPORT
      HB_MutexLock( ::mtxBusy )
      // already active or not already connected
      IF IsValidThread( ::thTcpAccept ) .or. ::skTCP == NIL .or. ::nStatus < RPC_STATUS_LOGGED
         HB_MutexUnlock( ::mtxBusy )
         RETURN NIL
      ENDIF
      HB_MutexUnlock( ::mtxBusy )
   #else
      IF ::skTCP == NIL .or. ::nStatus < RPC_STATUS_LOGGED
         RETURN NIL
      ENDIF
   #endif

   ::nStatus := RPC_STATUS_WAITING // waiting for a reply

   // send the call through the socket
   IF .not. ::SendCall( cFunction, aParams )
      RETURN .F.
   ENDIF

   // in async mode, just launch the listener
   #ifdef HB_THREAD_SUPPORT
      IF ::lAsyncMode
         HB_MutexLock( ::mtxBusy )
         ::thTCPAccept := StartThread( Self, "TCPAccept" )
         HB_MutexUnlock( ::mtxBusy )
      ELSE
         ::TCPAccept()
      ENDIF
   #else
      ::TCPAccept()
   #endif

RETURN ::oResult


METHOD SetPeriodCallback( ... ) CLASS tRPCClient
   LOCAL caCalling
   LOCAL nCount

   IF Pcount() < 3
      //TODO set an error
      RETURN .F.
   ENDIF

   #ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   #endif
   ::nTimeout := HB_PValue( 1 )
   ::nTimeLimit := HB_PValue( 2 )

   caCalling := HB_PValue( 3 )
   IF ValType( caCalling ) != "A"
      caCalling := Array( Pcount() -2 )
      FOR nCount := 3 TO Pcount()
         caCalling[nCount - 2] :=  HB_PValue( nCount )
      NEXT
   ENDIF
   ::caPerCall := caCalling

   IF ::skTCP != NIL
      InetSetTimeout( ::skTCP, ::nTimeout )
      InetSetTimeLimit( ::skTCP, ::nTimeLimit )
      InetSetPeriodCallback( ::skTCP, caCalling )
   ENDIF

   #ifdef HB_THREAD_SUPPORT
   HB_MutexUnlock( ::mtxBusy )
   #endif

RETURN .T.


METHOD ClearPeriodCallback() CLASS tRPCClient
   #ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   #endif

   ::nTimeout := -1
   ::nTimeLimit := -1
   ::caPerCall := NIL

   IF ::skTCP != NIL
      InetClearTimeout( ::skTCP )
      InetClearTimeLimit( ::skTCP )
      InetClearPeriodCallback( ::skTCP )
   ENDIF

   #ifdef HB_THREAD_SUPPORT
   HB_MutexUnlock( ::mtxBusy )
   #endif
RETURN .T.


METHOD SetTimeout( nTime ) CLASS tRPCClient
   #ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   #endif

   ::nTimeout := nTime
   InetSetTimeout( ::skTCP, ::nTimeout )

   #ifdef HB_THREAD_SUPPORT
   HB_MutexUnlock( ::mtxBusy )
   #endif
RETURN .T.


METHOD GetTimeout()
   LOCAL nRet
   #ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   #endif
   nRet := ::nTimeout
   #ifdef HB_THREAD_SUPPORT
   HB_MutexUnlock( ::mtxBusy )
   #endif
RETURN nRet


METHOD StopCall() CLASS tRPCClient

   IF ::nStatus != RPC_STATUS_WAITING
      RETURN .F.
   ENDIF

   // clear eventual pending data
   ::ClearTcpBuffer()

   // send cancelation request
   InetSendAll( ::skTCP, "XHBR29" );

   //Stops waiting for a result
#ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   IF IsValidThread( ::thTCPAccept )
      KillThread( ::thTCPAccept )
      ::thTCPAccept := NIL
      ::nStatus := RPC_STATUS_LOGGED
      HB_MutexUnlock( ::mtxBusy )
      ::OnFunctionReturn( NIL )
   ELSE
      HB_MutexUnlock( ::mtxBusy )
   ENDIF
#else
  ::nStatus := RPC_STATUS_LOGGED
  ::OnFunctionReturn( NIL )
#endif

RETURN .T.


METHOD SendCall( cFunction, aParams ) CLASS tRPCClient
   LOCAL cData := "", nLen
   LOCAL nReq, cType

   SWITCH ::nLoopMode
      CASE RPC_LOOP_NONE
         nReq = 0
         cType = ""
      EXIT

      CASE RPC_LOOP_ALLDATA
         nReq = 2
         cType = "A"
      EXIT

      CASE RPC_LOOP_SUMMARY
         nReq = 2
         cType = "C"
      EXIT

      CASE RPC_LOOP_CONFIRMATION
         nReq = 2
         cType = "E"
      EXIT
   END

   IF ::aLoopData == NIL .and. ::nLoopMode > RPC_LOOP_NONE
      cData := HB_Serialize( ::nLoopStart ) + HB_Serialize( ::nLoopEnd ) +;
         HB_Serialize( ::nLoopStep )
   ENDIF

   cData +=  HB_Serialize( cFunction ) + HB_Serialize( aParams )

   IF ::aLoopData != NIL
      cData += HB_Serialize( ::aLoopData )
      nReq += 2
   ENDIF

   nLen := Len( cData )
   IF nLen > 512
      cData := HB_Compress( cData )
      cData := "XHBR2" + AllTrim( Str( nReq + 1 ) ) + ;
         HB_CreateLen8( nLen ) + HB_CreateLen8( Len( cData ) ) +;
            cType + ::Encrypt( cData )
   ELSE
      cData := "XHBR2" + AllTrim( Str( nReq ) ) + HB_CreateLen8( nLen ) +;
             cType + ::Encrypt( cData)
   ENDIF

   InetSendAll( ::skTCP,  cData )
RETURN ( InetErrorCode( ::skTCP ) == 0 )


METHOD TCPAccept() CLASS tRPCClient
   LOCAL nTime := 0
   LOCAL cCode, bContinue
   LOCAL nTimeLimit

   // TcpAccept can also be called standalone, without the
   // support of call(). So, we must set the waiting state.
   #ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   #endif

   ::nErrorCode := 0
   ::nStatus := RPC_STATUS_WAITING
   bContinue := IIF( ::caPerCall != NIL, .T., .F. )

   #ifdef HB_THREAD_SUPPORT
   HB_MutexUnlock( ::mtxBusy )
   #endif

   cCode := Space(6)
   ::nTCPTimeBegin := INT( Seconds() * 1000 )
   nTimeLimit = Max( ::nTimeout, ::nTimeLimit )


   DO WHILE .T.
      IF InetRecvAll( ::skTCP, @cCode, 6 ) <= 0
         EXIT
      ENDIF

      IF .not. ::TCPParse( cCode )
         EXIT
      ENDIF

      IF nTimeLimit >= 0
         nTime := Int( Seconds() * 1000 )
         // a little tollerance must be added for double roundings
         // in the double INT() functions
         IF nTime - ::nTCPTimeBegin >= nTimeLimit - 5
            EXIT
         ENDIF
      ENDIF
   ENDDO

   #ifdef HB_THREAD_SUPPORT
   HB_MutexLock( ::mtxBusy )
   #endif

   // NOT waiting anymore
   ::nStatus := RPC_STATUS_LOGGED
   ::thTcpAccept := NIL

   IF ::caPerCall == NIL .and. InetErrorCode( ::skTCP ) != -1 .and.;
                   nTime - nTimeLimit < nTimeLimit - 5
      IF InetErrorCode( ::skTCP ) != 0
         ::nStatus := RPC_STATUS_ERROR
      ENDIF
   ENDIF

   #ifdef HB_THREAD_SUPPORT
   HB_MutexUnlock( ::mtxBusy )
   #endif

RETURN .T.


METHOD TCPParse( cCode ) CLASS tRPCClient
   LOCAL nDataLen, cData, nOrigLen
   LOCAL cDataLen := Space( 8 ), cOrigLen := Space( 8 )
   LOCAL cProgress := Space( 10 ), nProgress
   LOCAL lContinue := .F.

   ::nErrorCode := 0

   DO CASE
      /* Warn error codes */
      CASE cCode == "XHBR40"
         cData := Space(2)
         InetRecvAll( ::skTCP, @cData, 2 )
         ::nErrorCode := Val( cData )
         ::OnFunctionFail( ::nErrorCode, "No description for now" )

      /* We have a reply */
      CASE cCode == "XHBR30"
         IF InetRecvAll( ::skTCP, @cDataLen ) == Len( cDataLen )
            nDataLen := HB_GetLen8( cDataLen )
            cData := Space( nDataLen )
            IF InetRecvAll( ::skTCP, @cData, nDataLen ) == nDataLen
               ::oResult := HB_Deserialize( ::Decrypt( cData ), nDataLen )
               IF ::oResult != NIL
                  ::OnFunctionReturn( ::oResult )
               ENDIF
               // todo: rise an error if ::oResult is nil
            ENDIF
         ENDIF

      /* We have a reply */
      CASE cCode == "XHBR31"
         IF InetRecvAll( ::skTCP, @cOrigLen ) == Len( cOrigLen )
            nOrigLen = HB_GetLen8( cOrigLen )
            IF InetRecvAll( ::skTCP, @cDataLen ) == Len( cDataLen )
               nDataLen := HB_GetLen8( cDataLen )
               cData := Space( nDataLen )
               IF InetRecvAll( ::skTCP, @cData ) == nDataLen
                  cData := HB_Uncompress( nOrigLen, ::Decrypt( cData ) )
                  IF .not. Empty( cData )
                     ::oResult := HB_Deserialize( cData, nDataLen )
                     IF ::oResult != NIL
                        ::OnFunctionReturn( ::oResult )
                     ENDIF
                  ENDIF
               ENDIF
            ENDIF
         ENDIF

      /* We have a progress */
      CASE cCode == "XHBR33"
         IF InetRecvAll( ::skTCP, @cProgress, 10 ) == 10
            nProgress := HB_Deserialize( cProgress, 10 )
            IF nProgress != NIL
               lContinue := .T.
               ::OnFunctionProgress( nProgress )
            ENDIF
         ENDIF

      /* We have a progress with data*/
      CASE cCode == "XHBR34"
         IF InetRecvAll( ::skTCP, @cProgress ) == Len( cProgress )
            nProgress := HB_Deserialize( cProgress, Len( cProgress) )
            IF nProgress != NIL .and. InetRecvAll( ::skTCP, @cDataLen ) == Len( cDataLen )
               nDataLen := HB_GetLen8( cDataLen )
               cData := Space( nDataLen )
               IF InetRecvAll( ::skTCP, @cData ) == nDataLen
                  ::oResult := HB_Deserialize(::Decrypt( cData), nDataLen )
                  IF ::oResult != NIL
                     lContinue := .T.
                     ::OnFunctionProgress( nProgress, ::oResult  )
                  ENDIF
               ENDIF
            ENDIF
         ENDIF

      /* We have a progress with compressed data*/
      CASE cCode == "XHBR35"
         IF InetRecvAll( ::skTCP, @cProgress ) == Len( cProgress )
            nProgress := HB_Deserialize( cProgress, Len( cProgress ) )
            IF nProgress != NIL .and. InetRecvAll( ::skTCP, @cOrigLen ) == Len( cOrigLen )
               nOrigLen = HB_GetLen8( cOrigLen )
               IF InetRecvAll( ::skTCP, @cDataLen ) == Len( cDataLen )
                  nDataLen := HB_GetLen8( cDataLen )
                  cData := Space( nDataLen )
                  IF InetRecvAll( ::skTCP, @cData ) == nDataLen
                     cData := HB_Uncompress( nOrigLen, cData )
                     IF .not. Empty( cData )
                        ::oResult := HB_Deserialize( ::Decrypt( cData), nDataLen )
                        IF ::oResult != NIL
                           lContinue := .T.
                           ::OnFunctionProgress( nProgress, ::oResult )
                        ENDIF
                     ENDIF
                  ENDIF
               ENDIF
            ENDIF
         ENDIF
   ENDCASE

RETURN lContinue

/***********************************
* Utility functions
************************************/
METHOD GetFunctionName( xId ) CLASS tRpcClient
   LOCAL cData, nPos

   IF ValType( xID ) == "A"
      cData := xId[3]
   ELSEIF Len( ::aFunctions ) > 0
      cData := ::aFunctions[xId][3]
   ELSE
      cData := ""
   ENDIF

   IF .not. Empty(cData)
      nPos := At( "(", cData )
      cData := Substr( cData, 1, nPos-1 )
   ENDIF

RETURN cData


METHOD GetServerName( xId ) CLASS tRpcClient
   LOCAL cData

   IF ValType( xID ) == "A"
      cData := xId[2]
   ELSE
      IF Len( ::aFunctions ) > 0
         cData := ::aFunctions[xId][2]
      ELSEIF Len( ::aServers ) > 0
         cData := ::aServers[xId][2]
      ELSE
         cData := ""
      ENDIF
   ENDIF
RETURN cData


METHOD GetServerAddress( xId ) CLASS tRpcClient
   LOCAL cData, nPos

   IF ValType( xID ) == "A"
      cData := xId[1]
   ELSE
      IF .not. Empty( ::aFunctions )
         cData := ::aFunctions[xId][1]
      ELSEIF .not. Empty( ::aServers )
         cData := ::aServers[xId][1]
      ELSE
         cData := ""
      ENDIF
   ENDIF

RETURN cData


METHOD Encrypt(cDataIn) CLASS tRPCClient
   IF ::bEncrypted
      RETURN HB_Crypt( cDataIn, ::cCryptKey )
   ENDIF
RETURN cDataIn


METHOD Decrypt(cDataIn) CLASS tRPCClient
   IF ::bEncrypted
      RETURN HB_Decrypt( cDataIn, ::cCryptKey )
   ENDIF
RETURN cDataIn


/***********************************
* Event handlers
************************************/

METHOD OnScanComplete() CLASS tRPCClient
   IF ::bOnScanComplete != NIL
      RETURN Eval( ::bOnScanComplete )
   ENDIF
RETURN .T.

METHOD OnScanServersProgress( aLoc ) CLASS tRPCClient
   IF ::bOnScanServersProgress != NIL
      RETURN Eval( ::bOnScanServersProgress, aLoc )
   ENDIF
RETURN .T.

METHOD OnScanFunctionsProgress( aLoc ) CLASS tRPCClient
   IF ::bOnScanFunctionsProgress != NIL
      RETURN Eval( ::bOnScanFunctionsProgress, aLoc )
   ENDIF
RETURN .T.

METHOD OnFunctionFail( nReason, cReason ) CLASS tRPCClient
   IF ::bOnFunctionFail != NIL
      RETURN Eval( ::bOnFunctionFail, nReason, cReason )
   ENDIF
RETURN .T.

METHOD OnFunctionReturn( oReturn ) CLASS tRPCClient
   IF ::bOnFunctionReturn != NIL
      RETURN Eval( ::bOnFunctionReturn, oReturn )
   ENDIF
RETURN .T.

METHOD OnFunctionProgress( nProgress, oData ) CLASS tRPCClient
   IF ::bOnFunctionProgress != NIL
      RETURN Eval( ::bOnFunctionProgress, nProgress, oData )
   ENDIF
RETURN .T.

