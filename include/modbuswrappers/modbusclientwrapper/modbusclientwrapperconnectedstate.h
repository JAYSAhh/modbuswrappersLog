#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPERCONNECTEDSTATE_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPERCONNECTEDSTATE_H_

#include <memory>

#include <modbuswrappers/export.h>
#include <modbus/dynamic_modbus_clients/dynamic_modbus_client.h>

#include "modbuswrappers/modbuswrapper.h"

using modbus::ModbusClient;

class ModbusClientWrapper;

class MODBUSWRAPPERS_EXPORT ModbusClientWrapperConnectedState: public ModbusWrapper {
 public:
  explicit ModbusClientWrapperConnectedState(const std::shared_ptr<ModbusClientWrapper> &state_machine,
                                             const std::shared_ptr<DynamicModbusClient> &modbus_client_proxy,
                                             int default_timeout);
  ~ModbusClientWrapperConnectedState();

  ErrorCode changeAddress(const std::string &ip, int port, int modbus_id = 1, bool reconnect = true) override;

  std::string getIP() override;
  int getPort() override;
  int getModbusID() override;

  ErrorCode connect() override;
  ErrorCode disconnect() override;
  ErrorCode isConnected(bool &is_connected, int modbus_id) override;

  ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                Priority priority = {}) override;
  ErrorCode readHoldingRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id,
                                 Priority priority = {}) override;
  ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                 Priority priority = {}) override;
  ErrorCode writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                  Priority priority = {}) override;
  ErrorCode readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                              Priority priority = {}) override;
  ErrorCode readInputRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id,
                               Priority priority = {}) override;
  ErrorCode saveToEEPROM(int modbus_id,
                         Priority priority = {}) override;

  void addConnectable(Connectable *connectable) override;
  void sendConnectionStatus(bool connection_status) override;

 private:
  struct ModbusClientWrapperConnectedStatePrivate;
  ModbusClientWrapperConnectedStatePrivate *_impl = nullptr;

  void checkConnectionStatusByResponse(modbus::ModbusResult result);
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPERCONNECTEDSTATE_H_
