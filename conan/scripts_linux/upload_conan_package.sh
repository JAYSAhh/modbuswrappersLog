# Чтение данных из config.json
PACKAGE_NAME=$(jq -r '.conan_package.name' ../../config.json)
PACKAGE_VERSION=$(jq -r '.conan_package.version' ../../config.json)
PACKAGE_USER=$(jq -r '.conan_package.user' ../../config.json)

# Обработка аргументов командной строки
for arg in "$@"; do
    case "$arg" in
        --channel=*)
            CHANNEL="${arg#*=}"  # Получаем значение после '='
            shift
            ;;
        *)
            echo "Неизвестный параметр: $arg"
            exit 1
            ;;
    esac
done

# Проверка обязательных переменных
if [ -z "$CHANNEL" ]; then
  echo "Error: variable CHANNEL is not defined. Consider adding '--channel=release / dev'"
  exit 1
fi

# Команда conan для загрузки пакета
conan upload $PACKAGE_NAME/$PACKAGE_VERSION@$PACKAGE_USER/$CHANNEL -r=dt-conan -c
