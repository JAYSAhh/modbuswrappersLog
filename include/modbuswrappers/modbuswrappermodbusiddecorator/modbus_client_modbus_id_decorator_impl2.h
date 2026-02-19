#ifndef MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUS_CLIENT_MODBUS_ID_DECORATOR_IMPL2_H_
#define MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUS_CLIENT_MODBUS_ID_DECORATOR_IMPL2_H_

#include <memory>

#include "modbuswrappers/modbuswrapper.h"
#include <modbuswrappers/export.h>

class Retryer;
class ModbusClientWrapper;

class MODBUSWRAPPERS_EXPORT ModbusClientModbusIDDecoratorImpl2: public ModbusWrapper {
 public:
  explicit ModbusClientModbusIDDecoratorImpl2(const std::shared_ptr<ModbusClientWrapper> &modbus_wrapper,
                                              int modbus_id,
                                              const std::shared_ptr<Retryer> &retryer);
  ~ModbusClientModbusIDDecoratorImpl2();

  ErrorCode changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) override;

  std::string getIP() override;
  int getPort() override;
  int getModbusID() override;

  void setEnabled(bool enabled, int modbus_id) override;
  bool isEnabled(int modbus_id) override;
  void setIsOpened(bool is_opened, int modbus_id) override;
  bool isOpened(int modbus_id) override;

  ErrorCode connect() override;
  ErrorCode disconnect() override;
  ErrorCode isConnected(bool &is_connected, int modbus_id = 1) override;
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
  void process() override;

 private:
  struct ModbusClientModbusIDDecoratorImpl2Private;
  ModbusClientModbusIDDecoratorImpl2Private *_impl = nullptr;
};


#endif //MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUS_CLIENT_MODBUS_ID_DECORATOR_IMPL2_H_
