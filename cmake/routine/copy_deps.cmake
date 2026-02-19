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
endif ()