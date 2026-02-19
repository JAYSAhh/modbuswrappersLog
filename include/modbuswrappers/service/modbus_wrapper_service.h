#ifndef MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPER_TANGO_SERVICE_H_
#define MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPER_TANGO_SERVICE_H_

#include <memory>

#include <utils/baseutils.h>
#include <modbuswrappers/modbuswrapper.h>

#include <modbuswrappers/extra/modbus_slave_wrapper.h>


class MODBUSWRAPPERS_EXPORT ModbusWrapperService {
 public:
  /**
   * Позволяет получить данные TCP подключения: `IP` и `PORT`
   * @return Код ошибки и данные подключения
   */
  virtual std::tuple<ErrorCode, std::string, int> getTCPConnectionData() = 0;

  /**
   * Позволяет установить данные подключения.
   * Если новые данные подключения будут невалидны, то они не будут установлены и ничего не изменится
   * Если подключение уже было установлено, то оно будет прервано и "по возможности" восстановлено.
   * Если подключение после смены данных подключения оно не будет успешно восстановлено, то оно перейдет в состояние
   * DISCONNECTED
   *
   * @param ip
   * @param port
   * @return
   */
  virtual ErrorCode setTCPConnectionData(const std::string &ip, int port) = 0;

  /**
   * Метод, позволяющий получить состояние TCP соединения. Даже, если Modbus подключения к слейвами
   * не установлено.
   * @return Код ошибки, состояние подключения
   */
  virtual std::tuple<ErrorCode, ConnectionStatus> isConnectedTCP() = 0;

  /**
   * Метод, позволяющий выполнить подключение по TCP.
   * Подключение по Modbus осуществляется следующим образом.
   * Каждый метод process каждого modbus slave читает только 1 input регистр, чтобы держать
   * соединение. Однако, чтение ВСЕХ holding и input регистров, определенных в конфигурации слейва
   * не учитываются
   * @return
   */
  virtual ErrorCode connectTCP() = 0;

  /**
   * Метод, позволяющий порвать TCP соединение. Если девайсы были подключены и по Modbus, то и
   * эти соединения будут порваны
   * @return
   */
  virtual ErrorCode disconnectTCP() = 0;


  /**
   * Позволяет узнать состояние подключения по Modbus.
   * @param modbus_id выбранный слейв, по которому устанавливается состояние подключения.
   * @return
   */
  virtual std::tuple<ErrorCode, ConnectionStatus> isConnected(int modbus_id) = 0;

  /**
   * Метод, позволяющий установить соединение по modbus.
   *
   * Если TCP соединение уже установлено, то просто запуститься метод `process` у нужного `modbus_slave`.
   * Если TCP соединения не было установлено, то сначала будет попытка его установки, а потом, в случае успеха,
   * будет попытка запустить метод process в полной мере.
   * @param modbus_id - номер слейва, который необходимо подключить
   * @return
   *    SUCCESS - если подключение успешно удалось,
   *    DEVICE_NOT_RESPONDING_LAN - если не удалось подключиться
   */
  virtual ErrorCode connect(int modbus_id) = 0;

  /**
   * Метод, позволяющий отключиться по MODBUS. Этот метод не рвет TCP соединение,
   * он только отключает метод `process` у `modbus_wrapper` выбранного слейва.
   *
   * Если после отключения текущего слейва подключенных слейвов не остается,
   * то TCP соединение рвется.
   *
   * @param modbus_id номер слейва, который необходимо отключить
   * @return
   */
  virtual ErrorCode disconnect(int modbus_id) = 0;

  /**
   * @brief Позволяет получить информацию о наличии modbus слейвов
   * @return вектор из modbus_Id modbus слейвов
   */
  virtual std::vector<int> getModbusSlaves() = 0;

  /**
   * @brief Позволяет получить информацию о наличии конкретного modbus слейва
   * @param modbus_id ID модбас слейва
   * @return true - если слейв зарегистрирован, иначе - false
   */
  virtual bool hasModbusSlave(int modbus_id) = 0;

  /**
   * @brief Позволяет получить указатель на объект modbus слейва
   * @param modbus_id id modbus слейва
   * @return указатель на modbus слейва
   */
  virtual std::shared_ptr<ModbusWrapper> getModbusSlave(int modbus_id) = 0;

  /**
   * @brief Позволяет получить информацию о зарегистрированных обертках modbus слейва
   * @param modbus_id ID рассматриваемого modbus слейва
   * @return вектор UID'ов зарегистрированных оберток выбранного modbus слейва
   */
  virtual std::vector<UID> getModbusSlaveWrappers(int modbus_id) = 0;

  virtual std::shared_ptr<ModbusSlaveWrapper> getModbusSlaveWrapper(const UID &uid) = 0;

  /**
   * Позволяет добавить слейва
   * @param modbus_slave идентификатор слейва
   * @return
   */
  virtual ErrorCode addModbusSlave(int modbus_slave) = 0;

  /**
   * Позволяет добавить обертку слейва.
   * У одного слейва может быть несколько образов. Это нужно для того, чтобы за счет разных образов слейва
   * можно было с разным периодом читать разные holding и input регистров.
   * @param modbus_id - номер слейва
   * @param holding_regs_count - количество holding регистров для чтения
   * @param input_regs_count - количество input регистров для чтения
   * @param period_in_ms - период чтения holding и input регистров, в мс
   * @return код ошибки, уникальный идентификатор modbus_slave
   */
  virtual std::tuple<ErrorCode, std::string> addModbusSlaveWrapper(int modbus_id,
                                                                   int start_holding_reg_num,
                                                                   int holding_regs_count,
                                                                   int start_input_reg_num,
                                                                   int input_regs_count,
                                                                   int period_in_ms) = 0;

  virtual std::tuple<int, int> getModbusSlaveWrapperHoldingRegsRange(const UID &uid) = 0;
  virtual std::tuple<int, int> getModbusSlaveWrapperInputRegsRange(const UID &uid) = 0;
  virtual int getModbusSlaveWrapperMinUpdatePeriodInMs(const UID &uid) = 0;

  /**
   * Удаляет слейва со всеми его обертками
   * @param modbus_id идентификатор слейва
   * @return
   */
  virtual ErrorCode removeModbusSlave(int modbus_id) = 0;

  /**
   * Удаляет ModbusSlave ОБРАЗ по уникальному идентификатор
   * @param uid уникальный идентификатор ModbusSlave образа. Его можно получить в методe getModbusSlaves
   * @return
   */
  virtual ErrorCode removeModbusSlaveWrapper(const UID &uid) = 0;

  /**
   * @brief Позволяет добавлять объект-наблюдателя для modbus слейва. В случае изменения статуса подключения
   * modbus слейва в объекте-наблюдателя будет вызываться метод `setConnectionStatus`
   * @param modbus_id ID modbus слейва
   * @param connectable указатель на объект-наблюдатель
   */
  virtual void addConnectable(int modbus_id, Connectable *connectable) = 0;

  /**
   * Метод, исполняющийся раз в такт времени (определяется на уровне стороне, управляющей сервисом,
   * например, на стороне `Tango`.
   * В пределах этого метода периодически вызваются методы `process` для всех оберток
   */
  virtual void process() = 0;


  virtual ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id = 1,
                                        Priority priority = {}) = 0;

  virtual ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id = 1, Priority priority = {}) = 0;

  virtual ErrorCode readInputRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id = 1,
                                       Priority priority = {}) = 0;

  /**
   * @brief позволяет получить информацию о конфигурации сервиса
   *
   * Выдает информацию в виде строки о всех зарегистрированных слейвах, их обертках и базовом modbus
   * @return строку с информацией
   */
  virtual std::string getInfo();
};

#endif //MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPER_TANGO_SERVICE_H_
