SET CC_DIR=
REM SET BCC_LIB=
SET CC=
SET LD=

for /f "delims=" %%a in ('where bcc32c.exe') do @set _BCC_BIN=%%a
IF NOT "%_BCC_BIN%"=="" SET CC_DIR=%_BCC_BIN:~0,-9% && SET CC=bcc32c && set _BCC_BIN= && GOTO FOUND

:FIND_BCC
   IF EXIST \bcc102                          GOTO SET_BCC_102
   IF EXIST C:\bcc102                        GOTO SET_BCC_C_102
   IF EXIST \Borland\bcc58                   GOTO SET_BORLAND_58
   IF EXIST C:\Borland\bcc58                 GOTO SET_BORLAND_C_58
   IF EXIST \bcc58                           GOTO SET_BCC_58
   IF EXIST C:\bcc58                         GOTO SET_BCC_C_58
   IF EXIST \Borland\bcc55                   GOTO SET_BORLAND_55
   IF EXIST C:\Borland\bcc55                 GOTO SET_BORLAND_C_55
   IF EXIST \bcc55                           GOTO SET_BCC_55
   IF EXIST C:\bcc55                         GOTO SET_BCC_C_55
   IF EXIST "%ProgramFiles%\Borland\BDS\4.0" GOTO SET_BDS_40
   GOTO FOUND

:SET_BCC_102
   SET CC_DIR=\bcc102
   
   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM SET BCC_INC=%CC_DIR%\include\dinkumware64;%CC_DIR%\include\windows\crtl;%CC_DIR%\include\windows\sdk
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib\win32c\debug;%CC_DIR%\lib\win32c\release;%CC_DIR%\lib\win32c\release\psdk
   SET CC=bcc32c

   GOTO FOUND

:SET_BCC_C_102
   SET CC_DIR=C:\bcc102
   
   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM SET BCC_INC=%CC_DIR%\include\dinkumware64;%CC_DIR%\include\windows\crtl;%CC_DIR%\include\windows\sdk
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib\win32c\debug;%CC_DIR%\lib\win32c\release;%CC_DIR%\lib\win32c\release\psdk
   SET CC=bcc32c

   GOTO FOUND

:SET_BORLAND_58
   SET CC_DIR=\Borland\bcc58
   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM set BCC_INC=%CC_DIR%\include
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib;%CC_DIR%\lib\psdk

   GOTO FOUND

:SET_BORLAND_C_58
   SET CC_DIR=C:\Borland\bcc58
   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM set BCC_INC=%CC_DIR%\include
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib;%CC_DIR%\lib\psdk

   GOTO FOUND

:SET_BCC_58
   SET CC_DIR=\bcc58

   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM set BCC_INC=%CC_DIR%\include
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib;%CC_DIR%\lib\psdk

   GOTO FOUND

:SET_BCC_C_58
   SET CC_DIR=C:\bcc58

   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM set BCC_INC=%CC_DIR%\include
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib;%CC_DIR%\lib\psdk

   GOTO FOUND

:SET_BORLAND_55
   SET CC_DIR=\Borland\bcc55

   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM set BCC_INC=%CC_DIR%\include
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib;%CC_DIR%\lib\psdk

   GOTO FOUND

:SET_BORLAND_C_55
   SET CC_DIR=C:\Borland\bcc55

   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM set BCC_INC=%CC_DIR%\include
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib;%CC_DIR%\lib\psdk

   GOTO FOUND

:SET_BCC_55
   SET CC_DIR=\bcc55
   
   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM set BCC_INC=%CC_DIR%\include
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib;%CC_DIR%\lib\psdk

   GOTO FOUND

:SET_BCC_C_55
   SET CC_DIR=C:\bcc55
   
   REM Not needed because bcc32c.exe uses bcc32c.cfg
   REM set BCC_INC=%CC_DIR%\include
   
   REM Needed because iLink32.exe does not have a config file!
   SET BCC_LIB=%CC_DIR%\lib;%CC_DIR%\lib\psdk

   GOTO FOUND

:SET_BDS_40
   SET CC_DIR=%ProgramFiles%\Borland\BDS\4.0

   GOTO FOUND

:NOT_FOUND
   rem Let's return an error code to the caller
   echo "ERROR: Borland C++ not found!"
   exit /b 1
   
:FOUND
   IF "%CC%%"==""      SET CC=bcc32 
   IF "%CC%"=="bcc32c" SET SUB_DIR=b32c && SET LD=bcc32c
   IF "%CC%"=="bcc32"  SET SUB_DIR=b32
   SET HB_ARCH=w32
   exit /b 0
 
