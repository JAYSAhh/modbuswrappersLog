if (CMAKE_BUILD_TYPE STREQUAL "Release" OR
        CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CONAN_PROFILE "release")
else()
    set(CONAN_PROFILE "debug")
endif()

# Добавляем сервер, откуда Conan будет качать зависимости
execute_process(
        COMMAND bash ${CMAKE_SOURCE_DIR}/conan/scripts_linux/add_remote_server.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/conan/scripts_linux
        RESULT_VARIABLE CONAN_RESULT
)
if(NOT ${CONAN_RESULT} EQUAL 0)
    message(FATAL_ERROR "Conan failed to add remote server: ${CONAN_RESULT}")
endif()

# Создаём профили для Conan
execute_process(
        COMMAND bash ${CMAKE_SOURCE_DIR}/conan/scripts_linux/create_profiles.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/conan/scripts_linux
        RESULT_VARIABLE CONAN_RESULT
)
if(NOT ${CONAN_RESULT} EQUAL 0)
    message(FATAL_ERROR "Conan failed to create profiles: ${CONAN_RESULT}")
endif()

# Выполняем установку зависимостей
execute_process(
        COMMAND bash ${CMAKE_SOURCE_DIR}/conan/scripts_linux/install_deps.sh --channel=${CONAN_CHANNEL} --profile=${CONAN_PROFILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/conan/scripts_linux
        RESULT_VARIABLE CONAN_RESULT
)
if(NOT ${CONAN_RESULT} EQUAL 0)
    message(FATAL_ERROR "Conan failed to install dependencies: ${CONAN_RESULT}")
endif()
