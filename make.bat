echo off

set BIN=D:\gbdk-n\bin
set COMPILE=%BIN%\gbdk-n-compile.bat
set ASSEMBLE=%BIN%\gbdk-n-assemble.bat
set LINK=%BIN%\gbdk-n-link.bat
set MAKE_ROM=%BIN%\gbdk-n-make-rom.bat

set LCC=D:\gbdk\bin\lcc

set OBJ=build
set DIST=dist
set SRC=src

if "%1"=="clean" (
	if exist %OBJ% rd /s/q %OBJ%
	if exist %DIST%\off-orbit.gb del %DIST%\off-orbit.gb
	if exist %DIST%\off-orbit-lcc.gb del %DIST%\off-orbit-lcc.gb
	goto end
)

if "%1"=="music" (
	bin\mod2gbt.exe raw\music.mod song -c 2
	if exist output.c (
		type output.c | findstr /v pragma > %SRC%\music.c
		del output.c
	)
	goto end
)

if not exist %OBJ% mkdir %OBJ%
if not exist %DIST% mkdir %DIST%

if "%1"=="lcc" (
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/main.o             %SRC%/main.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/music.o            %SRC%/music.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/gbt_player.o       %SRC%/gbt_player.s
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/gbt_player_bank1.o %SRC%/gbt_player_bank1.s
	%LCC% -DUSE_SFR_FOR_REG -Wl-yt1 -Wl-ya0 -o %DIST%/off-orbit-lcc.gb %OBJ%/main.o %OBJ%/music.o %OBJ%/gbt_player.o %OBJ%/gbt_player_bank1.o
	goto end
)

call %ASSEMBLE% -o %OBJ%\gbt_player.rel %SRC%\gbt_player.s
call %ASSEMBLE% -o %OBJ%\gbt_player_bank1.rel %SRC%\gbt_player_bank1.s
call %COMPILE%  %SRC%\music.c -o %OBJ%\music.rel
call %COMPILE%  %SRC%\main.c %OBJ%\music.rel %OBJ%\gbt_player.rel %OBJ%\gbt_player_bank1.rel -o %OBJ%\main.rel
call %LINK%     %OBJ%\main.rel %OBJ%\music.rel %OBJ%\gbt_player.rel %OBJ%\gbt_player_bank1.rel -o %OBJ%\main.ihx
call %MAKE_ROM% %OBJ%\main.ihx %DIST%\off-orbit.gb

:end