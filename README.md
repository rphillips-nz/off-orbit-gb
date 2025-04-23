# off-orbit-gb

Off Orbit reborn for the original Nintendo Gameboy. Originally a wildly unpopular iOS game, Off Orbit is an asteroids style game where firing the ship's weapon also moves you in the opposite direction.

![Screenshot](screenshot.md)

## Development

This game was built on Linux/Windows WSL with [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020).

### Tools and Libraries

* [BGB](https://bgb.bircd.org/) emulator for testing the built `dist/off-orbit.gb` file
* [hUGETracker](https://github.com/SuperDisk/hUGETracker) for creating `src/sound/music.c` by exporting `raw/music.uge` to GBDK format.
* [hUGEDriver](https://github.com/SuperDisk/hUGEDriver) for playing `src/sound/music.c` in game.
* [GBTD](http://www.devrs.com/gb/hmgd/gbtd.html) for creating tiles and exporting the data to `.c` files.
* [GBMB](http://www.devrs.com/gb/hmgd/gbmb.html) for creating tilemaps and exporting them to `.c` files.
* [Pyxel Edit](https://pyxeledit.com/) to edit composed sprites easier (split pasted into GBTD).

## Build

1. Download the latest release of [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020) (v4.3.0 at this time).
2. Extract to a sibling folder of this project called `gbdk`.

   ```
   ├── off-orbit-gb
   └── gbdk
   ```

Run this from the base directory to build `dist/off-orbit.gb`:

```
$ ./bin/build.sh
```

## Run

Drag the `dist/off-orbit.gb` file into BGB to test it out.
