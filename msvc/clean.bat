set MSBUILD="c:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"

%MSBUILD% msvc.slnx /t:Clean /p:Configuration=Debug /p:Platform=x64
%MSBUILD% msvc.slnx /t:Clean /p:Configuration=Release /p:Platform=x64

rmdir /S /Q x64
if exist "kuiper-ranger\resources.aps" del "kuiper-ranger\resources.aps"
