#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUSCLIENTMODBUSIDDECORATOR_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUSCLIENTMODBUSIDDECORATOR_H_

#include <memory>

#include "modbuswrappers/modbuswrapper.h"
#include <modbuswrappers/export.h>

class MODBUSWRAPPERS_EXPORT ModbusClientModbusIDDecorator: public ModbusWrapper {
 public:
  explicit ModbusClientModbusIDDecorator(const std::shared_ptr<ModbusWrapper> &modbus_wrapper, int modbus_id);
  ~ModbusClientModbusIDDecorator();

  ErrorCode changeAddress(const std::string &ip, int port, int modbus_id = 1, bool reconnect = true) override;

  std::string getIP() override;
  int getPort() override;
  int getModbusID() override;

  ErrorCode connect() override;
  ErrorCode disconnect() override;
  ErrorCode isConnected(bool &is_connected, int modbus_id) override;

  void setEnabled(bool enabled, int modbus_id) override;
  bool isEnabled(int modbus_id) override;
  void setIsOpened(bool is_opened, int modbus_id) override;
  bool isOpened(int modbus_id) override;

  ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                Priority priority = {}) override;
  ErrorCode readHoldingRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id,
                                 Priority priority = {}) override;
  ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id = 1,
                                 Priority priority = {}) override;
  ErrorCode writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                  Priority priority = {}) override;
  ErrorCode readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                              Priority priority = {}) override;
  ErrorCode readInputRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id,
                               Priority priority = {}) override;
  ErrorCode saveToEEPROM(int modbus_id, Priority priority = {}) override;

  void addConnectable(Connectable *connectable) override;
  void sendConnectionStatus(bool connection_status) override;

  void checkConnectionStatusByResponse(ErrorCode response);

 private:
  struct ModbusClientModbusIDDecoratorPrivate;
  ModbusClientModbusIDDecoratorPrivate *_impl = nullptr;
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUSCLIENTMODBUSIDDECORATOR_H_
