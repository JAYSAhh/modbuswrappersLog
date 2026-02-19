cmake_minimum_required(VERSION 3.14)
project(service-example-1 LANGUAGES CXX)
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
    win_copy_deps_to_target_dir(${PROJECT_NAME}
            utils::utils
            modbus::modbus
            modbuswrappers::modbuswrappers)

    include(../../cmake/routine/copy_deps.cmake)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${utils_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/utils${CMAKE_CONAN_DEBUG_POSTFIX}.dll"
        "${modbus_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/modbus${CMAKE_CONAN_DEBUG_POSTFIX}.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)