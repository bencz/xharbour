:: Copyright (c) 2023 Ron Pinkas (ron@ronpinkas.com)
:: 
:: Permission is granted, free of charge, to any person obtaining a copy 
:: of this software and associated documentation files, to deal in the 
:: Software without restriction, including rights to use, copy, modify, 
:: merge, publish, distribute, sublicense, and/or sell copies of the Software, 
:: and to permit persons to whom the Software is furnished to do so, 
:: subject to the terms of the MIT License.
@ECHO OFF

IF "%scriptName%" NEQ "" SET "scriptName=%~n0" && ECHO    *** started [%~f0] >> %~dp0\functions.log
IF "%scriptName%" == ""  SET "scriptName=%~n0" && ECHO *** START [%~f0] > %~dp0\functions.log

REM First check if already set.
IF "%CC%" NEQ "" GOTO CHECK_CC

:SET_CC
   REM Quick check if CAN be quickly set (CC_DIR and PATH might be set).
   CALL %~dp0found_cc.bat cl
      IF ERRORLEVEL 99 GOTO FIND_EXIT_99
      IF ERRORLEVEL  2 GOTO FIND_EXIT_2
      IF ERRORLEVEL  1 GOTO SET_C_COMPILER
      IF ERRORLEVEL  0 GOTO FOUND
      ECHO [%~f0] - (%ERRORLEVEL%) Unexpected error!
      GOTO FIND_EXIT_99

:CHECK_CC
   REM Will force a SWITCH_CC if CC is NOT cl
   IF "%CC%" NEQ "cl" GOTO SET_CC

   REM Quick check if the compiler is already set.
   CALL %~dp0found_cc.bat %CC%
      IF ERRORLEVEL 99 GOTO FIND_EXIT_99
      IF ERRORLEVEL  2 GOTO FIND_EXIT_2
      IF ERRORLEVEL  1 GOTO SET_C_COMPILER
      IF ERRORLEVEL  0 GOTO FOUND
      ECHO "[%~f0] - (%ERRORLEVEL%) Unexpected error!" 
      GOTO FIND_EXIT_99
   
REM The Entry point for FIRST run.
:SET_C_COMPILER
   ver > nul REM Reset ERRORLEVEL

   IF "%CC%" == "" SET "CC=cl"
   IF "%RC%" == "" SET "RC=rc"
   SET LD=

   REM Check if CC_DIR is set by user and conttinue to DIR_SET | FIND_C_COMPILER | NOT_FOUND
   IF "%CC_DIR%" NEQ "" GOTO CHECK_CC_DIR

   REM Fall through to FIND_C_COMPILER

:FIND_C_COMPILER
   ECHO Searching for Microsoft C++...
   SET "CC_DIR="

   REM MSC Specific!
   IF "%VSINSTALLDIR%" == "" GOTO SKIP_VSINSTALLDIR
      REM Fall through to SET_VSINSTALLER 
      :SET_VSINSTALLER
         SET "CC_DIR=%VSINSTALLDIR%"
         GOTO DIR_SET

   :SKIP_VSINSTALLDIR
   REM Check if the compiler is in the path
   CALL %~dp0functions.bat findInPath CC CC_DIR
   IF "%CC_DIR%" NEQ "" GOTO DIR_SET

   REM Check if the compiler is in the known locations file
   
   REM *** Make sure to set the correct KNOWN file for your compiler ***
   CALL %~dp0functions.bat findKnown %~dp0known.vc CC CC_DIR "\Common7\Tools\VsDevCmd.bat"
   IF "%CC_DIR%" NEQ ""                         GOTO DIR_SET

   GOTO NOT_FOUND

ECHO [%~f0] - (%ERRORLEVEL%) Unexpected error!
GOTO FIND_EXIT_99

:DIR_SET
   REM Remove the trailing backslash if any
   IF "%CC_DIR:~-1%" == "\" CALL %~dp0functions.bat Left CC_DIR -1 CC_DIR

   IF EXIST "%CC_DIR%\Vc" SET "CC_DIR=%CC_DIR%\Vc"

   WHERE %CC%.exe >nul 2>&1 && WHERE %RC%.exe >nul 2>&1 && GOTO PATH_OK
   IF EXIST "%CC_DIR%\bin\%CC%.exe"                        GOTO PATH_SET

   REM MSC Specific!
   IF EXIST "%CC_DIR%\..\Common7\Tools\VsDevCmd.bat"       GOTO PATH_SET
   
   GOTO NOT_FOUND

:PATH_SET
   REM We arrived here ONLY if %CC%.exe exists in %CC_DIR%\bin and not in PATH.
    
   REM MSC Specific! 

   IF EXIST "%CC_DIR%\..\Common7\Tools\VsDevCmd.bat" (
      CALL "%CC_DIR%\..\Common7\Tools\VsDevCmd.bat" -arch=%HB_VS_ARCH%
   ) ELSE (
      ECHO For your convenience %CC%'s bin directory was added to your PATH && SET "PATH=%CC_DIR%\bin;%PATH%"
   )
   
:PATH_OK
   WHERE %CC%.exe >nul 2>&1 || GOTO NOT_FOUND
   
   GOTO FOUND

:NOT_FOUND
   REM Let's return an error code to the caller
   ECHO "ERROR: Microsoft C++ not found!"
   GOTO FIND_EXIT_1
   
:FOUND
   SET "LD=%CC%"

   REM MSC Specific!
   IF EXIST "CC_DIR=%CC_DIR%\Vc" (IF "%CC_DIR:~-3%" NEQ "\Vc" SET "CC_DIR=%CC_DIR%\Vc")

   IF "%is_x64_arch%"=="true" (
      SET "HB_VS_ARCH=x64"
      SET "HB_ARCH=w64"
   ) ELSE (
      SET "HB_VS_ARCH=x86"
   )
 
   IF "%HB_ARCH%" == "" SET "HB_ARCH=w32"

   IF "%HB_ARCH%" == "w32" SET "SUB_DIR=vc32"
   IF "%HB_ARCH%" == "w64" SET "SUB_DIR=vc64"

   GOTO FIND_EXIT_0

:CHECK_CC_DIR 
   REM MSC Specific!
   IF EXIST "%CC_DIR%\Common7\Tools\VsDevCmd.bat"      GOTO DIR_SET
   IF EXIST "%CC_DIR%\..\Common7\Tools\VsDevCmd.bat"   GOTO DIR_SET
   IF EXIST "%CC_DIR%\bin\%CC%.exe"                    GOTO DIR_SET
:CHECK_FAILED
   REM If we are here then compiler was not found in the user specified CC_DIR!

   ECHO Could not find Microsoft C++ at '%CC_DIR%'!

   REM Ask the user if they want to search for known locations.
   CALL %~dp0functions.bat continue_Y_N "Search known locations (Y/N)? "
   REM User does not want to search for known locations - Abort.
   IF ERRORLEVEL 1                                      GOTO FIND_EXIT_2
   REM User wants to search for known locations - Continue. 
   GOTO FIND_C_COMPILER

ECHO [%~f0] - (%ERRORLEVEL%) Unexpected error!
GOTO FIND_EXIT_99

 :FIND_EXIT_0
   SET "scriptName="
   ECHO    *** finished[0] [%~f0] >> %~dp0\functions.log
   exit /b 0

:FIND_EXIT_1
   SET "scriptName="
   ECHO    *** finished[1] [%~f0] >> %~dp0\functions.log
   exit /b 1

:FIND_EXIT_2
   SET "scriptName="
   ECHO    *** finished[2] [%~f0] >> %~dp0\functions.log
   exit /b 2

:FIND_EXIT_99
   SET "scriptName="
   ECHO    *** finished[99] [%~f0] >> %~dp0\functions.log
   exit /b 99