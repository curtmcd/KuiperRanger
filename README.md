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

A faithful implementation of a vector arcade game in which your ship shoots down rocks and flying saucers to gain points.

## Features

  * Takes over the whole screen
  * Uses monochrome vector graphics
  * Provides sound effects
  * Saves the high score table between games
  * Allows rotating turns for one to three players

## Notes

  * This is a SUPERB implementation and LUCS has managed a high score of 14300 so far: can you do better?

## High score file

The location of the high score file varies between systems and configurations:

* Windows

  `%APPDATA%\Fishlet\kuiper-ranger\scores`

* Linux, if installed by a package manager

  `/var/games/kuiper-ranger/scores`

* Linux, if run manually

  `~/.local/share/Fishlet/kuiper-ranger/scores`

  If you would prefer using a system-wide high score file, you may create the file manually:

  * sudo mkdir -p /var/games/kuiper-ranger
  * sudo touch /var/games/kuiper-ranger/scores
  * sudo chmod 777 /var/games/kuiper-ranger/scores

* Manual override

  If the environment variable `KR_SCORES` is set to the name of a file that the game can create and write to, scores will be stored there.

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

It can be built from Visual Studio or from the Command Prompt.

Please see the separate `README.md` under the `msvc/` directory.

## History

This game originally ran using exclusively integer arithmetic directly on the bitmapped displays of machines at the time, including aiws, apa16, sunbw, suncg6, mipscolor, and mega. Eventually, development swung toward supporting X11 only. In 2025, Curt ported it to C++, rebased it onto the SDL2 graphics and sound library and added sound effects. Now it can run on a variety of platforms including Unix (X11 or Wayland) on x64, arm64 or riscv5 (it has been packaged as a .deb for all three but the latter two have not been tested), Windows, and in a web browser using Emscripten. It might be ported to MacOS in the future.
