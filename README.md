# Kuiper Ranger version 2.6

*Dec 18, 1999 - April 2, 2026*

## Authors

  * Curt McDowell
  * Miles Bader
  * William Lott
  * Originated at Carnegie Mellon in 1988

### Contributors

  * Christos Zoulas
  * Zalmon Stern
  * R.K.
  * Liverpool
  * David Sanderson

## Purpose

A faithful implementation of a vector arcade game
in which your ship shoots down rocks and flying saucers to gain points.

## Features

  * Takes over the whole screen
  * Uses monochrome vector graphics
  * Provides sound effects
  * Saves the high score table between games
  * Allows rotating turns for one to three players

## Notes

  * This is a SUPERB implementation and LUCS has managed
    a high score of 14300 so far: can you do better?
  * High scores are normally stored in
	* `~/.local/share` on Linux or `%APPDATA%` on Windows
	* You can set the `KR_SCORES` to another writable filename if desired

## Build Instructions for Linux

### Native (SDL2) Version

1.  Install `libsdl2-dev`
2.  Run `./prebuild.sh`
3.  Run `make` or `make native` (native is the default), or `make -j` for parallel build
4.  Run `./build/native/bin/kuiper-ranger`

### HTML5 (Web) Version

1.  Download and install the Emscripten toolchain
	- `git clone https://github.com/emscripten-core/emsdk.git emsdk.git`
	- `cd emsdk.git`
    - `./emsdk install latest`
    - `./emsdk activate latest` # Activate within current shell
	- `source ./emsdk_env.sh`
	- `em++ -v`
2.  Run `./prebuild.sh`
3.  Run `make emscripten`
4.  Run `python3 -m http.server --directory build/emscripten/bin`
5.  Open web browser and go to `http://localhost:8000/kuiper-ranger.html`
6.  Install the three files on whichever web server you'd like.

## Build Instructions for Windows

Please see the separate `README.md` under the `msvc/` directory.

## History

This game originally ran using exclusively integer arithmetic directly
on the bitmapped displays of machines at the time, including aiws,
apa16, sunbw, suncg6, mipscolor, and mega. Eventually, development swung
toward supporting X11 only. In 2025, Curt ported it to C++, rebased it
onto the SDL2 graphics and sound library and added sound effects. Now in
theory it can run on a variety of platforms including Unix (X11 or
Wayland), Windows, and macOS (it has not yet been tried on the latter
two). It can also be run in a web browser using Emscripten.
