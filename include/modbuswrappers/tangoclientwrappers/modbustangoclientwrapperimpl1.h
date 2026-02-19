#ifndef MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_TANGOCLIENTWRAPPERS_MODBUSTANGOCLIENTWRAPPERIMPL1_H_
#define MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_TANGOCLIENTWRAPPERS_MODBUSTANGOCLIENTWRAPPERIMPL1_H_

#include <memory>

#include <modbuswrappers/modbuswrapper.h>


class ModbusTangoClientWrapperImpl1: public ModbusWrapper {
 public:
  explicit ModbusTangoClientWrapperImpl1(/*const std::shared_ptr<>*/);

  std::string getIP() override;
  int getPort() override;
  int getModbusID() override;
  ErrorCode connect() override;
  ErrorCode disconnect() override;
  ErrorCode isConnected(bool &is_connected) override;
  ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id) override;
  ErrorCode readHoldingRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id) override;
  ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id) override;
  ErrorCode writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id) override;
  ErrorCode readInputRegister(int reg_num, uint16_t &value, int modbus_id) override;
  ErrorCode readInputRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id) override;
  void addConnectable(Connectable *connectable) override;
  void sendConnectionStatus(bool connection_status) override;
  void process() override;
};

#endif //MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_TANGOCLIENTWRAPPERS_MODBUSTANGOCLIENTWRAPPERIMPL1_H_
