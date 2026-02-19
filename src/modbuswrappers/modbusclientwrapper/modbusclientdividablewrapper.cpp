#include "modbuswrappers/modbusclientwrapper/modbusclientdividablewrapper.h"

//#include <utils/services/dividers/impls/reg_read_divider_basic_impl1.h>
#include <iostream>

#include <utils/services/dividers/reg_read_divider.h>
#include <utils/services/dividers/impls/reg_read_divider_impl1.h>

static const int DEFAULT_INPUT_FIRST_REG_NUM = 0;
static const int DEFAULT_HOLDING_FIRST_REG_NUM = 0;

static const int DEFAULT_INPUT_REG_PER_REQUEST_COUNT = 30;
static const int DEFAULT_HOLDING_REG_PER_REQUEST_COUNT = 30;

// TODO: Добавить потом более правильную обработку ошибок в множественной записи

struct ModbusClientDividableWrapper::ModbusClientDividableWrapperPrivate{
  int input_regs_count = 0;
  int input_regs_per_request = 30;
  int holding_regs_count = 0;
  int holding_regs_per_request = 30;
  std::shared_ptr<RegReadDivider> holding_regs_divider = nullptr;
  std::shared_ptr<RegReadDivider> input_regs_divider = nullptr;
  std::shared_ptr<ModbusWrapper> modbus_wrapper = nullptr;
};

ModbusClientDividableWrapper::ModbusClientDividableWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                                           int holding_regs_count,
                                                           int input_regs_count):
                                                           _impl(new ModbusClientDividableWrapperPrivate()){
  _impl->modbus_wrapper = modbus_wrapper;
  _impl->holding_regs_count = holding_regs_count;
  _impl->input_regs_count = input_regs_count;
  _impl->input_regs_divider = std::make_shared<RegReadDividerImpl1>(DEFAULT_INPUT_REG_PER_REQUEST_COUNT);

  _impl->holding_regs_divider = std::make_shared<RegReadDividerImpl1>(DEFAULT_HOLDING_REG_PER_REQUEST_COUNT);
}
ModbusClientDividableWrapper::ModbusClientDividableWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                                           const std::shared_ptr<RegReadDivider> &holding_regs_divider,
                                                           const std::shared_ptr<RegReadDivider> &input_regs_divider):
                                                           _impl(new ModbusClientDividableWrapperPrivate()){
  _impl->modbus_wrapper = modbus_wrapper;
  _impl->holding_regs_divider = holding_regs_divider;
  _impl->input_regs_divider = input_regs_divider;
}

ModbusClientDividableWrapper::~ModbusClientDividableWrapper() {
  delete _impl;
}

ErrorCode ModbusClientDividableWrapper::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->changeAddress(ip, port, modbus_id, reconnect);
  }

  return result;
}

std::string ModbusClientDividableWrapper::getIP() {
  std::string result;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getIP();
  }

  return result;
}

int ModbusClientDividableWrapper::getPort() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getPort();
  }

  return result;
}

int ModbusClientDividableWrapper::getModbusID() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getModbusID();
  }

  return result;
}



ErrorCode ModbusClientDividableWrapper::connect() {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->connect();
  }

  return result;
}

ErrorCode ModbusClientDividableWrapper::disconnect() {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->disconnect();
  }

  return result;
}

ErrorCode ModbusClientDividableWrapper::isConnected(bool &is_connected, int modbus_id) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isConnected(is_connected, modbus_id);
  }

  return result;
}

ErrorCode ModbusClientDividableWrapper::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                            Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->readHoldingRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusClientDividableWrapper::readHoldingRegisters(int reg_num,
                                                             int reg_count,
                                                             std::vector<uint16_t> &values,
                                                             int modbus_id,
                                                             Priority priority) {

  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr && _impl->holding_regs_divider != nullptr) {
    std::vector<uint16_t> holding_regs(reg_count);
//    _holding_regs_divider->setParams(reg_num, reg_count, _holding_regs_per_request);

    auto holding_regs_read_data = _impl->holding_regs_divider->getRegReadData(reg_num, reg_count);
    std::vector<uint16_t> current_holding_reg_vector;
    auto it = std::find_if(std::begin(holding_regs_read_data), std::end(holding_regs_read_data), [&](const RegReadData &data) {
      current_holding_reg_vector.resize(data.reg_num);
      result = _impl->modbus_wrapper->readHoldingRegisters(data.reg_num, data.reg_count, current_holding_reg_vector);

      if (result) return true;

      for (int i = 0; i < current_holding_reg_vector.size(); i++) {
        auto holding_reg = data.reg_num + i;
        if (holding_reg < holding_regs.size())
          holding_regs[holding_reg] = current_holding_reg_vector[i];
      }

      return false;
    });

    values = std::move(holding_regs);
  }

  return result;
}

ErrorCode ModbusClientDividableWrapper::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                             Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->writeHoldingRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusClientDividableWrapper::writeHoldingRegisters(int reg_num, std::vector<uint16_t> values, int modbus_id,
                                                              Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  int reg_count = (int)values.size();

  if (_impl->modbus_wrapper != nullptr && _impl->holding_regs_divider != nullptr) {

//    _holding_regs_divider->setParams(reg_num, reg_count, _holding_regs_per_request);
    auto holding_regs_read_data = _impl->holding_regs_divider->getRegReadData(reg_num, reg_count);
    std::vector<uint16_t> current_holding_reg_vector;
    auto it = std::find_if(std::begin(holding_regs_read_data), std::end(holding_regs_read_data), [&](const RegReadData &data) {
      current_holding_reg_vector.resize(data.reg_num);
      int start_pos = data.reg_num - reg_num;
      int stop_pos = start_pos + data.reg_count;
      std::vector<uint16_t> current_vector_to_write(std::begin(values) + start_pos, std::begin(values) + stop_pos);
      result = _impl->modbus_wrapper->writeHoldingRegisters(data.reg_num, current_vector_to_write);
      return result != SUCCESS;
    });
  }

  return result;
}

ErrorCode ModbusClientDividableWrapper::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                          Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->readInputRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusClientDividableWrapper::readInputRegisters(int reg_num,
                                                           int reg_count,
                                                           std::vector<uint16_t> &values,
                                                           int modbus_id,
                                                           Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr && _impl->input_regs_divider != nullptr) {
    std::vector<uint16_t> input_regs(reg_count);
//    _input_regs_divider->setParams(reg_num, reg_count, _input_regs_per_request);

    auto input_regs_read_data = _impl->input_regs_divider->getRegReadData(reg_num, reg_count);
    std::vector<uint16_t> current_input_reg_vector;
    auto it = std::find_if(std::begin(input_regs_read_data), std::end(input_regs_read_data), [&](const RegReadData &data) {
      std::cout << "REG READ DATA: " << data.reg_num << " " << data.reg_count;
      current_input_reg_vector.resize(data.reg_count);
      result = _impl->modbus_wrapper->readInputRegisters(data.reg_num, data.reg_count, current_input_reg_vector);

      if (result != SUCCESS) {
        return true;
      }

      for (int i = 0; i < current_input_reg_vector.size(); i++) {
        auto input_reg = data.reg_num + i - reg_num;
        if (input_reg < input_regs.size()) {
          input_regs[input_reg] = current_input_reg_vector[i];
        }
      }

      return false;

//      std::cout << "REG READ DATA " << data.reg_num << " " << data.reg_count << std::endl;
    });

    // TODO: Потом поменять на нормальный move
    values = input_regs;
  }

  return result;
}

ErrorCode ModbusClientDividableWrapper::saveToEEPROM(int modbus_id,
                                                     Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->saveToEEPROM(modbus_id, priority);
  }

  return result;
}


void ModbusClientDividableWrapper::addConnectable(Connectable *connectable) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->addConnectable(connectable);
  }
}

void ModbusClientDividableWrapper::sendConnectionStatus(bool connection_status) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->sendConnectionStatus(connection_status);
  }
}

void ModbusClientDividableWrapper::process() {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->process();
  }
}
void ModbusClientDividableWrapper::setEnabled(bool enabled, int modbus_id) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->setEnabled(enabled, modbus_id);
  }
  ModbusWrapper::setEnabled(enabled, modbus_id);
}

bool ModbusClientDividableWrapper::isEnabled(int modbus_id) {
  bool result = ModbusWrapper::isEnabled(modbus_id);;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isEnabled(modbus_id);
  }
  return result;
}

void ModbusClientDividableWrapper::setIsOpened(bool is_opened, int modbus_id) {
  _impl->modbus_wrapper->setIsOpened(is_opened, modbus_id);
  ModbusWrapper::setIsOpened(is_opened, modbus_id);
}

bool ModbusClientDividableWrapper::isOpened(int modbus_id) {
  return _impl->modbus_wrapper->isOpened(modbus_id);
}
