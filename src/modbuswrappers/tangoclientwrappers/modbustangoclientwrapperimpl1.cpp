#include <string>
#include <utils/baseutils.h>
#include <memory>

#include "modbuswrappers/modbuswrapper.h"
#include "modbuswrappers/tangoclientwrappers/modbustangoclientwrapperimpl1.h"

std::string ModbusTangoClientWrapperImpl1::getIP() {
  return std::string();
}

int ModbusTangoClientWrapperImpl1::getPort() {
  return 0;
}

int ModbusTangoClientWrapperImpl1::getModbusID() {
  return 0;
}

ErrorCode ModbusTangoClientWrapperImpl1::connect() {
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusTangoClientWrapperImpl1::disconnect() {
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusTangoClientWrapperImpl1::isConnected(bool &is_connected) {
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusTangoClientWrapperImpl1::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id) {
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusTangoClientWrapperImpl1::readHoldingRegisters(int reg_num,
                                                              int reg_count,
                                                              std::vector<uint16_t> &values,
                                                              int modbus_id) {
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusTangoClientWrapperImpl1::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id) {
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusTangoClientWrapperImpl1::writeHoldingRegisters(int reg_num,
                                                               std::vector<uint16_t> value,
                                                               int modbus_id) {
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusTangoClientWrapperImpl1::readInputRegister(int reg_num, uint16_t &value, int modbus_id) {
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusTangoClientWrapperImpl1::readInputRegisters(int reg_num,
                                                            int reg_count,
                                                            std::vector<uint16_t> &values,
                                                            int modbus_id) {
  return DEVICE_NOT_RESPONDING_LAN;
}

void ModbusTangoClientWrapperImpl1::addConnectable(const std::shared_ptr<Connectable> &connectable) {
  ModbusWrapper::addConnectable(connectable);
}

void ModbusTangoClientWrapperImpl1::sendConnectionStatus(bool connection_status) {
  ModbusWrapper::sendConnectionStatus(connection_status);
}

void ModbusTangoClientWrapperImpl1::process() {
  ModbusWrapper::process();
}
