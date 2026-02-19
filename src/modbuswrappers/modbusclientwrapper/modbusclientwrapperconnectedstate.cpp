#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperconnectedstate.h"

#include <mutex>

#include "modbuswrappers/modbusclientwrapper/modbusclientwrapper.h"
#include <utils/enumconverter.h>

struct ModbusClientWrapperConnectedState::ModbusClientWrapperConnectedStatePrivate{
  std::shared_ptr<ModbusClientWrapper> state_machine = nullptr;

  std::mutex modbus_client_mutex;
  std::shared_ptr<DynamicModbusClient> modbus_client = nullptr;

  int default_timeout = 1;
};

ModbusClientWrapperConnectedState::ModbusClientWrapperConnectedState(const std::shared_ptr<ModbusClientWrapper> &state_machine,
                                                                     const std::shared_ptr<DynamicModbusClient> &modbus_client_proxy,
                                                                     int default_timeout):
                                                                     _impl(new ModbusClientWrapperConnectedStatePrivate()){
  _impl->state_machine = state_machine;
  _impl->modbus_client = modbus_client_proxy;
  _impl->default_timeout = default_timeout;
}

ModbusClientWrapperConnectedState::~ModbusClientWrapperConnectedState() {
  delete _impl;
}

std::string ModbusClientWrapperConnectedState::getIP() {
  std::string result;

  if (_impl->modbus_client != nullptr) {
    result = _impl->modbus_client->getIP();
  }

  return result;
}

int ModbusClientWrapperConnectedState::getPort() {
  int result = 0;

  if (_impl->modbus_client != nullptr) {
    result = _impl->modbus_client->getPort();
  }

  return result;
}

int ModbusClientWrapperConnectedState::getModbusID() {
  int result = 0;

  if (_impl->modbus_client != nullptr) {
    result = 1;
  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::connect() {
  auto result = SUCCESS;
//  if (_modbus_client != nullptr) {
//    bool modbus_result = _modbus_client->connect();
//    if (modbus_result) {
//      result = SUCCESS;
//    } else {
//      result = ErrorCode::DEVICE_NOT_RESPONDING_LAN;
//    }
//  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::disconnect() {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_client != nullptr) {
    _impl->modbus_client->disconnect();
    result = SUCCESS;

    if (_impl->state_machine != nullptr) {
      _impl->state_machine->changeState(NOT_CONNECTED);
    }
  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::isConnected(bool &is_connected, int modbus_id) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_client != nullptr) {
    // TODO: Пересобрать библиотеку modbus client с тем, чтобы можно было коннект проверять в соответствие с modbus id
//    _modbus_client_mutex.lock();
    is_connected = _impl->modbus_client->isConnected(modbus_id);
//    _modbus_client_mutex.unlock();

    if (_impl->state_machine != nullptr) {
      if (is_connected) {
        _impl->state_machine->changeState(CONNECTED);
        result = SUCCESS;
      } else {
        _impl->state_machine->changeState(DISCONNECTED);
        result = DEVICE_NOT_RESPONDING_LAN;
      }
    }
  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                                 Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_client != nullptr) {
//    _modbus_client_mutex.lock();
    auto modbus_result = _impl->modbus_client->readHoldingRegister(reg_num, value, modbus_id);
    result = toErrorCode(modbus_result);
//    _modbus_client_mutex.unlock();
    checkConnectionStatusByResponse(modbus_result);
  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::readHoldingRegisters(int reg_num,
                                                                  int reg_count,
                                                                  std::vector<uint16_t> &values,
                                                                  int modbus_id,
                                                                  Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_client != nullptr) {
//    _modbus_client_mutex.lock();
    auto modbus_result = _impl->modbus_client->readHoldingRegisters(reg_num, reg_count, values, modbus_id);
    result = toErrorCode(modbus_result);
//    _modbus_client_mutex.unlock();
    checkConnectionStatusByResponse(modbus_result);
  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                                  Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_client != nullptr) {
//    _modbus_client_mutex.lock();
    auto modbus_result = _impl->modbus_client->writeHoldingRegister(reg_num, value, modbus_id);
    result = toErrorCode(modbus_result);
//    _modbus_client_mutex.unlock();
    checkConnectionStatusByResponse(modbus_result);
  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                                                   Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_client != nullptr) {
//    _modbus_client_mutex.lock();
    auto modbus_result = _impl->modbus_client->writeHoldingRegisters(reg_num, value, modbus_id);
    result = toErrorCode(modbus_result);
//    _modbus_client_mutex.unlock();
    checkConnectionStatusByResponse(modbus_result);
  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                               Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_client != nullptr) {
//    _modbus_client_mutex.lock();
    auto modbus_result = _impl->modbus_client->readInputRegister(reg_num, value, modbus_id);
    result = toErrorCode(modbus_result);
//    _modbus_client_mutex.unlock();
    checkConnectionStatusByResponse(modbus_result);
  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::readInputRegisters(int reg_num,
                                                  int reg_count,
                                                  std::vector<uint16_t> &values,
                                                  int modbus_id,
                                                                Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_client != nullptr) {
//    _modbus_client_mutex.lock();
    auto modbus_result = _impl->modbus_client->readInputRegisters(reg_num, reg_count, values, modbus_id);
    result = toErrorCode(modbus_result);
//    _modbus_client_mutex.unlock();

    checkConnectionStatusByResponse(modbus_result);
  }

  return result;
}

ErrorCode ModbusClientWrapperConnectedState::saveToEEPROM(int modbus_id, Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_client != nullptr) {
//    _modbus_client_mutex.lock();
    auto modbus_result = _impl->modbus_client->saveToEEPROM(modbus_id);
    result = toErrorCode(modbus_result);
//    _modbus_client_mutex.unlock();

    checkConnectionStatusByResponse(modbus_result);
  }

  return result;
}

void ModbusClientWrapperConnectedState::checkConnectionStatusByResponse(modbus::ModbusResult result) {
  if (_impl->state_machine != nullptr) {
    _impl->state_machine->checkConnectionStatusByResponse(result);
  }
}

void ModbusClientWrapperConnectedState::addConnectable(Connectable *connectable) {

}

void ModbusClientWrapperConnectedState::sendConnectionStatus(bool connection_status) {

}

ErrorCode ModbusClientWrapperConnectedState::changeAddress(const std::string &ip,
                                                           int port,
                                                           int modbus_id,
                                                           bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->modbus_client != nullptr) {
    result = _impl->modbus_client->changeAddress(ip, port, reconnect);
  }

  return result;
}
