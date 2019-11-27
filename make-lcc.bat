set BIN=D:\gbdk\bin
set OBJ=build
set DIST=dist
set SRC=src

if "%1"=="clean" (
	if exist %OBJ% rd /s/q %OBJ%
	if exist off-orbit.gb del off-orbit.gb
	goto end
)

if "%1"=="music" (
	bin\mod2gbt.exe raw\music.mod song -c 2
	goto end
)

if not exist %OBJ% mkdir %OBJ%
if not exist %DIST% mkdir %DIST%

%BIN%\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/main.o             %SRC%/main.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/output.o           %SRC%/output.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/gbt_player.o       %SRC%/gbt_player.s
%BIN%\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o %OBJ%/gbt_player_bank1.o %SRC%/gbt_player_bank1.s

%BIN%\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -Wl-yt1 -Wl-ya0 -o %DIST%/off-orbit-lcc.gb %OBJ%/main.o %OBJ%/output.o %OBJ%/gbt_player.o %OBJ%/gbt_player_bank1.o

:end