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
		type output.c | findstr /v pragma > %SRC%\sound\music.c
		del output.c
	)
	goto end
)

if not exist %OBJ% mkdir %OBJ%
if not exist %DIST% mkdir %DIST%

if "%1"=="lcc" (
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/semi-random.o      %SRC%/semi-random.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/game-character.o   %SRC%/game-character.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/enemy.o            %SRC%/enemy.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/player.o           %SRC%/player.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/bullet.o           %SRC%/bullet.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/main.o             %SRC%/main.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/sound.o            %SRC%/sound/sound.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/music.o            %SRC%/sound/music.c
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/gbt_player.o       %SRC%/sound/gbt_player.s
	%LCC% -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/gbt_player_bank1.o %SRC%/sound/gbt_player_bank1.s
	%LCC% -DUSE_SFR_FOR_REG -Wl-yt1 -Wl-ya0 -o %DIST%/off-orbit-lcc.gb %OBJ%/semi-random.o %OBJ%/game-character.o %OBJ%/enemy.o %OBJ%/player.o %OBJ%/bullet.o %OBJ%/main.o %OBJ%/sound.o %OBJ%/music.o %OBJ%/gbt_player.o %OBJ%/gbt_player_bank1.o
	goto end
)

call %ASSEMBLE% -o %OBJ%\gbt_player.rel       %SRC%\sound\gbt_player.s
call %ASSEMBLE% -o %OBJ%\gbt_player_bank1.rel %SRC%\sound\gbt_player_bank1.s
call %COMPILE%  %SRC%\semi-random.c    -o %OBJ%\semi-random.rel
call %COMPILE%  %SRC%\game-character.c -o %OBJ%\game-character.rel
call %COMPILE%  %SRC%\enemy.c          -o %OBJ%\enemy.rel
call %COMPILE%  %SRC%\player.c         -o %OBJ%\player.rel
call %COMPILE%  %SRC%\bullet.c         -o %OBJ%\bullet.rel
call %COMPILE%  %SRC%\sound\sound.c    -o %OBJ%\sound.rel
call %COMPILE%  %SRC%\sound\music.c    -o %OBJ%\music.rel
call %COMPILE%  %SRC%\main.c %OBJ%\music.rel %OBJ%\gbt_player.rel %OBJ%\gbt_player_bank1.rel -o %OBJ%\main.rel
call %LINK%     %OBJ%\main.rel %OBJ%\semi-random.rel %OBJ%\game-character.rel %OBJ%\enemy.rel %OBJ%\player.rel %OBJ%\bullet.rel %OBJ%\sound.rel %OBJ%\music.rel %OBJ%\gbt_player.rel %OBJ%\gbt_player_bank1.rel -o %OBJ%\main.ihx
call %MAKE_ROM% %OBJ%\main.ihx %DIST%\off-orbit.gb

:end