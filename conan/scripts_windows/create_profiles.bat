@echo off
:: Создание профиля для Windows с настройками MSVC
set PROFILE_DIR=%USERPROFILE%\.conan2\profiles
mkdir "%PROFILE_DIR%" >nul 2>&1

echo [settings] > "%PROFILE_DIR%\release"
echo arch=x86_64 >> "%PROFILE_DIR%\release"
echo build_type=Release >> "%PROFILE_DIR%\release"
echo compiler=msvc >> "%PROFILE_DIR%\release"
echo compiler.cppstd=17 >> "%PROFILE_DIR%\release"
echo compiler.runtime=dynamic >> "%PROFILE_DIR%\release"
echo compiler.version=194 >> "%PROFILE_DIR%\release"
echo os=Windows >> "%PROFILE_DIR%\release"
echo. >> "%PROFILE_DIR%\release"

echo [conf] >> "%PROFILE_DIR%\release"
echo tools.cmake.cmaketoolchain:generator=Ninja >> "%PROFILE_DIR%\release"

echo [settings] > "%PROFILE_DIR%\debug"
echo arch=x86_64 >> "%PROFILE_DIR%\debug"
echo build_type=Debug >> "%PROFILE_DIR%\debug"
echo compiler=msvc >> "%PROFILE_DIR%\debug"
echo compiler.cppstd=17 >> "%PROFILE_DIR%\debug"
echo compiler.runtime=dynamic >> "%PROFILE_DIR%\debug"
echo compiler.version=194 >> "%PROFILE_DIR%\debug"
echo os=Windows >> "%PROFILE_DIR%\debug"
echo. >> "%PROFILE_DIR%\debug"

echo [conf] >> "%PROFILE_DIR%\debug"
echo tools.cmake.cmaketoolchain:generator=Ninja >> "%PROFILE_DIR%\debug"

