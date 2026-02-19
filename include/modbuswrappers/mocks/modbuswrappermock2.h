#ifndef MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MOCKS_MODBUSWRAPPERMOCK2_H_
#define MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MOCKS_MODBUSWRAPPERMOCK2_H_

#include <map>

#include "modbuswrappers/modbuswrapper.h"
#include <modbuswrappers/export.h>
#include <utils/modbusutils.h>

using RegCardType = std::map<int, std::vector<uint16_t>>;

class MODBUSWRAPPERS_EXPORT ModbusWrapperMock2: public ModbusWrapper {
 public:
  explicit ModbusWrapperMock2(const std::string &ip, int port, int modbus_id);
  ~ModbusWrapperMock2() override;

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
  void addConnectable(Connectable *connectable) override;
  void sendConnectionStatus(bool connection_status) override;
  void process() override;

  ErrorCode saveToEEPROM(int modbus_id, Priority priority = {}) override;

 private:
  struct ModbusWrapperMock2Private;
  ModbusWrapperMock2Private *_impl = nullptr;

  ErrorCode readRegCard(int reg_num, uint16_t &value, int modbus_id, RegCardType &reg_card);
  ErrorCode writeRegCard(int reg_num, uint16_t value, int modbus_id, RegCardType &reg_card);

  void checkConnectionStatusByResponse(modbus::ModbusResult response);

  void delaySim();
};

#endif //MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MOCKS_MODBUSWRAPPERMOCK2_H_
