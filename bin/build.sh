#!/bin/bash

OBJ=build
DIST=dist
SRC=src

rm -f $DIST/*.gb

mkdir -p $OBJ
mkdir -p $DIST

../gbdk/bin/sdar -ru lib/hUGEDriver.lib $OBJ/hUGEDriver.o

../gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o $OBJ/semi-random.o $SRC/semi-random.c
../gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o $OBJ/game-character.o $SRC/game-character.c
../gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o $OBJ/enemy.o $SRC/enemy.c
../gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o $OBJ/player.o $SRC/player.c
../gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o $OBJ/bullet.o $SRC/bullet.c
../gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o $OBJ/main.o $SRC/main.c
../gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o $OBJ/sound.o $SRC/sound/sound.c
../gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o $OBJ/music.o $SRC/sound/music.c

../gbdk/bin/lcc -Wl-yt1 -Wl-ya0 -Wl-llib/hUGEDriver.lib -o $DIST/off-orbit.gb \
	$OBJ/semi-random.o \
	$OBJ/game-character.o \
	$OBJ/enemy.o \
	$OBJ/player.o \
	$OBJ/bullet.o \
	$OBJ/main.o \
	$OBJ/sound.o \
	$OBJ/music.o

rm -f $OBJ/*.o \
	$OBJ/*.lst \
	$OBJ/*.map \
	$OBJ/*.rel \
	$OBJ/*.cdb \
	$OBJ/*.ihx \
	$OBJ/*.lnk \
	$OBJ/*.sym \
	$OBJ/*.asm \
	$OBJ/*.noi \
	$OBJ/*.rst \
	$DIST/*.ihx

rmdir $OBJ
