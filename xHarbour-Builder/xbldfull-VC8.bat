ECHO -----------
ECHO VC8 VERSION
ECHO -----------

SET _XB_Compiler=vc8
SET _XB_NonDebug=-NoXbp
SET _XB_Debug=-NoXbp -Debug
SET _XB_Exe=%_XB_NonDebug%

SET _BUILD_MT=YES

SET _BUILD_DEMO=NO
SET _BUILD_PERSONAL=NO
SET _BUILD_XHB.EXE=NO
SET _BUILD_XHB.LIB=NO
SET _BUILD_XHB.DLL=NO
SET _BUILD_CORE=NO
SET _BUILD_XBUILD=NO
SET _BUILD_CONTRIB=NO
SET _BUILD_DMAIN.LIB=NO
SET _BUILD_VXHDLL=YES
SET _BUILD_HBZLIB=YES
SET _BUILD_XHBCOMM=YES
SET _BUILD_ADS=YES
SET _BUILD_XDO.DLL=YES
SET _BUILD_IEGUI.LIB=NO
SET _BUILD_DBG_CLIENT=YES

SET _BUILD_TPROJECT.LIB=YES
SET _BUILD_XEDIT.LIB=YES

SET _BUILD_APOLLORDD=YES
SET _BUILD_WINAPI.LIB=YES
SET _BUILD_XBSCRIPT.LIB=YES
SET _BUILD_CT3COMM=YES
SET _BUILD_ACTIVEX=YES
SET _BUILD_RMDBFCDX=YES
SET _BUILD_WINCORE=YES
SET _BUILD_OLE=YES
SET _BUILD_SQLRDD=YES

SET _BUILD_XBUILDW_AS=EXE
SET _BUILD_VXH_AS=EXE
SET _BUILD_XEDITW_AS=DLL
SET _BUILD_XPROMPT_AS=DLL
SET _BUILD_XDEBUGW_AS=DLL


IF "%VC8_MT%"=="YES"          SET _BUILD_MT=YES
IF "%VC8_MT%"=="NO"           SET _BUILD_MT=NO

IF "%VC8_DEMO%"=="YES"        SET _BUILD_DEMO=YES
IF "%VC8_DEMO%"=="NO"         SET _BUILD_DEMO=NO

IF "%VC8_PERSONAL%"=="YES"    SET _BUILD_PERSONAL=YES
IF "%VC8_PERSONAL%"=="NO"     SET _BUILD_PERSONAL=NO

IF "%VC8_XHB.EXE%"=="YES"     SET _BUILD_XHB.EXE=YES
IF "%VC8_XHB.EXE%"=="NO"      SET _BUILD_XHB.EXE=NO

IF "%VC8_XHB.LIB%"=="YES"     SET _BUILD_XHB.LIB=YES
IF "%VC8_XHB.LIB%"=="NO"      SET _BUILD_XHB.LIB=NO

IF "%VC8_XHB.DLL%"=="YES"     SET _BUILD_XHB.DLL=YES
IF "%VC8_XHB.DLL%"=="NO"      SET _BUILD_XHB.DLL=NO

IF "%VC8_CORELIBS%"=="YES"    SET _BUILD_CORE=YES
IF "%VC8_CORELIBS%"=="NO"     SET _BUILD_CORE=NO

IF "%VC8_XBUILD%"=="YES"      SET _BUILD_XBUILD=YES
IF "%VC8_XBUILD%"=="NO"       SET _BUILD_XBUILD=NO

IF "%VC8_CONTRIB%"=="YES"     SET _BUILD_CONTRIB=YES
IF "%VC8_CONTRIB%"=="NO"      SET _BUILD_CONTRIB=NO

IF "%VC8_DMAIN.LIB%"=="YES"   SET _BUILD_DMAIN.LIB=YES
IF "%VC8_DMAIN.LIB%"=="NO"    SET _BUILD_DMAIN.LIB=NO

IF "%VC8_VXHDLL%"=="YES"      SET _BUILD_VXHDLL=YES
IF "%VC8_VXHDLL%"=="NO"       SET _BUILD_VXHDLL=NO

IF "%VC8_SQLRDD%"=="YES"      SET _BUILD_SQLRDD=YES
IF "%VC8_SQLRDD%"=="NO"       SET _BUILD_SQLRDD=NO

IF "%VC8_HBZLIB%"=="YES"      SET _BUILD_HBZLIB=YES
IF "%VC8_HBZLIB%"=="NO"       SET _BUILD_HBZLIB=NO

IF "%VC8_CT3COMM%"=="YES"     SET _BUILD_CT3COMM=YES
IF "%VC8_CT3COMM%"=="NO"      SET _BUILD_CT3COMM=NO

IF "%VC8_XHBCOMM%"=="YES"      SET _BUILD_XHBCOMM=YES
IF "%VC8_XHBCOMM%"=="NO"       SET _BUILD_XHBCOMM=NO

IF "%VC8_APOLLORDD%"=="YES"    SET _BUILD_APOLLORDD=YES
IF "%VC8_APOLLORDD%"=="NO"     SET _BUILD_APOLLORDD=NO

IF "%VC8_WINAPI.LIB%"=="YES"   SET _BUILD_WINAPI.LIB=YES
IF "%VC8_WINAPI.LIB%"=="NO"    SET _BUILD_WINAPI.LIB=NO

IF "%VC8_TPROJECT.LIB%"=="YES" SET _BUILD_TPROJECT.LIB=YES
IF "%VC8_TPROJECT.LIB%"=="NO"  SET _BUILD_TPROJECT.LIB=NO

IF "%VC8_XEDIT.LIB%"=="YES"    SET _BUILD_XEDIT.LIB=YES
IF "%VC8_XEDIT.LIB%"=="NO"     SET _BUILD_XEDIT.LIB=NO

IF "%VC8_XDO.DLL%"=="YES"      SET _BUILD_XDO.DLL=YES
IF "%VC8_XDO.DLL%"=="NO"       SET _BUILD_XDO.DLL=NO

IF "%VC8_IEGUI.LIB%"=="YES"    SET _BUILD_IEGUI.LIB=YES
IF "%VC8_IEGUI.LIB%"=="NO"     SET _BUILD_IEGUI.LIB=NO

IF "%VC8_DBG_CLIENT%"=="YES"   SET _BUILD_DBG_CLIENT=YES
IF "%VC8_DBG_CLIENT%"=="NO"    SET _BUILD_DBG_CLIENT=NO

IF "%VC8_DEBUG%"=="YES"       SET _XB_Exe=%_XB_Debug%
IF "%VC8_DEBUG%"=="NO"        SET _XB_Exe=%_XB_NonDebug%


IF "%VC8_XBUILDW_AS%"=="DLL"  SET _BUILD_XBUILDW_AS=DLL
IF "%VC8_XBUILDW_AS%"=="EXE"  SET _BUILD_XBUILDW_AS=EXE
IF "%VC8_XBUILDW_AS%"=="NONE" SET _BUILD_XBUILDW_AS=NONE

IF "%VC8_VXH_AS%"=="DLL"      SET _BUILD_VXH_AS=DLL
IF "%VC8_VXH_AS%"=="EXE"      SET _BUILD_VXH_AS=EXE
IF "%VC8_VXH_AS%"=="NONE"     SET _BUILD_VXH_AS=NONE

IF "%VC8_XEDITW_AS%"=="DLL"   SET _BUILD_XEDITW_AS=DLL
IF "%VC8_XEDITW_AS%"=="EXE"   SET _BUILD_XEDITW_AS=EXE
IF "%VC8_XEDITW_AS%"=="NONE"  SET _BUILD_XEDITW_AS=NONE

IF "%VC8_XDEBUGW_AS%"=="DLL"  SET _BUILD_XDEBUGW_AS=DLL
IF "%VC8_XDEBUGW_AS%"=="EXE"  SET _BUILD_XDEBUGW_AS=EXE
IF "%VC8_XDEBUGW_AS%"=="NONE" SET _BUILD_XDEBUGW_AS=NONE

IF "%VC8_XPROMPT_AS%"=="DLL"  SET _BUILD_XPROMPT_AS=DLL
IF "%VC8_XPROMPT_AS%"=="EXE"  SET _BUILD_XPROMPT_AS=EXE
IF "%VC8_XPROMPT_AS%"=="NONE" SET _BUILD_XPROMPT_AS=NONE


SET _XHB_LIB=\xhb\lib\%_XB_Compiler%
SET _XHB_BIN=\xhb\bin\%_XB_Compiler%
SET _XHB_DLL=\xhb\dll\%_XB_Compiler%

SET _XHARBOUR_ROOT=\xharbour
SET _XHARBOUR_LIB=\xharbour\lib\vc
SET _XHARBOUR_BIN=\xharbour\bin\vc
SET _XHARBOUR_XBP=\xbp\vc8
SET _XHARBOUR_XBP_DEMO=\xbp\vc8\demo
SET _XHARBOUR_XBP_PERS=\xbp\vc8\personal

ECHO ON
