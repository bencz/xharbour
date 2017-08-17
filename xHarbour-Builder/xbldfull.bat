@ECHO OFF

REM --> Save Environment
    SET _PATH=%PATH%
    SET _INCLUDE=%INCLUDE%
    SET _LIB=%LIB%

REM --> Set Environment
    IF "%PROG_PATH%"=="" SET PROG_PATH=%ProgramFiles%

    SET INCLUDE=
    SET LIB=

    IF EXIST xbuild.ini         DEL xBuild.ini > NUL:
    IF EXIST xbuild.windows.ini DEL xBuild.Windows.ini > NUL:
    

REM --> Cleanup for -ALL
    IF "%1"=="-all" GOTO CLEAN_ALL
    IF "%1"=="-All" GOTO CLEAN_ALL
    IF "%1"=="-ALL" GOTO CLEAN_ALL

    GOTO AFTER_CLEAN_ALL

    :CLEAN_ALL

    
    IF "%XBUILD_XCC%"=="YES" DEL \xHB\bin\*.exe /s
    IF "%XBUILD_XCC%"=="YES" DEL \xHB\bin\*.map /s
    IF "%XBUILD_XCC%"=="YES" DEL \xHB\bin\*.exp /s
    IF "%XBUILD_XCC%"=="YES" DEL \xHB\bin\*.dll /s
    IF "%XBUILD_XCC%"=="YES" RD  \xHB\bin\Demo /s /q
    IF "%XBUILD_XCC%"=="YES" RD  \xHB\bin\Personal /s /q
    IF "%XBUILD_XCC%"=="YES" RD  \xHB\bin\Professional /s /q
    
    IF "%XBUILD_XCC%"=="YES" DEL \xHB\dll\*.dll /s
    IF "%XBUILD_XCC%"=="YES" RD  \xHB\dll\Ads /s /q
    IF "%XBUILD_XCC%"=="YES" RD  \xHB\dll\ApolloRDD /s /q
    IF "%XBUILD_XCC%"=="YES" RD  \xHB\dll\SQLRDD /s /q
    
    IF "%XBUILD_XCC%"=="YES" DEL \xHB\lib\*.lib /s
    IF "%XBUILD_XCC%"=="YES" RD  \xHB\lib\Demo /s /q
    IF "%XBUILD_XCC%"=="YES" RD  \xHB\lib\Personal /s /q
    IF "%XBUILD_XCC%"=="YES" RD  \xHB\lib\Professional /s /q

    
    IF "%XBUILD_VC8%"=="YES" RD \xHB\bin\vc8 /s /q
    IF "%XBUILD_VC8%"=="YES" MD \xHB\bin\vc8
    IF "%XBUILD_VC8%"=="YES" RD \xHB\dll\vc8 /s /q
    IF "%XBUILD_VC8%"=="YES" RD \xHB\lib\vc8 /s /q
    IF "%XBUILD_VC8%"=="YES" MD \xHB\dll\vc8
    IF "%XBUILD_VC8%"=="YES" MD \xHB\lib\vc8

    DEL \xharbour\bin\xCC.*
    DEL \xharbour\bin\xRC.*
    DEL \xharbour\bin\xLib.exe
    DEL \xharbour\bin\xLink.exe
    DEL \xharbour\bin\xHB.exe

    IF EXIST \xHB\include        RD \xHB\include /s /q
    IF EXIST \xharbour\c_include RD \xharbour\c_include /s /q
    IF EXIST \xharbour\c_lib     RD \xharbour\c_lib /s /q

    REM We will use only \xharbour\lib\vc or bc5 so make sure that's the case
    REM when done using make_vc all or make_b32 all will quickly copy them back!
    DEL \xharbour\lib\*.lib
    COPY \xharbour\lib\vc\*.lib \xharbour\lib\*.lib

    REM Clean up XBP
    RD \xbp /S /Q

    :AFTER_CLEAN_ALL



REM --> Copy files

   IF "%XBUILD_VC8%"=="NO" GOTO N84
      MD \xHB\bin\vc8
      MD \xHB\dll\vc8
      MD \xHB\lib\vc8
   :N84

   IF NOT EXIST "\xHB\bin"         MD "\xHB\bin"
   IF NOT EXIST "\xHB\lib"         MD "\xHB\lib"
   IF NOT EXIST "\xHB\dll"         MD "\xHB\dll"
   IF NOT EXIST "\xHB\include\w32" MD "\xHB\include\w32"

    REM ** SQLRDD **
    IF NOT EXIST \xHB\dll\SQLRDD MD \xHB\dll\SQLRDD
    XCOPY \xharbour.com\xHarbour-SQLRDD\dll\*.dll      \xHB\dll\SQLRDD /d /y
    XCOPY \xharbour.com\xHarbour-SQLRDD\lib\*.lib      \xHB\lib /d /y
    XCOPY \xHarbour.com\xHarbour-SQLRDD\include        \xHB\include /d /y /i
    XCOPY \xHarbour.com\xHarbour-SQLRDD\source\*.ch    \xHB\include /d /y /i
    XCOPY \xHarbour.com\xHarbour-SQLRDD\source\mysql.h \xHB\include /d /y
    IF EXIST \xHB\include\sqlrdd.xns DEL \xHB\include\sqlrdd.xns /Q


    REM ** ADS **
    REM HB_DIR_ADS is the ONE place ace32.dll SHOULD be in.
    IF NOT EXIST \xHB\dll\ADS MD \xHB\dll\ADS
    XCOPY "%HB_DIR_ADS%\Ace32.dll"       \xHB\dll\ADS /d /y
    XCOPY "%HB_DIR_ADS%\AdsLoc32.dll"    \xHB\dll\ADS /d /y
    XCOPY "%HB_DIR_ADS%\AXCws32.dll"     \xHB\dll\ADS /d /y
    XCOPY "%HB_DIR_ADS%\Ansi.chr"        \xHB\dll\ADS /d /y
    XCOPY "%HB_DIR_ADS%\Extend.chr"      \xHB\dll\ADS /d /y
    XCOPY "%HB_DIR_ADS%\AdsLocal.cfg"    \xHB\dll\ADS /d /y
    XCOPY \xHarbour\contrib\rdd_ads\*.ch \xHB\include /d /y /i

    
    REM ** FreeImage **
    IF "%XBUILD_VC8%"=="NO" GOTO N114
       IF NOT EXIST \xHB\lib\vc8 MD \xHB\lib\vc8
       XCOPY \xHarbour.com\FreeImage\FreeImage.lib    \xHB\lib\vc8  /d /y /i
    :N114
    XCOPY \xHarbour.com\FreeImage\FreeImage.lib    \xHB\lib\     /d /y /i
    XCOPY \xHarbour.com\FreeImage\FreeImage.dll    \xHB\dll\     /d /y /i
    XCOPY \xHarbour\contrib\FreeImage\include\*.ch \xHB\include\ /d /y /i
    XCOPY \xHarbour\contrib\FreeImage\include\*.h  \xHB\include\ /d /y /i


    REM ** ApolloRDD **
    IF NOT EXIST \xHB\dll\ApolloRDD MD \xHB\dll\ApolloRDD
    XCOPY \xHarbour.com\xHarbour-ApolloRDD\dll\*.dll \xHB\dll\ApolloRDD /d /y /i
    XCOPY \xHarbour.com\xHarbour-ApolloRDD\*.h  \xHB\include /d /y /i
    XCOPY \xHarbour.com\xHarbour-ApolloRDD\*.ch \xHB\include /d /y /i


    REM ** BGD.DLL **
    XCOPY \xHarbour.com\xHarbour-Builder\BGD.DLL \xHB\dll  /d /y /i


    XCOPY \xHarbour\include\*.api                       \xHB\include /d /y /i
    XCOPY \xHarbour\include\*.ch                        \xHB\include /d /y /i
    XCOPY \xHarbour\include\*.h                         \xHB\include /d /y /i
    XCOPY \xHarbour\include\*.c                         \xHB\include /d /y /i

    XCOPY \xHarbour\source\rtl\pcre\*.h                 \xHB\include /d /y /i
    XCOPY \xHarbour\source\rtl\pcre\*.generic           \xHB\include /d /y /i
    XCOPY \xHarbour\include\hbverbld.h                  \xHB\include /d /y /i
    XCOPY \xHarbour\contrib\gd\include                  \xHB\include /d /y /i

    XCOPY \xHarbour.com\xHarbour-Builder\include        \xHB\include /d /y /i
    XCOPY \xHarbour.com\xHarbour-ActiveX\ole.ch         \xHB\include\w32 /d /y
    XCOPY \xHarbour.com\Visual-xHarbour\library\include \xHB\include\w32 /d /y /i
    XCOPY \xHarbour.com\IEGui\iegui.ch                  \xHB\include /d /y
    XCOPY \xHarbour.com\xHarbour-Builder\xcc*.lib       \xHarbour.com\xHarbour-XCC\xcc\xcc*.lib /d /y /i


    IF EXIST \xHarbour\include\Ado.ch      DEL \xHarbour\include\Ado.ch
    IF EXIST \xHarbour\include\Colors.ch   DEL \xHarbour\include\Colors.ch
    IF EXIST \xHarbour\include\CommCtrl.ch DEL \xHarbour\include\CommCtrl.ch
    IF EXIST \xHarbour\include\CommDlg.ch  DEL \xHarbour\include\CommDlg.ch
    IF EXIST \xHarbour\include\Debug.ch.ch DEL \xHarbour\include\Debug.ch.ch
    IF EXIST \xHarbour\include\Import.ch   DEL \xHarbour\include\Import.ch
    IF EXIST \xHarbour\include\Ole.ch      DEL \xHarbour\include\Ole.ch
    IF EXIST \xHarbour\include\RichEdit.ch DEL \xHarbour\include\RichEdit.ch
    IF EXIST \xHarbour\include\SQLTypes.ch DEL \xHarbour\include\SQLTypes.ch
    IF EXIST \xHarbour\include\VXH.ch      DEL \xHarbour\include\VXH.ch
    IF EXIST \xHarbour\include\What32.ch   DEL \xHarbour\include\What32.ch
    IF EXIST \xHarbour\include\WinApi.ch   DEL \xHarbour\include\WinApi.ch
    IF EXIST \xHarbour\include\WinGDI.ch   DEL \xHarbour\include\WinGDI.ch
    IF EXIST \xHarbour\include\WinInet.ch  DEL \xHarbour\include\WinInet.ch
    IF EXIST \xHarbour\include\WinReg.ch   DEL \xHarbour\include\WinReg.ch
    IF EXIST \xHarbour\include\WinStruc.ch DEL \xHarbour\include\WinStruc.ch
    IF EXIST \xHarbour\include\WinUser.ch  DEL \xHarbour\include\WinUser.ch

    IF EXIST \xHB\include\Ado.ch           DEL \xHB\include\Ado.ch
    IF EXIST \xHB\include\Colors.ch        DEL \xHB\include\Colors.ch
    IF EXIST \xHB\include\CommCtrl.ch      DEL \xHB\include\CommCtrl.ch
    IF EXIST \xHB\include\CommDlg.ch       DEL \xHB\include\CommDlg.ch
    IF EXIST \xHB\include\Debug.ch.ch      DEL \xHB\include\Debug.ch.ch
    IF EXIST \xHB\include\Import.ch        DEL \xHB\include\Import.ch
    IF EXIST \xHB\include\Ole.ch           DEL \xHB\include\Ole.ch
    IF EXIST \xHB\include\RichEdit.ch      DEL \xHB\include\RichEdit.ch
    IF EXIST \xHB\include\SQLTypes.ch      DEL \xHB\include\SQLTypes.ch
    IF EXIST \xHB\include\VXH.ch           DEL \xHB\include\VXH.ch
    IF EXIST \xHB\include\What32.ch        DEL \xHB\include\What32.ch
    IF EXIST \xHB\include\WinApi.ch        DEL \xHB\include\WinApi.ch
    IF EXIST \xHB\include\WinGDI.ch        DEL \xHB\include\WinGDI.ch
    IF EXIST \xHB\include\WinInet.ch       DEL \xHB\include\WinInet.ch
    IF EXIST \xHB\include\WinReg.ch        DEL \xHB\include\WinReg.ch
    IF EXIST \xHB\include\WinStruc.ch      DEL \xHB\include\WinStruc.ch
    IF EXIST \xHB\include\WinUser.ch       DEL \xHB\include\WinUser.ch
    IF EXIST \xHB\include\W32\WinTypes.ch  DEL \xHB\include\W32\WinTypes.ch


REM  ===============================================
REM  ===============================================
REM  ===============================================
REM  ===============================================


IF "%XBUILD_XCC%"=="YES" CALL \xHarbour.com\xHarbour-Builder\xbldfull-XCC.bat %1
IF "%XBUILD_XCC%"=="YES" CALL \xHarbour.com\xHarbour-Builder\xbldfull2.bat %1

IF "%XBUILD_VC8%"=="YES" CALL \xHarbour.com\xHarbour-Builder\xbldfull-VC8.bat %1
IF "%XBUILD_VC8%"=="YES" CALL \xHarbour.com\xHarbour-Builder\xbldfull2.bat %1

REM  ===============================================
REM  ===============================================
REM  ===============================================
REM  ===============================================


REM  ===============================================
ECHO Restore Environment
REM  ===============================================
    SET PATH=%_PATH%
    SET INCLUDE=%_INCLUDE%
    SET LIB=%_LIB%

    SET _PATH=
    SET _INCLUDE=
    SET _LIB=

    SET _XB_NonDebug=
    SET _XB_Debug=
    SET _XB_Exe=

    SET _XB_Compiler=

    SET _XHB_LIB=
    SET _XHB_DLL=

    SET VC8_MT=
    SET VC8_DEMO=
    SET VC8_PERSONAL=
    SET VC8_PROF=
    SET VC8_XHB_EXE=
    SET VC8_XHB_LIB=
    SET VC8_XHB_DLL=
    SET VC8_CORELIBS=
    SET VC8_CONTRIB=
    SET VC8_DMAIN_LIB=
    SET VC8_VXHDLL=
    SET VC8_XBUILDW_AS=
    SET VC8_VXH_AS=
    SET VC8_XEDITW_AS=
    SET VC8_XDEBUGW_AS=
    SET VC8_XPROMPT_AS=
    SET VC8_DEBUG=

    SET XCC_MT=
    SET XCC_DEMO=
    SET XCC_PERSONAL=
    SET XCC_PROF=
    SET XCC_XHB_EXE=
    SET XCC_XHB_LIB=
    SET XCC_XHB_DLL=
    SET XCC_CORELIBS=
    SET XCC_CONTRIB=
    SET XCC_DMAIN_LIB=
    SET XCC_VXHDLL=
    SET XCC_XBUILDW_AS=
    SET XCC_VXH_AS=
    SET XCC_XEDITW_AS=
    SET XCC_XDEBUGW_AS=
    SET XCC_XPROMPT_AS=
    SET XCC_DEBUG=

    SET _BUILD_MT=
    SET _BUILD_DEMO=
    SET _BUILD_PERSONAL=
    SET _BUILD_PROF=
    SET _BUILD_XHB_EXE=
    SET _BUILD_XHB_LIB=
    SET _BUILD_XHB_DLL=
    SET _BUILD_CORE=
    SET _BUILD_CONTRIB=
    SET _BUILD_DMAIN_LIB=
    SET _BUILD_VXHDLL=
    SET _BUILD_XBUILDW_AS=
    SET _BUILD_VXH_AS=
    SET _BUILD_XEDITW_AS=
    SET _BUILD_XDEBUGW_AS=
    SET _BUILD_XPROMPT_AS=

    SET _BUILD_ACTIVEX=
    SET _BUILD_ADS=
    SET _BUILD_APOLLORDD=
    SET _BUILD_CT3COMM=
    SET _BUILD_DBG_CLIENT=
    SET _BUILD_HBZLIB=
    SET _BUILD_OLE=
    SET _BUILD_RMDBFCDX=
    SET _BUILD_SQLRDD=
    SET _BUILD_BMDBFCDX=
    SET _BUILD_REDEBFCD=
    SET _BUILD_TPROJECT_LIB=
    SET _BUILD_WINAPI_LIB=
    SET _BUILD_WINCORE=
    SET _BUILD_XBSCRIPT_LIB=
    SET _BUILD_XBUILD=
    SET _BUILD_XDO_DLL=
    SET _BUILD_XEDIT_LIB=
    SET _BUILD_XHBCOMM=

    SET _XB_Exe=

    CD \xHarbour.com\xHarbour-Builder

:Done1

