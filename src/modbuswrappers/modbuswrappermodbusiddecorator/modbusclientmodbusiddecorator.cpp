#include "modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecorator.h"

#include <iostream>
#include <atomic>
#include <algorithm>

#include "modbuswrappers/connectable.h"

struct ModbusClientModbusIDDecorator::ModbusClientModbusIDDecoratorPrivate{
  int modbus_id = 1;
  std::shared_ptr<ModbusWrapper> modbus_wrapper = nullptr;

  std::atomic<bool> is_connected = false;

  std::vector<Connectable *> connectables;
};

ModbusClientModbusIDDecorator::ModbusClientModbusIDDecorator(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                                             int modbus_id):
                                                             _impl(new ModbusClientModbusIDDecoratorPrivate()){
  _impl->modbus_wrapper = modbus_wrapper;
  _impl->modbus_id = modbus_id;
}

ModbusClientModbusIDDecorator::~ModbusClientModbusIDDecorator() {
  delete _impl;
}

std::string ModbusClientModbusIDDecorator::getIP() {
  std::string result;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getIP();
  }

  return result;
}

int ModbusClientModbusIDDecorator::getPort() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getPort();
  }

  return result;
}

int ModbusClientModbusIDDecorator::getModbusID() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_id;
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::connect() {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->connect();
    bool is_connected = false;
    result = isConnected(is_connected, _impl->modbus_id);
    checkConnectionStatusByResponse(result);
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::disconnect() {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->disconnect();
    sendConnectionStatus(false);
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::isConnected(bool &is_connected, int modbus_id) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isConnected(is_connected, _impl->modbus_id);
    checkConnectionStatusByResponse(result);
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                             Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->readHoldingRegister(reg_num, value, _impl->modbus_id, priority);
    checkConnectionStatusByResponse(result);
  }

  std::cout << "READ HOLDING REGISTERS MODBUS ID IMPL1 " << result << std::endl;

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::readHoldingRegisters(int reg_num,
                                                              int reg_count,
                                                              std::vector<uint16_t> &values,
                                                              int modbus_id,
                                                              Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->readHoldingRegisters(reg_num, reg_count, values, _impl->modbus_id, priority);
    checkConnectionStatusByResponse(result);
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::writeHoldingRegister(int reg_num,
                                                              uint16_t value,
                                                              int modbus_id,
                                                              Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->writeHoldingRegister(reg_num, value, _impl->modbus_id, priority);
    checkConnectionStatusByResponse(result);
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::writeHoldingRegisters(int reg_num,
                                                               std::vector<uint16_t> value,
                                                               int modbus_id,
                                                               Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->writeHoldingRegisters(reg_num, value, _impl->modbus_id, priority);
    checkConnectionStatusByResponse(result);
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                           Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->readInputRegister(reg_num, value, _impl->modbus_id, priority);
    checkConnectionStatusByResponse(result);
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::readInputRegisters(int reg_num,
                                                            int reg_count,
                                                            std::vector<uint16_t> &values,
                                                            int modbus_id,
                                                            Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->readInputRegisters(reg_num, reg_count, values, _impl->modbus_id, priority);
    checkConnectionStatusByResponse(result);
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecorator::saveToEEPROM(int modbus_id, Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->saveToEEPROM(_impl->modbus_id, priority);
    checkConnectionStatusByResponse(result);
  }

  return result;
}

void ModbusClientModbusIDDecorator::addConnectable(Connectable *connectable) {
  std::cout << "Connectable adding... " << _impl->connectables.size() << " " << this << " " << __func__ << std::endl;
  _impl->connectables.push_back(connectable);
  std::cout << "Connectable added: " << _impl->connectables.size() << " " << this << " " << __func__ << std::endl;
}

void ModbusClientModbusIDDecorator::sendConnectionStatus(bool connection_status) {
  std::cout << "Sending to " << _impl->connectables.size() << " connectables " << this << " " << __func__ << std::endl;
  std::for_each(std::begin(_impl->connectables), std::end(_impl->connectables), [&](Connectable *connectable) {
    if (connectable != nullptr) {
      connectable->setConnectionStatus(connection_status);
    }
  });
}

void ModbusClientModbusIDDecorator::checkConnectionStatusByResponse(ErrorCode response) {
  if (response == ErrorCode::DEVICE_NOT_RESPONDING_LAN || response == ErrorCode::DEVICE_NOT_CONNECTED) {
    if (_impl->is_connected) {
      sendConnectionStatus(false);
      _impl->is_connected = false;
    }
  } else if (response == ErrorCode::SUCCESS) {
    if (!_impl->is_connected) {
      sendConnectionStatus(true);
      _impl->is_connected = true;
    }
  }
}

void ModbusClientModbusIDDecorator::setEnabled(bool enabled, int modbus_id) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->setEnabled(enabled, modbus_id);
  }

  if (modbus_id == _impl->modbus_id) {
    sendConnectionStatus(enabled);
  }
  ModbusWrapper::setEnabled(enabled, modbus_id);
}

bool ModbusClientModbusIDDecorator::isEnabled(int modbus_id) {
  bool result = ModbusWrapper::isEnabled(modbus_id);;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isEnabled(modbus_id);
  }
  return result;
}

void ModbusClientModbusIDDecorator::setIsOpened(bool is_opened, int modbus_id) {
  _impl->modbus_wrapper->setIsOpened(is_opened, modbus_id);
  ModbusWrapper::setIsOpened(is_opened, modbus_id);
}

bool ModbusClientModbusIDDecorator::isOpened(int modbus_id) {
  return _impl->modbus_wrapper->isOpened(modbus_id);
}


ErrorCode ModbusClientModbusIDDecorator::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->changeAddress(ip, port, modbus_id, reconnect);
  }

  return result;
}
