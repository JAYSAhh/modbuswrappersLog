#ifndef MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTDIVIDABLEWRAPPER_H_
#define MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTDIVIDABLEWRAPPER_H_

#include <memory>

#include "modbusclientwrapper.h"

#include <modbuswrappers/export.h>

class RegReadDivider;

class MODBUSWRAPPERS_EXPORT ModbusClientDividableWrapper: public ModbusWrapper {
 public:
  explicit ModbusClientDividableWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                        int holding_regs_count,
                                        int input_regs_count);
  explicit ModbusClientDividableWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                        const std::shared_ptr<RegReadDivider> &holding_regs_divider,
                                        const std::shared_ptr<RegReadDivider> &input_regs_divider);
  ~ModbusClientDividableWrapper();

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
  struct ModbusClientDividableWrapperPrivate;
  ModbusClientDividableWrapperPrivate *_impl = nullptr;
};


#endif //MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTDIVIDABLEWRAPPER_H_
