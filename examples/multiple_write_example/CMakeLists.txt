cmake_minimum_required(VERSION 3.14)
project(multiple_write_examples LANGUAGES CXX)
include("../../cmake/utils.cmake")

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
        is_top_level)


find_package(Threads REQUIRED)
find_package(utils REQUIRED)
find_package(modbus REQUIRED)

if(is_top_level)
    find_package(modbuswrappers REQUIRED)
endif()

file(GLOB SRC main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC})

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE
        Threads::Threads
        utils::utils
        modbus::modbus
        modbuswrappers::modbuswrappers
)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(${PROJECT_NAME} modbuswrappers::modbuswrappers)
    win_copy_deps_to_target_dir(${PROJECT_NAME} modbus::modbus)
    win_copy_deps_to_target_dir(${PROJECT_NAME} utils::utils)

    if(${TARGET_SYSTEM} STREQUAL ConanWindows)
        set(TARGET_DLL_PATH $<TARGET_FILE_DIR:${PROJECT_NAME}>)

        if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
            set(CONAN_FOLDER_POSTFIX "DEBUG")
        else ()
            set(CONAN_FOLDER_POSTFIX "RELEASE")
        endif ()

        message("${qt_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${utils_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/utils${CMAKE_DEBUG_POSTFIX}.dll ${TARGET_DLL_PATH}
                ${modbus_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/modbus${CMAKE_DEBUG_POSTFIX}.dll ${TARGET_DLL_PATH}
                ${threadpooling_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/threadpooling${CMAKE_DEBUG_POSTFIX}.dll ${TARGET_DLL_PATH}
#                ${modbuswrappers_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/modbuswrappers${CMAKE_DEBUG_POSTFIX}.dll ${TARGET_DLL_PATH}
#                ${callbacks_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/callbacks${CMAKE_DEBUG_POSTFIX}.dll ${TARGET_DLL_PATH}
#                ${widgets_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/widgets${CMAKE_DEBUG_POSTFIX}.dll ${TARGET_DLL_PATH}
#                ${plugin_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/plugin${CMAKE_DEBUG_POSTFIX}.dll ${TARGET_DLL_PATH}

#                COMMAND ${CMAKE_COMMAND} -E copy_directory
#                ${qt_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/plugins/platforms ${TARGET_DLL_PATH}/platforms
#
#                COMMAND ${CMAKE_COMMAND} -E copy_directory
#                ${qt_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/plugins/styles ${TARGET_DLL_PATH}/styles
        )

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E remove_directory ${TARGET_DLL_PATH}/Release
        )

    endif()
endif()


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${utils_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/utils${CMAKE_CONAN_DEBUG_POSTFIX}.dll"
        "${modbus_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/modbus${CMAKE_CONAN_DEBUG_POSTFIX}.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)


#cmake_minimum_required(VERSION 3.14)
#project(modbus_example_1)
#
#include(env.cmake)
#
#find_package(utils REQUIRED)
#find_package(modbus REQUIRED)
#
##file(GLOB sources
##        include/modbus/*.h
##        include/modbus/impls/*.h
##         include/modbus/tcpclient/*.h
##        include/modbus/tcpclient/impls/*.h
##        include/modbus/factories/*.h
##        include/modbus/factories/impls/*.h
##
##        src/modbus/*.cpp
##        src/modbus/impls/*.cpp
##        src/modbus/tcpclient/*.cpp
##        src/modbus/tcpclient/impls/*.cpp
##        src/modbus/factories/*.cpp
##        src/modbus/factories/impls/*.cpp
##)
##source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}../" FILES ${sources})
#
#
#
#add_executable(${PROJECT_NAME} ${sources} main.cpp)
##target_compile_options(${PROJECT_NAME} MODBUS_EXPORT "")
#target_link_libraries(${PROJECT_NAME}
#        utils::utils
#        modbus::modbus
#)
#
#
#set (Boost_NO_SYSTEM_PATHS ON)
#set (Boost_USE_MULTITHREADED ON)
#set (Boost_USE_STATIC_LIBS ON)
#set (Boost_USE_STATIC_RUNTIME OFF)
#set (BOOST_ALL_DYN_LINK OFF)
#
## Подключаем необходимые модули. Для примера подключим program_options
#find_package (Boost REQUIRED)
#
## Если библиотека найдена, то
#if (Boost_FOUND)
#    include_directories (${Boost_INCLUDE_DIR}) # подключаем заголовочные файлы
#    target_link_libraries (${PROJECT_NAME}  ${Boost_LIBRARIES}) # подключаем библиотеку
#endif ()
#
#if(WIN32)
#    find_library(WSOCK32_LIBRARY wsock32)
#    find_library(WS2_32_LIBRARY ws2_32)
#    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
#endif()
#
