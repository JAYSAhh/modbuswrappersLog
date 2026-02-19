#include "modbuswrappers/extra/impls/modbus_slave_wrapper_impl1.h"

#include <iostream>
#include <chrono>
#include <mutex>

// TODO: refactor порядок методов

struct ModbusSlaveWrapperImpl1::ModbusSlaveWrapperImpl1Private{
  std::recursive_mutex mutex;

  std::shared_ptr<AbstractModbusAsyncClientWrapper> base_modbus_wrapper = nullptr;

  int period_in_ms = 0;
  std::chrono::system_clock::time_point last_time_processed;

  std::vector<uint16_t> holding_regs;
  std::vector<uint16_t> input_regs;
};

ModbusSlaveWrapperImpl1::ModbusSlaveWrapperImpl1(const std::shared_ptr<AbstractModbusAsyncClientWrapper> &base_modbus_wrapper,
                                                 int period_in_ms):
                                                 _impl(new ModbusSlaveWrapperImpl1Private()) {
  _impl->base_modbus_wrapper = base_modbus_wrapper;
  _impl->period_in_ms = period_in_ms;
  auto now = std::chrono::system_clock::now();
  _impl->last_time_processed = now;
}

ModbusSlaveWrapperImpl1::~ModbusSlaveWrapperImpl1() {
 delete _impl;
}

void ModbusSlaveWrapperImpl1::process() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (_impl->base_modbus_wrapper != nullptr) {
    auto now = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _impl->last_time_processed).count() >= _impl->period_in_ms) {
      _impl->last_time_processed = now;
      if (_impl->base_modbus_wrapper != nullptr) {
        std::cout << "PROCESSED";
        _impl->base_modbus_wrapper->process();
      }
    }

//    ModbusWrapper::process();
  }
}


//ErrorCode ModbusSlaveWrapperImpl1::changeAddress(const std::string &ip, int port, bool reconnect) {
//  ErrorCode result = OPERATION_INTERRUPTED;
//
//  if (_base_modbus_wrapper != nullptr) {
//    result = _base_modbus_wrapper->changeAddress(ip, port, reconnect);
//  }
//
//  return result;
//}

std::string ModbusSlaveWrapperImpl1::getIP() {
  std::string result;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->getIP();
  }

  return result;
}

int ModbusSlaveWrapperImpl1::getPort() {
  int result = 0;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->getPort();
  }

  return result;
}

int ModbusSlaveWrapperImpl1::getModbusID() {
  int result = 0;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->getModbusID();
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::connect() {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->connect();
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::disconnect() {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->disconnect();
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::isConnected(bool &is_connected, int modbus_id) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->isConnected(is_connected, modbus_id);
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id, Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->readHoldingRegister(reg_num, value, modbus_id, priority);
  }

  std::cout << "READ HOLDING REGISTER IN MODBUS SLAVE WRAPPER IMPL1 " << std::endl;

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::readHoldingRegisters(int reg_num,
                                                        int reg_count,
                                                        std::vector<uint16_t> &values,
                                                        int modbus_id,
                                                        Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->readHoldingRegisters(reg_num, reg_count, values, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id, Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->writeHoldingRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::writeHoldingRegisters(int reg_num,
                                                         std::vector<uint16_t> value,
                                                         int modbus_id,
                                                         Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->writeHoldingRegisters(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::readInputRegister(int reg_num, uint16_t &value, int modbus_id, Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->readInputRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::readInputRegisters(int reg_num,
                                                      int reg_count,
                                                      std::vector<uint16_t> &values,
                                                      int modbus_id,
                                                      Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->readInputRegisters(reg_num, reg_count, values, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::saveToEEPROM(int modbus_id, Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->saveToEEPROM(modbus_id, priority);
  }

  return result;
}

void ModbusSlaveWrapperImpl1::addConnectable(Connectable *connectable) {
  if (_impl->base_modbus_wrapper != nullptr) {
    _impl->base_modbus_wrapper->addConnectable(connectable);
  }
}

void ModbusSlaveWrapperImpl1::sendConnectionStatus(bool connection_status) {
  if (_impl->base_modbus_wrapper != nullptr) {
    _impl->base_modbus_wrapper->sendConnectionStatus(connection_status);
  }
}

const std::vector<uint16_t> &ModbusSlaveWrapperImpl1::getHoldingRegs() {
  if (_impl->base_modbus_wrapper != nullptr) {
    _impl->holding_regs = _impl->base_modbus_wrapper->getHoldingRegs();
  }

  return _impl->holding_regs;
}

const std::vector<uint16_t> &ModbusSlaveWrapperImpl1::getInputRegs() {
  if (_impl->base_modbus_wrapper != nullptr) {
    _impl->input_regs = _impl->base_modbus_wrapper->getInputRegs();
  }

  return _impl->input_regs;
}

int ModbusSlaveWrapperImpl1::getHoldingRegsCount() {
  int result = 0;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->getHoldingRegsCount();
  }

  return result;
}

int ModbusSlaveWrapperImpl1::getInputRegsCount() {
  int result = 0;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->getInputRegsCount();
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::setHoldingRegsCount(int reg_count) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->setHoldingRegsCount(reg_count);
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::setInputRegsCount(int reg_count) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->setInputRegsCount(reg_count);
  }

  return result;
}

int ModbusSlaveWrapperImpl1::getStartHoldingRegsNum() {
  int result = 0;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->getStartHoldingRegsNum();
  }

  return result;
}

int ModbusSlaveWrapperImpl1::getStartInputRegsNum() {
  int result = 0;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->getStartInputRegsNum();
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::setStartHoldingRegsNum(int reg_num) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->setStartHoldingRegsNum(reg_num);
  }

  return result;
}

ErrorCode ModbusSlaveWrapperImpl1::setStartInputRegsNum(int reg_num) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->setStartInputRegsNum(reg_num);
  }

  return result;
}

int ModbusSlaveWrapperImpl1::getMinUpdatePeriodInMs() {
  return _impl->period_in_ms;
}

ErrorCode ModbusSlaveWrapperImpl1::setMinUpdatePeriodInMs(int period_in_ms) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (period_in_ms >= 0) {
    _impl->period_in_ms = period_in_ms;
    result = SUCCESS;
  }

  return result;
}
ErrorCode ModbusSlaveWrapperImpl1::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->base_modbus_wrapper != nullptr) {
    result = _impl->base_modbus_wrapper->changeAddress(ip, port, modbus_id, reconnect);
  }

  return result;
}
