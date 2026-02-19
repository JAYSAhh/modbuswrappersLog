#include "modbuswrappers/modbusclientwrapper/modbusclientwrappernotconnectedstate.h"

#include <mutex>

struct ModbusClientWrapperNotConnectedState::ModbusClientWrapperNotConnectedStatePrivate{
  std::shared_ptr<ModbusClientWrapper> state_machine = nullptr;

  std::mutex modbus_client_mutex = {};
  std::shared_ptr<DynamicModbusClient> modbus_client = {};
};

ModbusClientWrapperNotConnectedState::ModbusClientWrapperNotConnectedState(const std::shared_ptr<ModbusClientWrapper> &state_machine,
                                                                           const std::shared_ptr<DynamicModbusClient> &modbus_client):
                                                                           _impl(new ModbusClientWrapperNotConnectedStatePrivate()){
  _impl->state_machine = state_machine;
  _impl->modbus_client = modbus_client;
}

ModbusClientWrapperNotConnectedState::~ModbusClientWrapperNotConnectedState() {
  delete _impl;
}

std::string ModbusClientWrapperNotConnectedState::getIP() {
  std::string result;

  if (_impl->modbus_client != nullptr) {
    result = _impl->modbus_client->getIP();
  }

  return result;
}

int ModbusClientWrapperNotConnectedState::getPort() {
  int result = 0;

  if (_impl->modbus_client != nullptr) {
    result = _impl->modbus_client->getPort();
  }

  return result;
}

int ModbusClientWrapperNotConnectedState::getModbusID() {
  int result = 0;

  if (_impl->modbus_client != nullptr) {
    result = 1;
  }

  return result;
}

ErrorCode ModbusClientWrapperNotConnectedState::connect() {
  ErrorCode result = OPERATION_INTERRUPTED;
  std::unique_lock<std::mutex> lock(_impl->modbus_client_mutex);

  if (_impl->modbus_client != nullptr) {
    bool connected = _impl->modbus_client->connect();
    if (connected) {
      result = SUCCESS;
      if (_impl->state_machine != nullptr) {
        _impl->state_machine->changeState(CONNECTED);
      }
    } else {
      result = DEVICE_NOT_CONNECTED;
      if (_impl->state_machine != nullptr) {
        _impl->state_machine->changeState(DISCONNECTED);
      }
    }
  }
  return result;
}

ErrorCode ModbusClientWrapperNotConnectedState::disconnect() {
  ErrorCode result = OPERATION_INTERRUPTED;
  return result;
}

ErrorCode ModbusClientWrapperNotConnectedState::isConnected(bool &is_connected, int modbus_id) {
//  auto result = OPERATION_INTERRUPTED;
//  if (_modbus_client != nullptr) {
//    // TODO: Пересобрать библиотеку modbus client с тем, чтобы можно было коннект проверять в соответствие с modbus id
//    _modbus_client_mutex.lock();
//    is_connected = _modbus_client->isConnected(modbus_id);
//    _modbus_client_mutex.unlock();
//
//    if (_state_machine != nullptr) {
//      if (is_connected) {
//        _state_machine->changeState(CONNECTED);
//      } else {
//        _state_machine->changeState(DISCONNECTED);
//      }
//    }
//
//    result = SUCCESS;
//  }
  is_connected = false;
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusClientWrapperNotConnectedState::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                                    Priority priority) {
  ErrorCode result = DEVICE_NOT_RESPONDING_LAN;
  return result;
}

ErrorCode ModbusClientWrapperNotConnectedState::readHoldingRegisters(int reg_num,
                                                                     int reg_count,
                                                                     std::vector<uint16_t> &values,
                                                                     int modbus_id,
                                                                     Priority priority) {
  ErrorCode result = DEVICE_NOT_RESPONDING_LAN;
  return result;
}

ErrorCode ModbusClientWrapperNotConnectedState::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                                     Priority priority) {
  ErrorCode result = DEVICE_NOT_RESPONDING_LAN;
  return result;
}

ErrorCode ModbusClientWrapperNotConnectedState::writeHoldingRegisters(int reg_num,
                                                                      std::vector<uint16_t> value,
                                                                      int modbus_id,
                                                                      Priority priority) {
  ErrorCode result = DEVICE_NOT_RESPONDING_LAN;
  return result;
}

ErrorCode ModbusClientWrapperNotConnectedState::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                                  Priority priority) {
  ErrorCode result = DEVICE_NOT_RESPONDING_LAN;
  return result;
}

ErrorCode ModbusClientWrapperNotConnectedState::readInputRegisters(int reg_num,
                                                                   int reg_count,
                                                                   std::vector<uint16_t> &values,
                                                                   int modbus_id,
                                                                   Priority priority) {
  ErrorCode result = DEVICE_NOT_RESPONDING_LAN;
  return result;
}

ErrorCode ModbusClientWrapperNotConnectedState::saveToEEPROM(int modbus_id, Priority priority) {
  ErrorCode result = DEVICE_NOT_RESPONDING_LAN;
  return result;
}


void ModbusClientWrapperNotConnectedState::addConnectable(Connectable *connectable) {

}
void ModbusClientWrapperNotConnectedState::sendConnectionStatus(bool connection_status) {

}

ErrorCode ModbusClientWrapperNotConnectedState::changeAddress(const std::string &ip,
                                                              int port,
                                                              int modbus_id,
                                                              bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->modbus_client != nullptr) {
    result = _impl->modbus_client->changeAddress(ip, port, reconnect);
  }

  return result;
}
