@echo off

REM
REM $Id: dll_vc.bat,v 1.7 2005/03/31 19:51:24 andijahja Exp $
REM
REM 旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
REM � This is a batch file to create harbour.dll 넴
REM 읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸�
REM  賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽

if not exist obj md obj
if not exist obj\dll md obj\dll
if not exist obj\dll\vc md obj\dll\vc
if not exist obj\dll\vc\bin md obj\dll\vc\bin

   nmake /f hrbdll.vc %1 %2 %3 > dll_vc.log
   if errorlevel 1 goto BUILD_ERR
   if "%1" == "clean" goto exit
   if "%1" == "CLEAN" goto exit

   copy lib\vc\harbour.lib lib > nul
   copy lib\vc\harbour.exp lib > nul
   copy bin\vc\harbour.dll lib > nul
   copy bin\vc\harbour.dll tests > nul

   goto EXIT

:BUILD_ERR
   notepad dll_vc.log

:EXIT
