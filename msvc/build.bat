@echo off
setlocal enabledelayedexpansion

:: 1. Setup MSBuild Path
:: Note: If the path changes, you only have to fix it here once.
set MSBUILD="c:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"

if "%~1"=="" (
   call :MENU ans
) else (
   set "ans=%~1"
)

if /i "%ans%"=="1" ( call :BUILD_PROJ kuiper-ranger\kuiper-ranger.vcxproj Release x64 & exit /b )
if /i "%ans%"=="2" ( call :BUILD_PROJ kuiper-ranger\kuiper-ranger.vcxproj Debug x64 & exit /b )
if /i "%ans%"=="3" ( call :BUILD_PROJ kuiper-ranger\kuiper-ranger.vcxproj Release Win32 & exit /b )
if /i "%ans%"=="4" ( call :BUILD_PROJ kuiper-ranger\kuiper-ranger.vcxproj Debug Win32 & exit /b )

if /i "%ans%"=="5" ( call :BUILD_PROJ sound-test\sound-test.vcxproj Release x64 & exit /b )
if /i "%ans%"=="6" ( call :BUILD_PROJ sound-test\sound-test.vcxproj Debug x64 & exit /b )
if /i "%ans%"=="7" ( call :BUILD_PROJ sound-test\sound-test.vcxproj Release Win32 & exit /b )
if /i "%ans%"=="8" ( call :BUILD_PROJ sound-test\sound-test.vcxproj Debug Win32 & exit /b )

if /i "%ans%"=="9" ( call :BUILD_MSIX & exit /b )
if /i "%ans%"=="q" goto :EOF

echo Invalid Option.
exit /b

:: --- Subroutines ---

:MENU
echo ======================================================
echo           KUIPER RANGER - BUILD SYSTEM
echo ======================================================
echo 1) Build Kuiper Ranger (x64 Release)
echo 2) Build Kuiper Ranger (x64 Debug)
echo 3) Build Kuiper Ranger (Win32 Release)
echo 4) Build Kuiper Ranger (Win32 Debug)
echo ------------------------------------------------------
echo 5) Build Sound Test (x64 Release)
echo 6) Build Sound Test (x64 Debug)
echo 7) Build Sound Test (Win32 Release)
echo 8) Build Sound Test (Win32 Debug)
echo ------------------------------------------------------
echo 9) Build FULL MSIX BUNDLE (Clean + x86 + x64)
echo Q) Quit
echo ======================================================
set /p ans="Option? "
set "%~1=%ans%"
goto :EOF

:BUILD_PROJ
echo [BUILDING] %1 (%2 %3)...
%MSBUILD% %1 -t:Build -p:Configuration=%2 -p:Platform=%3
if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: Build failed for %1
)
exit /b

:BUILD_MSIX
echo [CLEANING SOLUTION] Killing the MSIX cache...
%MSBUILD% msvc.slnx -t:Clean
echo [BUILDING MSIX BUNDLE] This handles x86 and x64...
:: AppxBundle=Always creates the .msixbundle
:: UapAppxPackageBuildMode=Sideloading ensures it creates the test/sideload folders
%MSBUILD% msvc.slnx -p:Configuration=Release -p:AppxBundle=Always -p:UapAppxPackageBuildMode=Sideloading -p:AppxPackageSigningEnabled=true
if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: MSIX Bundle generation failed.
) else (
    echo.
    echo SUCCESS: Bundle is ready in the AppPackages folder.
)
exit /b
