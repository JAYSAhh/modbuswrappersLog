@echo off

set SCRIPT_DIR=%~dp0
set CONFIG=%SCRIPT_DIR%..\..\config.json

for /f "tokens=2 delims=:," %%a in ('findstr /i "\"name\"" "%CONFIG%"') do set RAW=%%a
set "PACKAGE_NAME=%RAW: =%"

for /f "tokens=2 delims=:," %%a in ('findstr /i "\"version\"" "%CONFIG%"') do set RAW=%%a
set "PACKAGE_VERSION=%RAW: =%"

for /f "tokens=2 delims=:," %%a in ('findstr /i "\"user\"" "%CONFIG%"') do set RAW=%%a
set "PACKAGE_USER=%RAW: =%"


:: Обработка аргументов командной строки
:loop
if "%1"=="" goto endloop
if "%1"=="--channel" (
    set CHANNEL=%2
    shift
)
if "%1"=="--profile" (
    set PROFILE=%2
    shift
)
shift
goto loop
:endloop

if not defined CHANNEL (
    echo Error: variable CHANNEL is not defined. Consider adding "--channel=release / dev" if you are running "install_deps.bat".
    echo Or specify CMake variable: "-DCONAN_CHANNEL=release / dev" if you are loading CMake project.
    exit /b 1
)

if not defined PROFILE (
    echo Error: variable PROFILE is not defined. Consider adding "--profile=release / debug"  if you are running "install_deps.bat".
    exit /b 1
)

conan install ../.. --build=missing --remote=dt-conan --user=%PACKAGE_USER% --channel=%CHANNEL% --profile:host=%PROFILE% --profile:build=%PROFILE%
