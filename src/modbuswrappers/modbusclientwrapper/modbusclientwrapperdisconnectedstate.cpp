#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperdisconnectedstate.h"

#include "modbuswrappers/modbusclientwrapper/modbusclientwrapper.h"
#include <utils/services/retryer.h>

struct ModbusClientWrapperDisconnectedState::ModbusClientWrapperDisconnectedStatePrivate{
  std::shared_ptr<ModbusClientWrapper> state_machine = nullptr;

  std::recursive_mutex modbus_client_mutex;
  std::shared_ptr<DynamicModbusClient> modbus_client;

  std::shared_ptr<Retryer> retryer = nullptr;
};

ModbusClientWrapperDisconnectedState::ModbusClientWrapperDisconnectedState(const std::shared_ptr<ModbusClientWrapper> &state_machine,
                                                                           const std::shared_ptr<DynamicModbusClient>  &modbus_client,
                                                                           const std::shared_ptr<Retryer> &retryer):
                                                                           _impl(new ModbusClientWrapperDisconnectedStatePrivate()){
  _impl->state_machine = state_machine;
  _impl->modbus_client = modbus_client;
  _impl->retryer = retryer;
}

ModbusClientWrapperDisconnectedState::~ModbusClientWrapperDisconnectedState() {
  delete _impl;
}

std::string ModbusClientWrapperDisconnectedState::getIP() {
  std::string result;

  if (_impl->modbus_client != nullptr) {
    result = _impl->modbus_client->getIP();
  }

  return result;
}

int ModbusClientWrapperDisconnectedState::getPort() {
  int result = 0;

  if (_impl->modbus_client != nullptr) {
    result = _impl->modbus_client->getPort();
  }

  return result;
}

int ModbusClientWrapperDisconnectedState::getModbusID() {
  int result = 0;

  if (_impl->modbus_client != nullptr) {
    result = 1;
  }

  return result;
}

ErrorCode ModbusClientWrapperDisconnectedState::connect() {
  ErrorCode result = OPERATION_INTERRUPTED;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);

  if (_impl->modbus_client != nullptr) {
    bool connected = _impl->modbus_client->connect();
    if (connected) {
      result = SUCCESS;
      if (_impl->state_machine != nullptr) {
        _impl->state_machine->changeState(CONNECTED);
        result = SUCCESS;
      }
    }
  }
  return result;
}

ErrorCode ModbusClientWrapperDisconnectedState::reconnect() {
  ErrorCode result = OPERATION_INTERRUPTED;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);

  bool is_to_retry = true;
  if (_impl->retryer != nullptr) {
    is_to_retry = _impl->retryer->isToRetry();
  }
//  qDebug() << "Trying to reconnect " << is_to_retry;

  if (!is_to_retry) return result;
  auto modbus_wrapper_error_code = connect();
  if (modbus_wrapper_error_code == SUCCESS) {
    if (_impl->state_machine != nullptr) {
      _impl->state_machine->changeState(CONNECTED);
      result = SUCCESS;
    }

    if (_impl->retryer != nullptr) {
      _impl->retryer->reset();
    }
  }
  return result;
}

ErrorCode ModbusClientWrapperDisconnectedState::disconnect() {
  auto result = OPERATION_INTERRUPTED;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  if (_impl->modbus_client != nullptr) {
//    _modbus_client->disconnect();

    if (_impl->state_machine != nullptr) {
      _impl->state_machine->changeState(NOT_CONNECTED);
      result = SUCCESS;
    }
  }

  return result;
}

ErrorCode ModbusClientWrapperDisconnectedState::isConnected(bool &is_connected, int modbus_id) {
//  auto error_code = OPERATION_INTERRUPTED;
////  if (_state_machine != nullptr) {
//    error_code = DEVICE_NOT_RESPONDING_LAN;
//    is_connected = false;
////    if (error_code == SUCCESS) {
////      _state_machine->changeState(CONNECTED);
//      auto reconnect_error_code = reconnect();
//      if (reconnect_error_code == SUCCESS) {
//        error_code = _state_machine->isConnected(is_connected, modbus_id);
//      }
//    }
//  }
  is_connected = false;
  return DEVICE_NOT_RESPONDING_LAN;
}

ErrorCode ModbusClientWrapperDisconnectedState::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                                    Priority priority) {
  auto error_code = DEVICE_NOT_RESPONDING_LAN;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  if (_impl->state_machine != nullptr) {
      auto reconnect_error_code = reconnect();
      if (reconnect_error_code == SUCCESS) {
        error_code = _impl->state_machine->readHoldingRegister(reg_num, value, modbus_id, priority);
      }
  }
  return error_code;
}

ErrorCode ModbusClientWrapperDisconnectedState::readHoldingRegisters(int reg_num,
                                                                     int reg_count,
                                                                     std::vector<uint16_t> &values,
                                                                     int modbus_id,
                                                                     Priority priority) {
  auto error_code = DEVICE_NOT_RESPONDING_LAN;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  if (_impl->state_machine != nullptr) {
//      _state_machine->changeState(CONNECTED);
    auto reconnect_error_code = reconnect();
    std::cout << "READ HOLDING REGS: " << reg_num << " " << reg_count << " IN DISCONNECTED STATE " << reconnect_error_code << std::endl;
    if (reconnect_error_code == SUCCESS) {
      error_code = _impl->state_machine->readHoldingRegisters(reg_num, reg_count, values, modbus_id, priority);
    }
  }
  return error_code;
}

ErrorCode ModbusClientWrapperDisconnectedState::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                                     Priority priority) {
  auto error_code = DEVICE_NOT_RESPONDING_LAN;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  if (_impl->state_machine != nullptr) {
//      _state_machine->changeState(CONNECTED);
    auto reconnect_error_code = reconnect();
    if (reconnect_error_code == SUCCESS) {
      error_code = _impl->state_machine->writeHoldingRegister(reg_num, value, modbus_id, priority);
    }
  }
  return error_code;
}

ErrorCode ModbusClientWrapperDisconnectedState::writeHoldingRegisters(int reg_num,
                                                                      std::vector<uint16_t> value,
                                                                      int modbus_id,
                                                                      Priority priority) {
  auto error_code = DEVICE_NOT_RESPONDING_LAN;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  if (_impl->state_machine != nullptr) {
//      _state_machine->changeState(CONNECTED);
      auto reconnect_error_code = reconnect();
      if (reconnect_error_code == SUCCESS) {
        error_code = _impl->state_machine->writeHoldingRegisters(reg_num, value, modbus_id, priority);
      }
  }
  return error_code;
}

ErrorCode ModbusClientWrapperDisconnectedState::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                                  Priority priority) {
  auto error_code = DEVICE_NOT_RESPONDING_LAN;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  if (_impl->state_machine != nullptr) {
//      _state_machine->changeState(CONNECTED);
      auto reconnect_error_code = reconnect();
      if (reconnect_error_code == SUCCESS) {
        error_code = _impl->state_machine->readInputRegister(reg_num, value, modbus_id, priority);
      }
  }
  return error_code;
}

ErrorCode ModbusClientWrapperDisconnectedState::readInputRegisters(int reg_num,
                                                                   int reg_count,
                                                                   std::vector<uint16_t> &values,
                                                                   int modbus_id,
                                                                   Priority priority) {
  auto error_code = DEVICE_NOT_RESPONDING_LAN;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  if (_impl->state_machine != nullptr) {
//      _state_machine->changeState(CONNECTED);
      auto reconnect_error_code = reconnect();
      if (reconnect_error_code == SUCCESS) {
        error_code = _impl->state_machine->readInputRegisters(reg_num, reg_count, values, modbus_id, priority);
      }
  }
  return error_code;
}

ErrorCode ModbusClientWrapperDisconnectedState::saveToEEPROM(int modbus_id, Priority priority) {
  auto error_code = DEVICE_NOT_RESPONDING_LAN;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  if (_impl->state_machine != nullptr) {
//      _state_machine->changeState(CONNECTED);
      auto reconnect_error_code = reconnect();
      if (reconnect_error_code == SUCCESS) {
        error_code = _impl->state_machine->saveToEEPROM(modbus_id, priority);
      }
  }
  return error_code;
}


void ModbusClientWrapperDisconnectedState::addConnectable(Connectable *connectable) {

}
void ModbusClientWrapperDisconnectedState::sendConnectionStatus(bool connection_status) {

}

ErrorCode ModbusClientWrapperDisconnectedState::changeAddress(const std::string &ip,
                                                              int port,
                                                              int modbus_id,
                                                              bool reconnect) {
  auto result = OPERATION_INTERRUPTED;
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  if (_impl->modbus_client != nullptr) {
    result = _impl->modbus_client->changeAddress(ip, port, reconnect);
  }

  return result;
}

void ModbusClientWrapperDisconnectedState::process() {
  // std::unique_lock<std::recursive_mutex> lock(_impl->modbus_client_mutex);
  reconnect();
}
