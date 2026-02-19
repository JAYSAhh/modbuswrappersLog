#include "modbuswrappers/mocks/modbuswrappermock1.h"

#include <iostream>

#include <utils/baseutils.h>

struct ModbusWrapperMock1::ModbusWrapperMock1Private{
  std::string ip;
  int port = 4001;
  int modbus_id = 1;
};

ModbusWrapperMock1::ModbusWrapperMock1(const std::string &ip, int port, int modbus_id):
                                       _impl(new ModbusWrapperMock1Private()){
  _impl->ip = ip;
  _impl->port = port;
  _impl->modbus_id = modbus_id;
}

ModbusWrapperMock1::~ModbusWrapperMock1() {
  delete _impl;
}

std::string ModbusWrapperMock1::getIP() {
  return _impl->ip;
}

int ModbusWrapperMock1::getPort() {
  return _impl->port;
}

int ModbusWrapperMock1::getModbusID() {
  return _impl->modbus_id;
}


ErrorCode ModbusWrapperMock1::connect() {
  std::cout << "TEST CONNECT MOCK " << __func__ << std::endl;
  return SUCCESS;
}

ErrorCode ModbusWrapperMock1::disconnect() {
  std::cout << "TEST DISCONNECT MOCK "  << __func__ << std::endl;
  return SUCCESS;
}

ErrorCode ModbusWrapperMock1::isConnected(bool &is_connected, int modbus_id) {
  std::cout << "IS CONNECTED MOCK "  << __func__ << std::endl;
  is_connected = false;
  return SUCCESS;
}

ErrorCode ModbusWrapperMock1::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                  Priority priority) {
  std::cout << "READ HOLDING REGSITER MOCK " << reg_num << " " << unsigned(value)  << __func__ << std::endl;
  return SUCCESS;
}


ErrorCode ModbusWrapperMock1::readHoldingRegisters(int reg_num,
                                                   int reg_count,
                                                   std::vector<uint16_t> &values,
                                                   int modbus_id,
                                                   Priority priority) {
  std::cout << "READ HOLDING REGSITERS! MOCK " << reg_num << " " << reg_count  << __func__ << std::endl;
  return SUCCESS;
}

ErrorCode ModbusWrapperMock1::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                   Priority priority) {
  std::cout << "TEST WRITE HOLDING REGSITER MOCK " << reg_num << " " << unsigned (value) << " " << __func__ << std::endl;
  return SUCCESS;
}

ErrorCode ModbusWrapperMock1::writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                                    Priority priority) {
  std::cout << "WRITE HOLDING REGSITERS! MOCK " << reg_num << " " << value.size()  << __func__ << std::endl;
  return SUCCESS;
}

ErrorCode ModbusWrapperMock1::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                Priority priority) {
  std::cout << "READ INPUT REGSITER MOCK " << reg_num << " " << value << __func__ << std::endl;
  return SUCCESS;
}

ErrorCode ModbusWrapperMock1::readInputRegisters(int reg_num,
                                                 int reg_count,
                                                 std::vector<uint16_t> &values,
                                                 int modbus_id,
                                                 Priority priority) {
  std::cout << "READ INPUT REGSITERS! MOCK " << reg_num << " " << reg_count  << __func__ << std::endl;
  return SUCCESS;
}

void ModbusWrapperMock1::addConnectable(Connectable *connectable) {

}
void ModbusWrapperMock1::sendConnectionStatus(bool connection_status) {

}

ErrorCode ModbusWrapperMock1::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  _impl->ip = ip;
  _impl->port = port;
  _impl->modbus_id = modbus_id;
  result = SUCCESS;

  return result;
}
