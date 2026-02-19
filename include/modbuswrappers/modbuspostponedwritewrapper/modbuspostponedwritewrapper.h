#ifndef MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSPOSTPONEDWRITEWRAPPER_MODBUSPOSTPONEDWRITEWRAPPER_H_
#define MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSPOSTPONEDWRITEWRAPPER_MODBUSPOSTPONEDWRITEWRAPPER_H_

#include <memory>

#include "modbuswrappers/modbuswrapper.h"
#include <modbuswrappers/export.h>

class AbstractModbusAsyncClientWrapper;

class MODBUSWRAPPERS_EXPORT ModbusPostponedWriteWrapper: public ModbusWrapper {
 public:
  explicit ModbusPostponedWriteWrapper(const std::shared_ptr<AbstractModbusAsyncClientWrapper> &modbus_wrapper);
  ~ModbusPostponedWriteWrapper();

  ErrorCode changeAddress(const std::string &ip, int port, int modbus_id = 1, bool reconnect = true) override;

  std::string getIP() override;
  int getPort() override;
  int getModbusID() override;

  ErrorCode connect() override;
  ErrorCode disconnect() override;
  ErrorCode isConnected(bool &is_connected, int modbus_id) override;

  void setEnabled(bool enabled, int modbus_id) override;
  bool isEnabled(int modbus_id) override;

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

  void process() override;

 private:
  struct ModbusPostponedWriteWrapperPrivate;
  ModbusPostponedWriteWrapperPrivate *_impl = nullptr;

  void updateTempHoldingRegs();
  bool validateRegNum(int reg_num);
};

#endif //MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSPOSTPONEDWRITEWRAPPER_MODBUSPOSTPONEDWRITEWRAPPER_H_
