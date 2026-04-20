@echo off

REM  Clean up all Windows project temporary files and build temporary files
REM  EXCEPT do not touch msix-package/AppPackages/ where built packages are kept.

rmdir /S /Q .vs
rmdir /S /Q Release Debug
rmdir /S /Q kuiper-ranger\Release kuiper-ranger\Debug
rmdir /S /Q kuiper-ranger\kuiper-ranger kuiper-ranger\x64
rmdir /S /Q sound-test\x64
rmdir /S /Q msix-package\BundleArtifacts
rmdir /S /Q msix-package\bin msix-package\obj
