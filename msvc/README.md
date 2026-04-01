Windows build instructions for Kuiper Ranger

Recommended: use vcpkg to install SDL2 then build in Visual Studio

1) Install vcpkg (if you don't have it)

As Administrator:

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
```

2) Install SDL2 via vcpkg

As Administrator:

```powershell
C:\vcpkg\vcpkg.exe install sdl2
C:\vcpkg\vcpkg.exe install sdl2-image
```

3) Double-click `msvc\msvc.slnx` in File Explorer or open it in Visual Studio.

The location of the SDL2 include files and libraries is configured in the kuiper-ranger.vcxproj file.

For Debug Build (SDL2d.lib and SDL2maind.lib):
	C:\vcpkg\installed\x64-windows\include
	C:\vcpkg\installed\x64-windows\debug\lib
	C:\vcpkg\installed\x64-windows\debug\lib\manual-link

For Release Build (SDL2.lib and SDL2main.lib):
	C:\vcpkg\installed\x64-windows\include
	C:\vcpkg\installed\x64-windows\lib
	C:\vcpkg\installed\x64-windows\lib\manual-link

4) Build

Select build option at the top (configuration Debug or Release and platform x64).

Execute Build -> Build Solution

For Debug build, output is placed in
	msvc/x64/Debug/obj/						<== intermediate files
 	msvc/x64/Debug/bin/kuiper-ranger.exe
	msvc/x64/Debug/bin/SDL2d.dll

For Release build, output is placed in
	msvc/x64/Debug/obj/						<== intermediate files
	msvc/x64/Release/bin/kuiper-ranger.exe
	msvc/x64/Release/bin/SDL2.dll

Example building the Release version from the command line:

p:\KuiperRanger>"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" msvc\msvc.slnx /p:Configuration=Release /p:Platform=x64

A sample batch file `build.bat` is included.
- use `build.bat` to build Release
- use `build.bat debug` to build Debug

5) Run the binary

The `SDL2.dll` must be in the same directory as the .exe file or in your PATH.

Go to the bin directory and run `.\kuiper-ranger.exe`.

A sample batch file `run.bat` is included.
- use `run.bat` to run Release
- use `run.bat debug` to run Debug

6) Clean up

A sample batch file `clean.bat` is included. It removes both Debug and Release and the whole x86 artifacts directory.
