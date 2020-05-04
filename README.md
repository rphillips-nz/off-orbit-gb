# off-orbit-gb

Off Orbit reborn for the original Nintendo Gameboy. An asteroids style game where firing the ship's weapon also moves you in the opposite direction.



## Development Prerequisites

This game is built on Windows with [GBDK](http://gbdk.sourceforge.net/install.html) *or* [gbdk-n](https://github.com/andreasjhkarlsson/gbdk-n). gbdk-n seems to make a build that runs smoother.

> [This tutorial series](https://www.youtube.com/watch?v=HIsWR_jLdwo) created by [GamingMonsters](https://www.youtube.com/channel/UCMMhSfBStEti-Lqzs30HYWw) is a good learning resource for GBDK/gbdk-n.

### gbdk-n

1. Install SDCC 3.8.0 with a prebuilt installer (newer versions did not work for me)
2. Install [gbdk-n](https://github.com/andreasjhkarlsson/gbdk-n) at the root of a drive (i.e. `D:\gbdk-n`)
3. Run `make` in your gbdk-n folder to build it
4. Ensure `make.bat` has the correct path to the gbdk-n folder
5. Ensure SDCC is in your path

### GBDK

1. Extract the [gbdk zip](https://sourceforge.net/projects/gbdk/files/) at the root of a drive (i.e. `D:\gbdk`)
2. Ensure `make.bat` has the correct path to the gbdk-n folder

### Other Tools

* BGB emulator for testing the built `off-orbit.gb` file
* [GBTD](http://www.devrs.com/gb/hmgd/gbtd.html) for creating tiles and exporting the data to `.c` files
* [GBMB](http://www.devrs.com/gb/hmgd/gbmb.html) for creating tilemaps and exporting them to `.c` files
* [OpenMPT](https://openmpt.org/download) for creating the music in `raw/music.mod`
* [Pyxel Edit](https://pyxeledit.com/) to edit composed sprites easier (split pasted into GBTD).



## Music

Run the following to recreate the `src/music.c` file after making changes to `raw/music.mod`:

```
$ make music
```

This runs mod2gbt and removes the `#pragma bank=2` line from the generated file so it doesn't use banking (breaks the build).

> The `gbt_player*` files in this repo are modified to disable banking (https://gbdev.gg8.se/forums/viewtopic.php?id=466), since SDCC doesn't support it and this game is made for a 32KB cart which doesn't switch memory banks.



## Building

Run this to build `dist/off-orbit.gb` with SDCC and gbdk-n:

```
$ make
```

Run this to build `dist/off-orbit-lcc.gb` with GBDK:

```
$ make lcc
```

Remove the temporary build files with the following:

```
$ make clean
```

> Drag a `.gb` file into BGB to test it out.
