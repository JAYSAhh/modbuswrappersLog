#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPERNOTCONNECTEDSTATE_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPERNOTCONNECTEDSTATE_H_

#include <memory>

#include <modbus/dynamic_modbus_clients/dynamic_modbus_client.h>

#include "modbusclientwrapper.h"

using modbus::ModbusClient;


class MODBUSWRAPPERS_EXPORT ModbusClientWrapperNotConnectedState: public ModbusWrapper {
 public:
  explicit ModbusClientWrapperNotConnectedState(const std::shared_ptr<ModbusClientWrapper> &state_machine,
                                                const std::shared_ptr<DynamicModbusClient> &modbus_client);
  ~ModbusClientWrapperNotConnectedState();

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
  ErrorCode saveToEEPROM(int modbus_id, Priority priority = {}) override;

  void addConnectable(Connectable *connectable) override;
  void sendConnectionStatus(bool connection_status) override;

 private:
  struct ModbusClientWrapperNotConnectedStatePrivate;
  ModbusClientWrapperNotConnectedStatePrivate *_impl = nullptr;
};
#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPERNOTCONNECTEDSTATE_H_
