@echo off

set CONFIG=Release

if /I "%~1" == "debug" (
   set CONFIG=Debug
)

set PROG=".\kuiper-ranger\x64\%CONFIG%\kuiper-ranger.exe"

%PROG%
