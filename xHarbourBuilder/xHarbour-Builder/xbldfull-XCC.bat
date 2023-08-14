ECHO -----------
ECHO XCC VERSION
ECHO -----------

SET _XB_Compiler=xcc
SET _XB_NonDebug=-x\xHB -cXCC=\xHB -NoXbp
SET _XB_Debug=-x\xHB -cXCC=\xHB -NoXbp -Debug
SET _XB_Exe=%_XB_NonDebug%

SET _BUILD_MT=YES
SET _BUILD_DEBUG=YES

SET _BUILD_DEMO=YES
SET _BUILD_PERSONAL=YES
SET _BUILD_PROF=YES
SET _BUILD_XHB_EXE=YES
SET _BUILD_XHB_LIB=YES
SET _BUILD_XHB_DLL=YES
SET _BUILD_CORE=YES
SET _BUILD_XBUILD=YES
SET _BUILD_CONTRIB=YES
SET _BUILD_DMAIN_LIB=YES
SET _BUILD_VXHDLL=YES
SET _BUILD_HBZLIB=YES
SET _BUILD_XHBCOMM=YES
SET _BUILD_ADS=YES
SET _BUILD_BGD=YES
SET _BUILD_XDO_DLL=YES
SET _BUILD_IEGUI_LIB=NO
SET _BUILD_DBG_CLIENT=YES
SET _BUILD_DBG_SERVER=YES

SET _BUILD_TPROJECT_LIB=YES
SET _BUILD_XEDIT_LIB=YES

SET _BUILD_APOLLORDD=YES
SET _BUILD_WINAPI_LIB=YES
SET _BUILD_XBSCRIPT_LIB=YES
SET _BUILD_XBSCRIPT_DLL=NO
SET _BUILD_CT3COMM=YES
SET _BUILD_ACTIVEX=YES
SET _BUILD_RMDBFCDX=YES
SET _BUILD_WINCORE=YES
SET _BUILD_OLE=YES
SET _BUILD_SQLRDD=YES
SET _BUILD_BMDBFCDX=YES
SET _BUILD_REDBFCDX=YES
SET _BUILD_WINPRINT=YES
SET _BUILD_VR=YES

SET _BUILD_XBUILDW_AS=EXE
SET _BUILD_VXH_AS=EXE
SET _BUILD_XEDITW_AS=EXE
SET _BUILD_XPROMPT_AS=EXE
SET _BUILD_XDEBUGW_AS=EXE


IF "%XCC_MT%"=="YES"           SET _BUILD_MT=YES
IF "%XCC_MT%"=="NO"            SET _BUILD_MT=NO

IF "%XCC_DEMO%"=="YES"         SET _BUILD_DEMO=YES
IF "%XCC_DEMO%"=="NO"          SET _BUILD_DEMO=NO

IF "%XCC_PERSONAL%"=="YES"     SET _BUILD_PERSONAL=YES
IF "%XCC_PERSONAL%"=="NO"      SET _BUILD_PERSONAL=NO

IF "%XCC_PROF%"=="YES"         SET _BUILD_PROF=YES
IF "%XCC_PROF%"=="NO"          SET _BUILD_PROF=NO

IF "%XCC_XHB_EXE%"=="YES"      SET _BUILD_XHB_EXE=YES
IF "%XCC_XHB_EXE%"=="NO"       SET _BUILD_XHB_EXE=NO

IF "%XCC_XHB_LIB%"=="YES"      SET _BUILD_XHB_LIB=YES
IF "%XCC_XHB_LIB%"=="NO"       SET _BUILD_XHB_LIB=NO

IF "%XCC_XHB_DLL%"=="YES"      SET _BUILD_XHB_DLL=YES
IF "%XCC_XHB_DLL%"=="NO"       SET _BUILD_XHB_DLL=NO

IF "%XCC_CORELIBS%"=="YES"     SET _BUILD_CORE=YES
IF "%XCC_CORELIBS%"=="NO"      SET _BUILD_CORE=NO

IF "%XCC_XBUILD%"=="YES"       SET _BUILD_XBUILD=YES
IF "%XCC_XBUILD%"=="NO"        SET _BUILD_XBUILD=NO

IF "%XCC_CONTRIB%"=="YES"      SET _BUILD_CONTRIB=YES
IF "%XCC_CONTRIB%"=="NO"       SET _BUILD_CONTRIB=NO

IF "%XCC_DMAIN_LIB%"=="YES"    SET _BUILD_DMAIN_LIB=YES
IF "%XCC_DMAIN_LIB%"=="NO"     SET _BUILD_DMAIN_LIB=NO

IF "%XCC_VXHDLL%"=="YES"       SET _BUILD_VXHDLL=YES
IF "%XCC_VXHDLL%"=="NO"        SET _BUILD_VXHDLL=NO

IF "%XCC_SQLRDD%"=="YES"       SET _BUILD_SQLRDD=YES
IF "%XCC_SQLRDD%"=="NO"        SET _BUILD_SQLRDD=NO

IF "%XCC_BMDBFCDX%"=="YES"     SET _BUILD_BMDBFCDX=YES
IF "%XCC_BMDBFCDX%"=="NO"      SET _BUILD_BMDBFCDX=NO

IF "%XCC_REDBFCDX%"=="YES"     SET _BUILD_REDBFCDX=YES
IF "%XCC_REDBFCDX%"=="NO"      SET _BUILD_REDBFCDX=NO

IF "%XCC_HBZLIB%"=="YES"       SET _BUILD_HBZLIB=YES
IF "%XCC_HBZLIB%"=="NO"        SET _BUILD_HBZLIB=NO

IF "%XCC_CT3COMM%"=="YES"      SET _BUILD_CT3COMM=YES
IF "%XCC_CT3COMM%"=="NO"       SET _BUILD_CT3COMM=NO

IF "%XCC_XHBCOMM%"=="YES"      SET _BUILD_XHBCOMM=YES
IF "%XCC_XHBCOMM%"=="NO"       SET _BUILD_XHBCOMM=NO

IF "%XCC_APOLLORDD%"=="YES"    SET _BUILD_APOLLORDD=YES
IF "%XCC_APOLLORDD%"=="NO"     SET _BUILD_APOLLORDD=NO

IF "%XCC_WINAPI_LIB%"=="YES"   SET _BUILD_WINAPI_LIB=YES
IF "%XCC_WINAPI_LIB%"=="NO"    SET _BUILD_WINAPI_LIB=NO

IF "%XCC_TPROJECT_LIB%"=="YES" SET _BUILD_TPROJECT_LIB=YES
IF "%XCC_TPROJECT_LIB%"=="NO"  SET _BUILD_TPROJECT_LIB=NO

IF "%XCC_XEDIT_LIB%"=="YES"    SET _BUILD_XEDIT_LIB=YES
IF "%XCC_XEDIT_LIB%"=="NO"     SET _BUILD_XEDIT_LIB=NO

IF "%XCC_XDO_DLL%"=="YES"      SET _BUILD_XDO_DLL=YES
IF "%XCC_XDO_DLL%"=="NO"       SET _BUILD_XDO_DLL=NO

IF "%XCC_IEGUI_LIB%"=="YES"    SET _BUILD_IEGUI_LIB=YES
IF "%XCC_IEGUI_LIB%"=="NO"     SET _BUILD_IEGUI_LIB=NO

IF "%XCC_DBG_CLIENT%"=="YES"   SET _BUILD_DBG_CLIENT=YES
IF "%XCC_DBG_CLIENT%"=="NO"    SET _BUILD_DBG_CLIENT=NO

IF "%XCC_DBG_SERVER%"=="YES"   SET _BUILD_DBG_SERVER=YES
IF "%XCC_DBG_SERVER%"=="NO"    SET _BUILD_DBG_SERVER=NO

IF "%XCC_BGD%"=="YES"          SET _BUILD_BGD=YES
IF "%XCC_BGD%"=="NO"           SET _BUILD_BGD=NO

IF "%XCC_XBSCRIPT_LIB%"=="YES" SET _BUILD_XBSCRIPT_LIB=YES
IF "%XCC_XBSCRIPT_LIB%"=="NO"  SET _BUILD_XBSCRIPT_LIB=NO

IF "%XCC_XBSCRIPT_DLL%"=="YES" SET _BUILD_XBSCRIPT_DLL=YES
IF "%XCC_XBSCRIPT_DLL%"=="NO"  SET _BUILD_XBSCRIPT_DLL=NO

IF "%XCC_WINCORE%"=="YES"      SET _BUILD_WINCORE=YES
IF "%XCC_WINCORE%"=="NO"       SET _BUILD_WINCORE=NO

IF "%XCC_WINPRINT%"=="YES"     SET _BUILD_WINPRINT=YES
IF "%XCC_WINPRINT%"=="NO"      SET _BUILD_WINPRINT=NO

IF "%XCC_VR%"=="YES"           SET _BUILD_VR=YES
IF "%XCC_VR%"=="NO"            SET _BUILD_VR=NO

IF "%XCC_DEBUG%"=="YES"        SET _XB_Exe=%_XB_Debug%
IF "%XCC_DEBUG%"=="NO"         SET _XB_Exe=%_XB_NonDebug%
IF "%XCC_DEBUG%"=="YES"        SET _BUILD_DEBUG=YES
IF "%XCC_DEBUG%"=="NO"         SET _BUILD_DEBUG=NO

IF "%XCC_XBUILDW_AS%"=="DLL"   SET _BUILD_XBUILDW_AS=DLL
IF "%XCC_XBUILDW_AS%"=="EXE"   SET _BUILD_XBUILDW_AS=EXE
IF "%XCC_XBUILDW_AS%"=="NONE"  SET _BUILD_XBUILDW_AS=NONE

IF "%XCC_VXH_AS%"=="DLL"       SET _BUILD_VXH_AS=DLL
IF "%XCC_VXH_AS%"=="EXE"       SET _BUILD_VXH_AS=EXE
IF "%XCC_VXH_AS%"=="NONE"      SET _BUILD_VXH_AS=NONE

IF "%XCC_XEDITW_AS%"=="DLL"    SET _BUILD_XEDITW_AS=DLL
IF "%XCC_XEDITW_AS%"=="EXE"    SET _BUILD_XEDITW_AS=EXE
IF "%XCC_XEDITW_AS%"=="NONE"   SET _BUILD_XEDITW_AS=NONE

IF "%XCC_XDEBUGW_AS%"=="DLL"   SET _BUILD_XDEBUGW_AS=DLL
IF "%XCC_XDEBUGW_AS%"=="EXE"   SET _BUILD_XDEBUGW_AS=EXE
IF "%XCC_XDEBUGW_AS%"=="NONE"  SET _BUILD_XDEBUGW_AS=NONE

IF "%XCC_XPROMPT_AS%"=="DLL"   SET _BUILD_XPROMPT_AS=DLL
IF "%XCC_XPROMPT_AS%"=="EXE"   SET _BUILD_XPROMPT_AS=EXE
IF "%XCC_XPROMPT_AS%"=="NONE"  SET _BUILD_XPROMPT_AS=NONE


SET _XHB_LIB=\xHB\lib
SET _XHB_BIN=\xHB\bin
SET _XHB_DLL=\xHB\dll

ECHO ON