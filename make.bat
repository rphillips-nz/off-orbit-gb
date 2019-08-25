set BIN=..\..\gbdk-n\bin
set OBJ=build
set DIST=dist

if "%1"=="clean" (
	if exist %OBJ% rd /s/q %OBJ%
	if exist off-orbit.gb del off-orbit.gb
	goto end
)

if not exist %OBJ% mkdir %OBJ%
if not exist %DIST% mkdir %DIST%

call %BIN%\gbdk-n-compile.bat main.c -o %OBJ%\main.rel
call %BIN%\gbdk-n-link.bat %OBJ%\main.rel -o %OBJ%\main.ihx
call %BIN%\gbdk-n-make-rom.bat %OBJ%\main.ihx %dist%\off-orbit.gb

:end