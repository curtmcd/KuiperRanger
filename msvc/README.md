# Windows Build Instructions for Kuiper Ranger

It is very easy to build in Visual Studio or at the Command Prompt if you use `vcpkg` to install the SDL2 libraries. The provided Solution file `msvc\msvc.slnx` is set up to build the project using the `vcpkg` method.

Not recommended: using `cmake`, or manually installing libraries in non-standard folder locations.

## Install vcpkg (if you don't already have it)

As Administrator:

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
C:\vcpkg integrate install
```

Be sure to run `C:\vcpkg integrate install` as above. It makes all of the following automatic. Otherwise there are many settings and commands needed in the MS build project files.

- Detects and sets up #include paths for C++ libraries being used
- Detects which libraries are being used, sets up the library paths and dependencies (although it misses SDL2main.lib)
- As part of the build, automatically copies the needed .dll files to the output folder alongside the .exe files

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

Right click the `kuiper-ranger` or `sound-test` project and run `Build` -> `Build Solution`.

### Command Prompt

Use msbuild.

To make it easy, a simple batch file `build.bat` is included that will build any of the options.

## Execute

You can run Kuiper Ranger from within Visual Studio after building it by using `Debug` -> `Start Debugging` or `Debug` -> `Start Without Debugging`.

To run it from the Command Prompt, you can go to the appropriate build subfolder and run `.\kuiper-ranger.exe`. A sample batch file `run.bat` is included at the `msvc\` level for easy testing:

- Use `run.bat` to run Release
- Use `run.bat debug` to run Debug

The utility sound testing program is built as a separate project called `sound-test.exe`. It can only be run from the command prompt because it has a command interface. It allows you to play various sounds in one-shot or continuous mode.

## Packaging

The application is packaged as an MSIX bundle that contains both `x86` and `x64` versions.

The project msix-package is configured to build this complete package. This is the default if you attempt to build the whole Solution.

This package was generated as follows:
* To the solution, Add -> New Project, type `Windows Application Packaging Project`.
* Visual Assets were auto-generated from the AI-generated cover image `cover-image2Gc.png`.
* The script `imageRegen.sh` was run Linux to replace all of the generated images with same-sized custom scaled and tweaked versions.

This package will be for the desktop only.
* Edit the `Package.appxmanifest` file
* Remove the `TargetDeviceFamily` for `Windows.Universal`

To build the package:
* Select `Build` -> `Clean Solution`. Or better yet, run the provided `clean.bat`.
  *WARNING*: I've found the DLLs do *not* get included in the packaged app unless the solution is cleaned first!!
* Right-click on the `msix-package` project and select `Publish` -> `Create App Packages...`.

After the build completes in Visual Studio, a window pops up for `Finished creating package` with a button at the bottom labeled `Launch Windows App Certification Kit`. Click that button to generate a certificate.

The finished package ends up under `AppPackages\msix-package_2.6.15.0_x86_x64_bundle.msixupload` or similar. Also, a test folder is created called `AppPackages\msix-package_2.6.15.0_Test` or similar.

To test the package:
* In the newly created `*_Test` folder, find the `.cer` file. It contains a certificate that corresponds only to this test release, and is required before it can be installed.
* Install the certificate by double-clicking on the `cer` file. Import it to `Local Machine` and place it in the certificate store called `Trusted People`.
* Double-click the `msixbundle` file to install and run the app for testing!

Note: after creating a package, the version number gets updated in `msvc\msix-package\Package.appxmanifest`. Therefore that file should be checked into Git each time a public release is made.

## Clean up

Run `clean.bat` to clean up. This is needed because Microsoft's idea of `make clean` is to leave 172 MB of junk laying around in various folders.

This clean script leaves the `AppPackages` folder alone (the folder containing all of the built packages).
