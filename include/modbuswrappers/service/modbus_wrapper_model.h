#ifndef MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPER_TANGO_MODEL_H_
#define MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPER_TANGO_MODEL_H_



#include <map>
#include <memory>

//#include <modbuswrappers/modbuswrapper.h>

#include "modbuswrappers/extra/modbus_slave_wrapper_factory.h"
#include "modbuswrappers/extra/modbus_slave_wrapper.h"

// TODO: Перенести UID в utils
typedef std::string UID;

struct ModbusSlaveWrapperStruct {
  std::shared_ptr<ModbusSlaveWrapper> slave_wrapper = nullptr;
};

struct ModbusSlaveStruct {
  std::shared_ptr<ModbusWrapper> slave = nullptr;
  std::map<UID, ModbusSlaveWrapperStruct> slave_wrappers;
};

struct ModbusWrapperModelStruct {
  std::shared_ptr<ModbusWrapper> base_modbus_wrapper = nullptr;
  std::map<int, ModbusSlaveStruct> slaves;
};

class UIDGenerator;
class ModbusSlaveFactory;

/**
 * @brief Модель для сервиса взаимодействия с мобдас устройствами.
 * Используется сервисом ModbusWrapperTangoService.
 *
 * Основная задача -- повысить уровень абстракции от конкретной структуры данных modbus слейвов и их оберток
 * до интерфейса управления ими
 *
 * @note Сама по себе модель является "глупой". В ней намеренно отсутствует множество проверок и валидаций,
 * которые должны проходить на том уровне, где используется данная модель. Например, в методе addModbusSlave
 * не проверяется, имеется ли уже зарегистрированный modbus слейв с требуемым modbus_id, а просто происходит
 * "перезапись" старого объекта (при его наличии) на новый.
 */
class MODBUSWRAPPERS_EXPORT ModbusWrapperModel {
 public:
  ModbusWrapperModel(const std::shared_ptr<UIDGenerator> &uid_generator,
                     const std::shared_ptr<ModbusWrapper> &base_modbus_wrapper,
                     const std::shared_ptr<ModbusSlaveFactory> &modbus_slave_factory,
                     const std::shared_ptr<ModbusSlaveWrapperFactory> &modbus_slave_wrapper_factory);
  ~ModbusWrapperModel();

  std::shared_ptr<ModbusWrapper> getBaseModbusWrapper();
  std::shared_ptr<ModbusWrapper> getModbusSlave(int modbus_id);
  std::shared_ptr<ModbusSlaveWrapper> getModbusSlaveWrapper(const UID &uid);

  std::string getBaseModbusWrapperIP();
  int getBaseModbusWrapperPort();

  /**
   * @brief Меняет адрес базовой modbus шины
   */
  void changeBaseModbusWrapperAddress(const std::string &ip, int port);

  /**
   * @brief Позволяет получить все зарегистрированные modbus слейвы
   * @return вектор из modbus_id зарегистрированных modbus слейвов
   */
  std::vector<int> getModbusSlaves();

  /**
   * @brief Определяет зарегистрирован ли modbus слейв
   * @param modbus_id ID проверяемого modbus слейва
   * @return true - если modbus слейв зарегистрирован, иначе - false
   */
  bool hasModbusSlave(int modbus_id);

  /**
   * @brief Добавляет (регистрирует) modbus slave
   * @param modbus_id ID регистрируемого слейва
   */
  void addModbusSlave(int modbus_id);

  /**
   * @brief Удаляет зарегистрированный modbus слейв
   * @param modbus_id ID удаляемого modbus слейва
   */
  void removeModbusSlave(int modbus_id);

  /**
   * @brief Позволяет получить UIDы всех оберток modbus слейва
   * @param modbus_id ID modbus слейва
   * @return вектор из UIDов всех оберток modbus слейва
   */
  std::vector<UID> getModbusSlaveWrappers(int modbus_id);

  /**
   * @brief Проверяет, существует ли обертка modbus слейва
   * @note Для проверки не нужно указывать modbus_id того слейва, обертка которого проверяется.
   * Достаточно только UIDа (т.к. UIDы уникальны в рамках всей модели
   * @param uid - uid обертки
   * @return true - если обертка найдена, иначе - false
   */
  bool hasModbusSlaveWrapper(const UID &uid);


  /**
   * @brief Добавляет обертку modbus слейва
   * @param modbus_id номер слейва, для которого регистрируется текущая обертка
   * @param start_holding_reg_num начальный holding регистр
   * @param holding_regs_count количество holding регистров для чтения в пределах данной обертки
   * @param start_input_reg_num начальный input регистр
   * @param input_regs_count количество input регистров для чтения в пределах данной обертки
   * @param period_in_ms Минимальный период времени в мс между обновлениями holding и input регистров в методе process
   * @return
   */
  UID addModbusSlaveWrapper(int modbus_id,
                            int start_holding_reg_num,
                            int holding_regs_count,
                            int start_input_reg_num,
                            int input_regs_count,
                            int period_in_ms);

  /**
   * @brief Удаляет обертку modbus слейва
   * @param uid uid удаляемой обертки modbus слейва
   */
  void removeModbusSlaveWrapper(const UID &uid);

  int getModbusSlaveWrapperStartHoldingRegNum(const UID &uid);
  int getModbusSlaveWrapperHoldingRegsCount(const UID &uid);
  int getModbusSlaveWrapperStartInputRegNum(const UID &uid);
  int getModbusSlaveWrapperInputRegsCount(const UID &uid);
  int getModbusSlaveWrapperPeriodInMS(const UID &uid);

  void setModbusSlaveWrapperStartHoldingRegNum(const UID &uid, int reg_num);
  void setModbusSlaveWrapperHoldingRegsCount(const UID &uid, int holding_regs_count);
  void setModbusSlaveWrapperStartInputRegNum(const UID &uid, int reg_num);
  void setModbusSlaveWrapperInputRegsCount(const UID &uid, int input_regs_count);
  void setModbusSlaveWrapperPeriodInMS(const UID &uid, int period_in_ms);

 private:
  struct ModbusWrapperModelPrivate;
  ModbusWrapperModelPrivate *_impl = nullptr;

  ModbusSlaveWrapperStruct getModbusSlaveWrapperByUID(const UID &uid);
};

#endif //MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPER_TANGO_MODEL_H_
