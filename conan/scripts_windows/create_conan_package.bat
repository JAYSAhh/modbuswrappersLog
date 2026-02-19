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

:: Проверка переменных CHANNEL и PROFILE
if not defined CHANNEL (
    echo Error: variable CHANNEL is not defined. Consider adding "--channel=release / dev"
    exit /b 1
)

if not defined PROFILE (
    echo Error: variable PROFILE is not defined. Consider adding "--profile=release / debug"
    exit /b 1
)

:: Команда conan с параметрами из config.json и переданным значением channel
conan create ..\.. --name=%PACKAGE_NAME% --version=%PACKAGE_VERSION% --user=%PACKAGE_USER% --channel=%CHANNEL% --profile:host=%PROFILE% --profile:build=%PROFILE% --build=missing -r=dt-conan