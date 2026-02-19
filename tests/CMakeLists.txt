if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/MD>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/MD>")
endif()

# Функция для создания тестов из папки
function(add_test_subdirectory test_dir)
    # Ищем все .cpp файлы в указанной папке
    file(GLOB TEST_SOURCES "${test_dir}/*.cpp")

    if(TEST_SOURCES)
        # Создаем имя для теста на основе имени папки
        get_filename_component(TEST_NAME ${test_dir} NAME)

        # Создаем исполняемый файл
        add_executable(${TEST_NAME}_tests ${TEST_SOURCES})

        # ============ СЮДА ДОБАВИТЬ ВСЕ ЗАВИСИМОСТИ ИЗ ОСНОВОГО CMakeLists.txt ============
        target_link_libraries(${TEST_NAME}_tests
                GTest::gtest
                GTest::gtest_main
                ${PROJECT_NAME}
                utils::utils
                modbus::modbus
                threadpooling::threadpooling
        )

        if (Boost_FOUND)
            target_link_libraries (${TEST_NAME}_tests  ${Boost_LIBRARIES})
        endif ()


        if (WIN32)
            target_link_libraries(${TEST_NAME}_tests wsock32 ws2_32)
        endif()

        add_test(NAME ${TEST_NAME}_tests COMMAND ${TEST_NAME}_tests)

        set(TARGET_FILE_PATH $<TARGET_FILE_DIR:${TEST_NAME}_tests>)

        # Копируем DLL основной библиотеки
        add_custom_command(TARGET ${TEST_NAME}_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${PROJECT_NAME}>
                ${TARGET_FILE_PATH}
        )

        if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
            set(CONAN_FOLDER_POSTFIX "DEBUG")
            set(LIB_DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
        else ()
            set(CONAN_FOLDER_POSTFIX "RELEASE")
            set(LIB_DEBUG_POSTFIX )
        endif ()

        if (WIN32)
            # ============ СЮДА ДОБАВИТЬ ВСЕ БИБЛИОТЕКИ, КОТОРЫЕ ЮЗАЕМ В ТЕСТАХ .dll============
            add_custom_command(TARGET ${TEST_NAME}_tests POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${utils_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/utils${LIB_DEBUG_POSTFIX}.dll"
                    "${modbus_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/modbus${LIB_DEBUG_POSTFIX}.dll"
                    "${threadpooling_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/bin/threadpooling${LIB_DEBUG_POSTFIX}.dll"
                    ${TARGET_FILE_PATH}
            )
        else()
            # ============ СЮДА ДОБАВИТЬ ВСЕ БИБЛИОТЕКИ, КОТОРЫЕ ЮЗАЕМ В ТЕСТАХ .so============
            add_custom_command(TARGET ${TEST_NAME}_tests POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${utils_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/lib/*utils${LIB_DEBUG_POSTFIX}.so*"
                    "${modbus_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/lib/*modbus${LIB_DEBUG_POSTFIX}.so*"
                    "${threadpooling_PACKAGE_FOLDER_${CONAN_FOLDER_POSTFIX}}/lib/*threadpooling${LIB_DEBUG_POSTFIX}.so*"
                    ${TARGET_FILE_PATH}
            )
        endif()


        message(STATUS "Added test: ${TEST_NAME}_tests")
    endif()
endfunction()

# Рекурсивно ищем все подпапки
file(GLOB TEST_DIRS LIST_DIRECTORIES true "*")

foreach(test_dir ${TEST_DIRS})
    if(IS_DIRECTORY ${test_dir})
        add_test_subdirectory(${test_dir})
    endif()
endforeach()
