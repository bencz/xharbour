@ECHO OFF

SET _PRESET_PATH=%PATH%
SET _PRESET_INCLUDE=%INCLUDE%
SET _PRESET_LIB=%LIB%
SET _PRESET_CFLAGS=%CFLAGS%
SET _PRESET_LFLAGS=%LFLAGS%

:FIND_VC
   IF EXIST "%ProgramFiles%\Microsoft Visual Studio 9.0\vc"  GOTO SET_VC2008
   IF EXIST "%ProgramFiles%\Microsoft Visual Studio 8\vc"    GOTO SET_VC2005
   IF EXIST "%ProgramFiles%\Microsoft Visual Studio 2003\vc" GOTO SET_VC2003
   IF EXIST "%ProgramFiles%\Microsoft Visual Studio\vc8"     GOTO SET_VC6
   GOTO NONE

:SET_VC2008
   CALL "%ProgramFiles%\Microsoft Visual Studio 9.0\vc\vcvarsall.bat"
   GOTO READY

:SET_VC2005
   CALL "%ProgramFiles%\Microsoft Visual Studio 8\vc\vcvarsall.bat"
   GOTO READY

:SET_VC2003
   CALL "%ProgramFiles%\Microsoft Visual Studio .NET 2003\VC7\vcvarsall.bat"
   GOTO READY

:SET_VC6
   CALL "%ProgramFiles%\Microsoft Visual Studio\VC98\vcvarsall.bat"
   GOTO READY

:NONE

:READY
SET PELLESCDIR=%ProgramFiles%\PellesC
SET PATH=%PATH%;"%VS90COMNTOOLS%bin";"%PELLESCDIR%\bin"

SET _PRESET_CFLAGS=%CFLAGS%
SET CFLAGS=/Od /EHsc /RTC1 /MTd /Gs /GS /Gy /GR /Zi /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE

SET _PRESET_LFLAGS=%LFLAGS%
SET LFLAGS=-DEBUG -DEBUGTYPE:CV

IF NOT EXIST MSObj MD MSObj
IF NOT EXIST Obj MD Obj

NMAKE -S > Log
IF ERRORLEVEL 0 GOTO :NO_ERROR

ECHO !!! ERROR BUILDING xLink !!!
PAUSE

:NO_ERROR
TYPE Log
xcopy xlink.exe \xhb\bin /d /r /y

SET PATH=%_PRESET_PATH%
SET INCLUDE=%_PRESET_INCLUDE%
SET LIB=%_PRESET_LIB%
SET CFLAGS=%_PRESET_CFLAGS%
SET LFLAGS=%_PRESET_LFLAGS%

SET _PRESET_PATH=
SET _PRESET_INCLUDE=
SET _PRESET_LIB=
SET _PRESET_CFLAGS=
SET _PRESET_LFLAGS=
