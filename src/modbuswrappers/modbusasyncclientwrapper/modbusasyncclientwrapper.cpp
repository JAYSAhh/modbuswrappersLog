

#include <algorithm>

#include <atomic>
#include <mutex>

#include <utils/services/dividers/impls/reg_read_divider_impl1.h>

#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapper.h"


static const int DEFAULT_INPUT_FIRST_REG_NUM = 0;
static const int DEFAULT_HOLDING_FIRST_REG_NUM = 0;

static const int DEFAULT_INPUT_REG_PER_REQUEST_COUNT = 30;
static const int DEFAULT_HOLDING_REG_PER_REQUEST_COUNT = 30;

struct ModbusAsyncClientWrapper::ModbusAsyncClientWrapperPrivate{
  std::shared_ptr<ModbusWrapper> modbus_wrapper = nullptr;

  int start_holding_regs_num = 0;
  int start_input_regs_num = 0;
  int holding_regs_count = 1;
  int input_regs_count = 1;

  std::vector<uint16_t> holding_regs;
  std::vector<uint16_t> input_regs;

  std::shared_ptr<RegReadDivider> holding_regs_divider = nullptr;
  std::shared_ptr<RegReadDivider> input_regs_divider = nullptr;

  std::atomic<bool> is_processed{false};
};

ModbusAsyncClientWrapper::ModbusAsyncClientWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                                   int start_holding_regs_num,
                                                   int holding_regs_count,
                                                   int start_input_regs_num,
                                                   int input_regs_count):
                                                   _impl(new ModbusAsyncClientWrapperPrivate()) {
  _impl->modbus_wrapper = modbus_wrapper;
  _impl->start_holding_regs_num = start_holding_regs_num;
  _impl->holding_regs_count = holding_regs_count;
  _impl->start_input_regs_num = start_holding_regs_num;
  _impl->input_regs_count = input_regs_count;
  _impl->holding_regs = std::vector<uint16_t>(holding_regs_count);
  _impl->input_regs = std::vector<uint16_t>(input_regs_count);
  _impl->input_regs_divider = std::make_shared<RegReadDividerImpl1>(_impl->input_regs_count);
  _impl->holding_regs_divider = std::make_shared<RegReadDividerImpl1>(_impl->holding_regs_count);
}

ModbusAsyncClientWrapper::~ModbusAsyncClientWrapper() {
  delete _impl;
}

std::string ModbusAsyncClientWrapper::getIP() {
  std::string result;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getIP();
  }

  return result;
}

int ModbusAsyncClientWrapper::getPort() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getPort();
  }

  return result;
}

int ModbusAsyncClientWrapper::getModbusID() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getModbusID();
  }

  return result;
}

void ModbusAsyncClientWrapper::process() {
////  _is_processed = true;
//
////  while (_is_processed) {
//    if (_modbus_wrapper != nullptr) {
//      _modbus_wrapper->readHoldingRegisters(0, _holding_regs_count, _holding_regs);
//      _modbus_wrapper->readInputRegisters(0, _input_regs_count, _input_regs);
//    }
//
////    std::this_thread::sleep_for(std::chrono::milliseconds(200));

//  if (_impl->modbus_wrapper != nullptr && _impl->holding_regs_divider != nullptr && _impl->input_regs_divider != nullptr) {
//    auto input_regs_read_data = _impl->input_regs_divider->getRegReadData();
//    std::vector<uint16_t> current_input_reg_vector;
//    std::for_each(std::begin(input_regs_read_data), std::end(input_regs_read_data), [&](const RegReadData &data) {
//      current_input_reg_vector.resize(data.reg_num);
//      auto error_code = _impl->modbus_wrapper->readInputRegisters(data.reg_num, data.reg_count, current_input_reg_vector);
//
//      if (error_code == SUCCESS) {
//        for (int i = 0; i < current_input_reg_vector.size(); i++) {
//          auto input_reg = data.reg_num + i;
//          if (input_reg < _impl->input_regs.size())
//            _impl->input_regs[input_reg] = current_input_reg_vector[i];
//        }
//      }
////      std::cout << "REG READ DATA " << data.reg_num << " " << data.reg_count << std::endl;
//    });
//
//
//
//    auto holding_regs_read_data = _impl->holding_regs_divider->getRegReadData();
//    std::vector<uint16_t> current_holding_reg_vector;
//    std::for_each(std::begin(holding_regs_read_data), std::end(holding_regs_read_data), [&](const RegReadData &data) {
//      current_holding_reg_vector.resize(data.reg_num);
//      auto error_code = _impl->modbus_wrapper->readHoldingRegisters(data.reg_num, data.reg_count, current_holding_reg_vector);
//
//      if (error_code == SUCCESS) {
//        for (int i = 0; i < current_holding_reg_vector.size(); i++) {
//          auto holding_reg = data.reg_num + i;
//          if (holding_reg < _impl->holding_regs.size())
//            _impl->holding_regs[holding_reg] = current_holding_reg_vector[i];
//        }
//      }
//
////      std::cout << "REG READ DATA " << data.reg_num << " " << data.reg_count << std::endl;
//    });
//  }
}

//  _is_processed = false;
//}

ErrorCode ModbusAsyncClientWrapper::connect() {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->connect();
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper::disconnect() {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->disconnect();
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper::isConnected(bool &is_connected, int modbus_id) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isConnected(is_connected, modbus_id);
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                        Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    if (reg_num >= 0 && reg_num < _impl->holding_regs.size()) {
      value = _impl->holding_regs[reg_num];
      result = SUCCESS;
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper::readHoldingRegisters(int reg_num,
                                                         int reg_count,
                                                         std::vector<uint16_t> &values,
                                                         int modbus_id,
                                                         Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  values.resize(reg_count);
  if (_impl->modbus_wrapper != nullptr) {
    if (reg_num >= 0 && reg_num < _impl->holding_regs.size()) {
      values = std::vector<uint16_t>(_impl->holding_regs.begin() + reg_num, _impl->holding_regs.begin() + reg_num + reg_count);
      result = SUCCESS;
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                         Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->writeHoldingRegister(reg_num, value, modbus_id, priority);

    // Обновление

    if (result == SUCCESS) {
      uint16_t reg_value;
      result = _impl->modbus_wrapper->readHoldingRegister(reg_num, reg_value, modbus_id, priority);

      if (result == SUCCESS) {
        if (reg_num >= 0 && reg_num < _impl->holding_regs.size()) {
          _impl->holding_regs[reg_num] = reg_value;
        }
      }
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper::writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                                          Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->writeHoldingRegisters(reg_num, value, modbus_id, priority);

    // Обновление
    if (result == SUCCESS) {
      uint16_t reg_count = (uint16_t)value.size();
      std::vector<uint16_t> reg_values;
      result = _impl->modbus_wrapper->readHoldingRegisters(reg_num, reg_count, reg_values, modbus_id, priority);

      if (result == SUCCESS) {
        if (reg_num >= 0 && reg_num < _impl->holding_regs.size() && reg_num + reg_count <= _impl->holding_regs.size()) {
          for (int i = reg_num; i < reg_num + reg_count; i++) {
            _impl->holding_regs[i] = reg_values[i - reg_num];
          }
        }
      }
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                      Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    if (reg_num >= 0 && reg_num < _impl->input_regs.size()) {
      value = _impl->input_regs[reg_num];
      result = SUCCESS;
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper::readInputRegisters(int reg_num,
                                                       int reg_count,
                                                       std::vector<uint16_t> &values,
                                                       int modbus_id,
                                                       Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  values.resize(reg_count);
  if (_impl->modbus_wrapper != nullptr) {
    if (reg_num >= 0 && reg_num < _impl->input_regs.size()) {
      values = std::vector<uint16_t>(_impl->input_regs.begin() + reg_num, _impl->input_regs.begin() + reg_num + reg_count);
      result = SUCCESS;
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper::saveToEEPROM(int modbus_id, Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->saveToEEPROM(modbus_id, priority);
  }

  return result;
}

const std::vector<uint16_t> &ModbusAsyncClientWrapper::getHoldingRegs() {
  return _impl->holding_regs;
}

const std::vector<uint16_t> &ModbusAsyncClientWrapper::getInputRegs() {
  return _impl->input_regs;
}

void ModbusAsyncClientWrapper::addConnectable(Connectable *connectable) {
  if (_impl->modbus_wrapper != nullptr) {
   _impl->modbus_wrapper->addConnectable(connectable);
  }
}

void ModbusAsyncClientWrapper::sendConnectionStatus(bool connection_status) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->sendConnectionStatus(connection_status);
  }
}
void ModbusAsyncClientWrapper::setEnabled(bool enabled, int modbus_id) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->setEnabled(enabled, modbus_id);
  }
  ModbusWrapper::setEnabled(enabled, modbus_id);
}

bool ModbusAsyncClientWrapper::isEnabled(int modbus_id) {
  bool result = ModbusWrapper::isEnabled(modbus_id);;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isEnabled(modbus_id);
  }
  return result;
}

void ModbusAsyncClientWrapper::setIsOpened(bool is_opened, int modbus_id) {
  _impl->modbus_wrapper->setIsOpened(is_opened, modbus_id);
  ModbusWrapper::setIsOpened(is_opened, modbus_id);
}

bool ModbusAsyncClientWrapper::isOpened(int modbus_id) {
  return _impl->modbus_wrapper->isOpened(modbus_id);
}

ErrorCode ModbusAsyncClientWrapper::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->changeAddress(ip, port, modbus_id, reconnect);
  }

  return result;
}

