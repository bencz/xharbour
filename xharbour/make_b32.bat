@echo off
rem ============================================================================
rem
rem $Id: make_b32.bat,v 1.34 2008/04/29 12:34:56 enricomaria Exp $
rem
rem FILE: make_b32.bat
rem BATCH FILE FOR BORLAND C++
rem
rem This is Generic File, do not change it. If you should require your own build
rem version, changes should only be made on your local copy.(AJ:2008-04-26)
rem
rem ============================================================================

if "%BCCDIR%" == "" SET BCCDIR=C:\BORLAND\BCC58

SET CC_DIR=C:\BORLAND\BCC58
SET BISON_DIR=C:\BISON\BIN
SET SUB_DIR=b32
SET HB_GT_LIB=$(GTWIN_LIB)

SET _PATH=%PATH%
SET PATH=%CC_DIR%\BIN;%BISON_DIR%;%PATH%

rem ============================================================================
rem The followings should never change
rem Do not hard-code in makefile because there are needed for clean build
rem ============================================================================
SET LIBEXT=.lib
SET OBJEXT=.obj
SET DIR_SEP=\
REM SET LIBPREFIX=
rem ============================================================================

if "%1" == "clean" goto CLEAN
if "%1" == "CLEAN" goto CLEAN

   @CALL MDIR.BAT

:BUILD
   SET HB_MT=
   SET HB_MT_DIR=
   make -l -fmakefile.bc %1 %2 %3 >make_b32.log
   if errorlevel 1 goto BUILD_ERR

   SET HB_MT=mt
   SET HB_MT_DIR=\mt
   make -l -DHB_THREAD_SUPPORT -fmakefile.bc %1 %2 %3 >>make_b32.log
   if errorlevel 1 goto BUILD_ERR

:BUILD_OK
   xcopy bin\%SUB_DIR%\*.exe bin\*.* /D /Y > nul
   REM xcopy bin\%SUB_DIR%\*.tds bin\*.* /D /Y > nul
   xcopy lib\%SUB_DIR%\*.lib lib\*.* /D /Y > nul
   if exist lib\%SUB_DIR%\*.bak del lib\%SUB_DIR%\*.bak
   goto EXIT

:BUILD_ERR
   notepad make_b32.log
   goto EXIT

:CLEAN
   @CALL mdir.bat clean
   IF EXIST make_b32.log DEL make_b32.log

:EXIT
   SET CC_DIR=
   SET BISON_DIR=
   SET SUB_DIR=
   SET HB_GT_LIB=
   SET PATH=%_PATH%
   SET _PATH=
   SET LIBEXT=
   SET OBJEXT=
   SET DIR_SEP=
   REM SET LIBPREFIX=
   IF NOT "%HB_MT%"=="" SET HB_MT=
   IF NOT "%HB_MT_DIR%"=="" SET HB_MT_DIR=
