#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPER_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPER_H_

#include <memory>

#include <utils/modbusutils.h>

#include "modbuswrappers/modbuswrapper.h"
#include <modbuswrappers/export.h>

// TODO: Добавить состояние NOT_CONNECTED и вынести туда логику из DISCONNECTED. Состояние DISCONNECTED сделать таким, чтобы там была логика восстановления коннекта

class MODBUSWRAPPERS_EXPORT ModbusClientWrapper: public ModbusWrapper {
 public:
  explicit ModbusClientWrapper(const std::string &ip, int port, int modbus_id);
  ~ModbusClientWrapper();

  ErrorCode changeAddress(const std::string &ip, int port, int modbus_id = 1, bool reconnect = true) override;

  std::string getIP() override;
  int getPort() override;
  int getModbusID() override;

  ErrorCode connect() override;
  ErrorCode disconnect() override;
  ErrorCode isConnected(bool &is_connected, int modbus_id) override;

  void addSlave(int modbus_id);
  void removeSlave(int modbus_id);
  void setEnabled(bool enabled, int modbus_id) override;
  bool isEnabled(int modbus_id) override;
  void setIsOpened(bool is_opened, int modbus_id) override;
  bool isOpened(int modbus_id) override;

  ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                Priority priority = {}) override;
  ErrorCode readHoldingRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id,
                                 Priority priority = {}) override;
  ErrorCode writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                  Priority priority = {}) override;
  ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                 Priority priority = {}) override;

  ErrorCode readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                              Priority priority = {}) override;
  ErrorCode readInputRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id,
                               Priority priority = {}) override;

  ErrorCode saveToEEPROM(int modbus_id,
                         Priority priority = {}) override;

  void checkConnectionStatusByResponse(modbus::ModbusResult response);

  void changeState(DeviceState state);
  void addState(const std::pair<DeviceState, std::shared_ptr<ModbusWrapper>> &state);
  void addConnectable(Connectable *connectable) override;
  void sendConnectionStatus(bool connection_status) override;
  void process() override;

 private:
  struct ModbusClientWrapperPrivate;
  ModbusClientWrapperPrivate *_impl = nullptr;
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPER_H_
