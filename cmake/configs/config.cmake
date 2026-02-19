# Чтение JSON файла
file(READ "config.json" JSON_CONTENT)
# Извлечем нужную информацию
string(REGEX MATCH "\"version\"[ \t]*:[ \t]*\"([0-9]+)\\.([0-9]+)\\.([0-9]+)\"" JSON_VERSION_MATCH "${JSON_CONTENT}")

# Извлечем части версии
string(REGEX REPLACE ".*\"version\"[ \t]*:[ \t]*\"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*" "\\1" PROJECT_VERSION_MAJOR ${JSON_VERSION_MATCH})
string(REGEX REPLACE ".*\"version\"[ \t]*:[ \t]*\"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*" "\\2" PROJECT_VERSION_MINOR ${JSON_VERSION_MATCH})
string(REGEX REPLACE ".*\"version\"[ \t]*:[ \t]*\"([0-9]+)\\.([0-9]+)\\.([0-9]+)\".*" "\\3" PROJECT_VERSION_PATCH ${JSON_VERSION_MATCH})

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
message("PROJECT_VERSION: " ${PROJECT_VERSION})

configure_file(
        "${SETTINGS_FOLDER}/config.h.in"
        "${SETTINGS_FOLDER}/config.h"
        @ONLY
)