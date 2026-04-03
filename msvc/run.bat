@echo off

set CONFIG=Release

if /I "%~1" == "debug" (
   set CONFIG=Debug
)

set PROG=".\x64\%CONFIG%\kuiper-ranger\bin\kuiper-ranger.exe"

%PROG%
