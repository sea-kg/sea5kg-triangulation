@echo off
pushd %~p1
call rsvars.bat
msbuild.exe /m /t:build /p:Config=Debug
popd
