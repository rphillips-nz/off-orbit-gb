# off-orbit-gb

Off Orbit reborn for the original Nintendo Gameboy. An asteroids style game where firing the ship's weapon also moves you in the opposite direction.

## Development Prerequisites

This game was built on Linux/Windows WSL with [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020).

> [This tutorial series](https://www.youtube.com/watch?v=HIsWR_jLdwo) created by [GamingMonsters](https://www.youtube.com/channel/UCMMhSfBStEti-Lqzs30HYWw) is a good learning resource for GBDK.

### gbdk-2020

1. Download the latest release (v4.3.0 at this time).
2. Extract to a sibling folder of this project called `gbdk`.

   ```
	 ├── off-orbit-gb
   └── gbdk
	 ```

### Other Tools

* [BGB](https://bgb.bircd.org/) emulator for testing the built `off-orbit.gb` file
* [GBTD](http://www.devrs.com/gb/hmgd/gbtd.html) for creating tiles and exporting the data to `.c` files
* [GBMB](http://www.devrs.com/gb/hmgd/gbmb.html) for creating tilemaps and exporting them to `.c` files
* [Pyxel Edit](https://pyxeledit.com/) to edit composed sprites easier (split pasted into GBTD)

## Music

The `src/sound/music.c` file is created from exporting `raw/music.uge` to GBDK from [hUGETracker](https://github.com/SuperDisk/hUGETracker). It's run in game with [hUGEDriver](https://github.com/SuperDisk/hUGEDriver).

## Building

Run this to build `dist/off-orbit.gb` with SDCC and gbdk-2020:

```
$ ./bin/build.sh
```

> Drag the `dist/off-orbit.gb` file into BGB to test it out.
