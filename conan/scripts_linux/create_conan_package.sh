# Чтение данных из config.json
PACKAGE_NAME=$(jq -r '.conan_package.name' ../../config.json)
PACKAGE_VERSION=$(jq -r '.conan_package.version' ../../config.json)
PACKAGE_USER=$(jq -r '.conan_package.user' ../../config.json)

# Обработка аргументов командной строки
for arg in "$@"; do
    case "$arg" in
        --channel=*)
            CHANNEL="${arg#*=}"
            ;;
        --profile=*)
            PROFILE="${arg#*=}"
            ;;
        *)
            echo "Неизвестный параметр: $arg"
            exit 1
            ;;
    esac
done

# Проверка переменных
if [ -z "$CHANNEL" ]; then
  echo "Error: variable CHANNEL is not defined. Consider adding '--channel=release / dev'"
  echo "Or specify CMake variable: '-DCONAN_CHANNEL=release / dev' if you are loading CMake project."
  exit 1
fi

if [ -z "$PROFILE" ]; then
  echo "Error: variable PROFILE is not defined. Consider adding '--profile=release / debug'"
  echo "Or specify CMake variable: '-DCONAN_PROFILE=release / debug' if you are loading CMake project."
  exit 1
fi

# Команда conan
conan create ../.. --name=$PACKAGE_NAME --version=$PACKAGE_VERSION --user=$PACKAGE_USER --channel=$CHANNEL --profile:host=$PROFILE --profile:build=$PROFILE --build=missing -r=dt-conan

