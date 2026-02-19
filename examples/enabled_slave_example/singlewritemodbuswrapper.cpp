#include "singlewritemodbuswrapper.h"


struct SingleWriteModbusWrapper::Impl {
  std::mutex _mutex;
  std::shared_ptr<ModbusWrapper> _modbus_wrapper = nullptr;
};

SingleWriteModbusWrapper::SingleWriteModbusWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper):
  _impl(new Impl) {
  _impl->_modbus_wrapper = modbus_wrapper;
}

SingleWriteModbusWrapper::~SingleWriteModbusWrapper() {
  delete _impl;
}

std::string SingleWriteModbusWrapper::getIP() {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  std::string result;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->getIP();
  }

  return result;
}

int SingleWriteModbusWrapper::getPort() {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  int result = 0;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->getPort();
  }

  return result;
}

int SingleWriteModbusWrapper::getModbusID() {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  int result = 0;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->getModbusID();
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::connect() {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->connect();
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::disconnect() {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->disconnect();
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::isConnected(bool &is_connected, int modbus_id) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->isConnected(is_connected, modbus_id);
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::readHoldingRegister(int reg_num,
                                                        uint16_t &value,
                                                        int modbus_id,
                                                        Priority priority) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->readHoldingRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::readHoldingRegisters(int reg_num,
                                                         int reg_count,
                                                         std::vector<uint16_t> &values,
                                                         int modbus_id,
                                                         Priority priority) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->readHoldingRegisters(reg_num, reg_count, values, modbus_id, priority);
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::writeHoldingRegister(int reg_num,
                                                         uint16_t value,
                                                         int modbus_id,
                                                         Priority priority) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->writeHoldingRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::writeHoldingRegisters(int reg_num,
                                                          std::vector<uint16_t> value,
                                                          int modbus_id,
                                                          Priority priority) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = SUCCESS;
    auto current_error_code = OPERATION_INTERRUPTED;
    for (int i = 0; i < value.size(); i++) {
      current_error_code = _impl->_modbus_wrapper->writeHoldingRegister(reg_num + i, value[i], modbus_id, priority);
      if (current_error_code != SUCCESS) {
        result = current_error_code;
      }
    }
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::readInputRegister(int reg_num, uint16_t &value, int modbus_id, Priority priority) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->readInputRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::readInputRegisters(int reg_num,
                                                       int reg_count,
                                                       std::vector<uint16_t> &values,
                                                       int modbus_id,
                                                       Priority priority) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->readInputRegisters(reg_num, reg_count, values, modbus_id, priority);
  }

  return result;
}

ErrorCode SingleWriteModbusWrapper::saveToEEPROM(int modbus_id, Priority priority) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  auto result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->saveToEEPROM(modbus_id, priority);
  }

  return result;
}

void SingleWriteModbusWrapper::addConnectable(Connectable *connectable) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  if (_impl->_modbus_wrapper != nullptr) {
    _impl->_modbus_wrapper->addConnectable(connectable);
  }
}

void SingleWriteModbusWrapper::sendConnectionStatus(bool connection_status) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  if (_impl->_modbus_wrapper != nullptr) {
//    _impl->_modbus_wrapper->sendConnectionStatus(connection_status);
  }
}

void SingleWriteModbusWrapper::process() {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  if (_impl->_modbus_wrapper != nullptr) {
    _impl->_modbus_wrapper->process();
  }
}

ErrorCode SingleWriteModbusWrapper::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->_modbus_wrapper != nullptr) {
    result = _impl->_modbus_wrapper->changeAddress(ip, port, modbus_id, reconnect);
  }

  return result;
}

void SingleWriteModbusWrapper::setEnabled(bool enabled, int modbus_id) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  if (_impl->_modbus_wrapper != nullptr) {
    _impl->_modbus_wrapper->setEnabled(enabled, modbus_id);
  }

  ModbusWrapper::setEnabled(enabled, modbus_id);
}

bool SingleWriteModbusWrapper::isEnabled(int modbus_id) {
  std::unique_lock<std::mutex> lock(_impl->_mutex);
  if (_impl->_modbus_wrapper != nullptr) {
    _impl->_modbus_wrapper->isEnabled(modbus_id);
  }

  return ModbusWrapper::isEnabled(modbus_id);
}
