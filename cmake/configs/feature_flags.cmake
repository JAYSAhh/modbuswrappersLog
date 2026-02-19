option(IS_REGISTRY_USED_FEATURE "Enabled save settings in registry (if OFF, settings will be saved
in Default.ini file right in the executable folder) -- DEFAULT OFF" OFF)


if(IS_REGISTRY_USED_FEATURE)
    add_definitions(-DIS_REGISTRY_USED_FEATURE)
endif()
