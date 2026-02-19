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
shift
goto loop
:endloop

if not defined CHANNEL (
    echo Error: variable CHANNEL is not defined. Consider adding "--channel=release / dev"
    exit /b 1
)

:: Команда conan для загрузки пакета
conan upload %PACKAGE_NAME%/%PACKAGE_VERSION%@%PACKAGE_USER%/%CHANNEL% -r=dt-conan -c
