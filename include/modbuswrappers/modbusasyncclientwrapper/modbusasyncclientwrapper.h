#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSASYNCCLIENTWRAPPER_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSASYNCCLIENTWRAPPER_H_

#include <memory>

#include "abstractmodbusasyncclientwrapper.h"
#include <modbuswrappers/export.h>

class MODBUSWRAPPERS_EXPORT ModbusAsyncClientWrapper: public AbstractModbusAsyncClientWrapper {
 public:
  explicit ModbusAsyncClientWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                    int start_holding_regs_num,
                                    int holding_regs_count,
                                    int start_input_regs_num,
                                    int input_regs_count);
  ~ModbusAsyncClientWrapper();

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

  ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id = 1,
                                Priority priority = {}) override;
  ErrorCode readHoldingRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id = 1,
                                 Priority priority = {}) override;
  ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id = 1,
                                 Priority priority = {}) override;
  ErrorCode writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id = 1,
                                  Priority priority = {}) override;
  ErrorCode readInputRegister(int reg_num, uint16_t &value, int modbus_id = 1,
                              Priority priority = {}) override;
  ErrorCode readInputRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id = 1,
                               Priority priority = {}) override;
  ErrorCode saveToEEPROM(int modbus_id, Priority priority = {}) override;

  void addConnectable(Connectable *connectable) override;
  void sendConnectionStatus(bool connection_status) override;

  void process() override;

  const std::vector<uint16_t> &getHoldingRegs();
  const std::vector<uint16_t> &getInputRegs();

 private:
  struct ModbusAsyncClientWrapperPrivate;
  ModbusAsyncClientWrapperPrivate *_impl = nullptr;
};


#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSASYNCCLIENTWRAPPER_H_
