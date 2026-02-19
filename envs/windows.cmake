set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

if (CMAKE_BUILD_TYPE STREQUAL "Release" OR
        CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CONAN_PROFILE "release")
else()
    set(CONAN_PROFILE "debug")
endif()

# Добавляем сервер, откуда конан будет качать зависимости
execute_process(
        COMMAND cmd.exe /c add_remote_server.bat
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/conan/scripts_windows
        RESULT_VARIABLE CONAN_RESULT
)
if(NOT ${CONAN_RESULT} EQUAL 0)
    message(FATAL_ERROR "Conan failed to add remote server: ${CONAN_RESULT}")
endif()

# Создаём профили для conan
execute_process(
        COMMAND cmd.exe /c create_profiles.bat
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/conan/scripts_windows
        RESULT_VARIABLE CONAN_RESULT
)
if(NOT ${CONAN_RESULT} EQUAL 0)
    message(FATAL_ERROR "Conan failed to create profiles: ${CONAN_RESULT}")
endif()

# Выполняем установку зависимостей
execute_process(
        COMMAND cmd.exe /c install_deps.bat --channel=${CONAN_CHANNEL} --profile=${CONAN_PROFILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/conan/scripts_windows
        RESULT_VARIABLE CONAN_RESULT
)
if(NOT ${CONAN_RESULT} EQUAL 0)
    message(FATAL_ERROR "Conan failed to install dependencies: ${CONAN_RESULT}")
endif()

