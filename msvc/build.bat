@echo off

set MSBUILD="c:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"

set CONFIG=Release
set PLATFORM=x64

if /I "%~1" == "debug" (
   set CONFIG=Debug
)

%MSBUILD% msvc.slnx /p:Configuration=%CONFIG% /p:Platform=%PLATFORM%
