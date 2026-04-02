# Windows Build Instructions for Kuiper Ranger

It is very easy to build in Visual Studio or at the Command Prompt if you use `vcpkg` to
install the SDL2 libraries. The provided Solution file `msvc\msvc.slnx` is set up to
build the project using the `vcpkg` method.

Not recommended: using `cmake`, or manually installing libraries in random directories.

## Install vcpkg (if you don't already have it)

As Administrator:

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
C:\vcpkg integrate install
```

Be sure to run `C:\vcpkg integrate install` as above. It makes all of the following
automatic. Otherwise there are many settings and commands needed in the MS build project files.

- Detects and sets up #include paths for C++ libraries being used
- Detects which libraries are being used, sets up the library paths and dependencies
  (although it misses SDL2main.lib)
- As part of the build, automatically copies the needed .dll files to the output directory
  alongside the .exe files

## Install SDL2 libraries via vcpkg

As Administrator:

```powershell
C:\vcpkg\vcpkg.exe install sdl2
C:\vcpkg\vcpkg.exe install sdl2-image
```

The `sdl2-image` library is currently used only for setting the task bar icon.

## Build

Build is easy from within Visual Studio or via the Command Prompt.

### Visual Studio

Double-click `msvc\msvc.slnx` in File Explorer or open it in Visual Studio.

Select whether you want a `Release` or `Debug` build from the drop-downs at the top of the screen.

Select the desired platform (likely `x64`) at the top of the screen.

Run `Build` -> `Build Solution`

All output files are placed in `msvc\x64\...`

### Command Prompt

Use msbuild.

To make it easy, a simple batch file `build.bat` is included:
- Use `build.bat` to build Release
- Use `build.bat debug` to build Debug

## Execute

You can run Kuiper Ranger from within Visual Studio after building it
by using `Debug` -> `Start Debugging` or `Debug` -> `Start Without Debugging`.

To run it from the Command Prompt, you can go to the appropriate `bin\` subdirectory
of `x64\` and run `.\kuiper-ranger.exe`. A sample batch file `run.bat` is included at
the `msvc\` level for easy testing:

- Use `run.bat` to run Release
- Use `run.bat debug` to run Debug

To run it from `File Explorer`, navigate to the `bin\` directory and double-click the `.exe` file.

There is a utility sound testing program built as a separate project called `sound-test.exe`.
It can only be run from the command prompt because it has a command interface. It allows you
to play various sounds in one-shot or continuous mode.

## Clean up

A sample batch file `clean.bat` is included. It cleans the `Debug` and `Release` builds
and removes the `x64\` directory entirely.
