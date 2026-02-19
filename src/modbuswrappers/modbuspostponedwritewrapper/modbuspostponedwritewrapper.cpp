#include "modbuswrappers/modbuspostponedwritewrapper/modbuspostponedwritewrapper.h"

#include "modbuswrappers/modbusasyncclientwrapper/abstractmodbusasyncclientwrapper.h"

struct ModbusPostponedWriteWrapper::ModbusPostponedWriteWrapperPrivate{
  int holding_regs_count = 0;
  std::vector<uint16_t> tmp_holding_regs;

  std::shared_ptr<AbstractModbusAsyncClientWrapper> modbus_wrapper = nullptr;

  bool start_postponing = false;
};

ModbusPostponedWriteWrapper::ModbusPostponedWriteWrapper(const std::shared_ptr<AbstractModbusAsyncClientWrapper> &modbus_wrapper):
                                                         _impl(new ModbusPostponedWriteWrapperPrivate()){
  _impl->modbus_wrapper = modbus_wrapper;
}

ModbusPostponedWriteWrapper::~ModbusPostponedWriteWrapper() {
  delete _impl;
}

std::string ModbusPostponedWriteWrapper::getIP() {
  std::string result;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getIP();
  }

  return result;
}

int ModbusPostponedWriteWrapper::getPort() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getPort();
  }

  return result;
}

int ModbusPostponedWriteWrapper::getModbusID() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getModbusID();
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::connect() {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->connect();
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::disconnect() {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->disconnect();
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::isConnected(bool &is_connected, int modbus_id) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isConnected(is_connected, modbus_id);
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                           Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {

    if (_impl->start_postponing) {
      if (validateRegNum(reg_num)) {
        value = _impl->tmp_holding_regs[reg_num];
        result = SUCCESS;
      }
    } else {
      result = _impl->modbus_wrapper->readHoldingRegister(reg_num, value, modbus_id, priority);
    }
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::readHoldingRegisters(int reg_num,
                                                            int reg_count,
                                                            std::vector<uint16_t> &values,
                                                            int modbus_id,
                                                            Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {

    if (_impl->start_postponing) {
      values.resize(reg_count);

      for (int i = reg_num; i < reg_num + reg_count; i++) {
        if (validateRegNum(i) && i - reg_num < values.size()) {
          values[i - reg_num] = _impl->tmp_holding_regs[i];
        }
      }

      result = SUCCESS;

    } else {
      result = _impl->modbus_wrapper->readHoldingRegisters(reg_num, reg_count, values, modbus_id, priority);
    }
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                            Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;
  updateTempHoldingRegs();

  if (validateRegNum(reg_num)) {
    _impl->tmp_holding_regs[reg_num] = value;
    result = SUCCESS;
  } else {
    result = INVALID_REQUEST;
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                                             Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;
  updateTempHoldingRegs();

  auto reg_count = (int)value.size();
  for (int i = reg_num; i < reg_num + reg_count; i++) {
    result = writeHoldingRegister(i, value[i - reg_num], modbus_id, priority);
    if (result != SUCCESS) {
      return result;
    }
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                         Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->readInputRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::readInputRegisters(int reg_num,
                                                          int reg_count,
                                                          std::vector<uint16_t> &values,
                                                          int modbus_id,
                                                          Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->readInputRegisters(reg_num, reg_count, values, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusPostponedWriteWrapper::saveToEEPROM(int modbus_id, Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->saveToEEPROM(modbus_id, priority);
  }

  return result;
}

void ModbusPostponedWriteWrapper::addConnectable(Connectable *connectable) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->addConnectable(connectable);
  }
}

void ModbusPostponedWriteWrapper::sendConnectionStatus(bool connection_status) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->sendConnectionStatus(connection_status);
  }
}

void ModbusPostponedWriteWrapper::process() {
  if (_impl->start_postponing) {
    _impl->start_postponing = false;
    _impl->modbus_wrapper->writeHoldingRegisters(0, _impl->tmp_holding_regs);
  }
}

void ModbusPostponedWriteWrapper::updateTempHoldingRegs() {
  if (!_impl->start_postponing) {
    _impl->start_postponing = true;

    if (_impl->modbus_wrapper != nullptr) {
      _impl->modbus_wrapper->process();
      _impl->tmp_holding_regs = _impl->modbus_wrapper->getHoldingRegs();
    }
  }
}

bool ModbusPostponedWriteWrapper::validateRegNum(int reg_num) {
  bool result = reg_num >= 0 && reg_num < _impl->tmp_holding_regs.size();
  return result;
}

void ModbusPostponedWriteWrapper::setEnabled(bool enabled, int modbus_id) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->setEnabled(enabled, modbus_id);
  }
  ModbusWrapper::setEnabled(enabled, modbus_id);
}

bool ModbusPostponedWriteWrapper::isEnabled(int modbus_id) {
  bool result = ModbusWrapper::isEnabled(modbus_id);;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isEnabled(modbus_id);
  }
  return result;
}

ErrorCode ModbusPostponedWriteWrapper::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->changeAddress(ip, port, modbus_id, reconnect);
  }

  return result;
}
