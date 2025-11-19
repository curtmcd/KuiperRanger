# Kuiper Ranger version 2.5

*Dec 18, 1999 - Nov 18, 2025*

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
  * Your high score nickname can be passed via the `KR_NICK` environment variable. 
      * If that's not set, it tries `USER`, followed by `LOGNAME`, followed by login name. 
      * For multi-player, `KR_NICK1`, `KR_NICK2`, and/or `KR_NICK3` can be set. 
  * High scores are recorded persistently in the system-wide high
    score file `/var/games/kuiper-ranger/scores` if that file exists and
	is world readable/writable. 
      * You can create the file easily by running the script `makehigh.sh`. 
      * If the system-wide file is not available, the scores are
	    stored in `$HOME/.local/state/kuiper-ranger/scores` if `$HOME/.local` exists,
		otherwise `$HOME/.kr-scores`. 

## Build Instructions

### Native (SDL2) Version

1.  Install `libsdl2-dev` 
2.  Run `./prebuild.sh` 
3.  Run `make native` (parallel build is default) 
4.  Optionally run `./makehigh.sh` 
5.  Run `./build/native/bin/kuiper-ranger` 

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

## History

This game originally ran using exclusively integer arithmetic directly
on the bitmapped displays of machines at the time, including aiws,
apa16, sunbw, suncg6, mipscolor, and mega.  Eventually, development swung
toward supporting X11 only.  In 2025, Curt ported it to C++, rebased it
onto the SDL2 graphics and sound library and added sound effects.  Now
in theory it can run on a variety of platforms including Unix (X11 or
Wayland), Windows, and macOS.  It can also be run in a web browser using
Emscripten.
