@echo off
REM
REM $Id: dll_b32.bat,v 1.7 2005/03/04 19:23:37 ronpinkas Exp $
REM
REM 旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
REM � This is a batch file to create harbour.dll 넴
REM � Please adjust envars accordingly           넴
REM 읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸�
REM  賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽賽

if not exist obj md obj
if not exist obj\dll md obj\dll
if not exist obj\dll\b32 md obj\dll\b32
if not exist lib\b32 md lib\b32

:BUILD

   make -fhrbdll.bc %1 %2 %3 > dll_b32.log
   if errorlevel 1 goto BUILD_ERR
   if "%1" == "clean" goto CLEAN
   if "%1" == "CLEAN" goto CLEAN

:BUILD_OK

if exist hdll.tmp del hdll.tmp
if exist bin\b32\harbour.lib copy bin\b32\harbour.lib lib > nul
if exist bin\b32\harbour.dll copy bin\b32\harbour.dll lib > nul
if exist bin\b32\harbour.dll copy bin\b32\harbour.dll tests > nul
if exist bin\b32\harbour.dll copy bin\b32\harbour.lib lib\b32 > nul

goto EXIT

:BUILD_ERR

notepad dll_b32.log
goto EXIT

:CLEAN
  if exist dll_b32.log del dll_b32.log

:EXIT
