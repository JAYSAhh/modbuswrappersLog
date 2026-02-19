#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapper2.h"

#include <iostream>

#include <atomic>
#include <mutex>
#include <thread>


static const int DEFAULT_INPUT_FIRST_REG_NUM = 0;
static const int DEFAULT_HOLDING_FIRST_REG_NUM = 0;

static const int DEFAULT_INPUT_REG_PER_REQUEST_COUNT = 30;
static const int DEFAULT_HOLDING_REG_PER_REQUEST_COUNT = 30;

static const int MAX_PRIORITY = 1000;

struct ModbusAsyncClientWrapper2::ModbusAsyncClientWrapper2Private{
  std::recursive_mutex mutex;

  std::shared_ptr<ModbusWrapper> modbus_wrapper = nullptr;

  int first_holding_reg_num = 0;
  int first_input_reg_num = 0;
  int holding_regs_count = 1;
  int input_regs_count = 1;

  std::vector<uint16_t> holding_regs;
  std::vector<uint16_t> input_regs;

  std::atomic<bool> is_processed{false};

  bool is_connected = false;
};

ModbusAsyncClientWrapper2::ModbusAsyncClientWrapper2(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                                     int start_holding_regs_num,
                                                     int holding_regs_count,
                                                     int start_input_regs_num,
                                                     int input_regs_count):
                                                     _impl(new ModbusAsyncClientWrapper2Private()){
  _impl->modbus_wrapper = modbus_wrapper;
  _impl->first_holding_reg_num = start_holding_regs_num;
  _impl->holding_regs_count = holding_regs_count;
  _impl->first_input_reg_num = start_input_regs_num;
  _impl->input_regs_count = input_regs_count;
  _impl->holding_regs = std::vector<uint16_t>(holding_regs_count);
  _impl->input_regs = std::vector<uint16_t>(input_regs_count);
}

ModbusAsyncClientWrapper2::~ModbusAsyncClientWrapper2() {
  delete _impl;
}

ErrorCode ModbusAsyncClientWrapper2::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->changeAddress(ip, port, modbus_id, reconnect);
  }

  return result;
}

std::string ModbusAsyncClientWrapper2::getIP() {
  std::string result;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getIP();
  }

  return result;
}

int ModbusAsyncClientWrapper2::getPort() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getPort();
  }

  return result;
}

int ModbusAsyncClientWrapper2::getModbusID() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getModbusID();
  }

  return result;
}

void ModbusAsyncClientWrapper2::checkConnectionStatusByResponse(ErrorCode response) {
  if (response == DEVICE_NOT_RESPONDING_LAN || response == DEVICE_NOT_CONNECTED) {
    _impl->is_connected = false;
  } else {
    _impl->is_connected = true;
  }
}

void ModbusAsyncClientWrapper2::process() {
//  std::cout << __func__ << " " << std::this_thread::get_id() <<  std::endl;
std::cout << "PROCESS INSIDE ASYNC" << std::endl;
  if (_impl->modbus_wrapper != nullptr) {
    auto thread_id = std::this_thread::get_id();
    std::cout << "PROCESS WAIT " << thread_id << " " << __func__;
    std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
    std::cout << "PROCESS INSIDE " << thread_id << " " << __func__;
    // На этом уровне необходимо исполнить логику более базовых modbus врапперов. Например, восстановление подключения
    // на уровне ModbusClientWrapper. Когда подключение потеряно, то всякая передача управления с этого уровня декораторов
    // блокируется для передачи более базовым уровням. Здесь же необходимо переодически восстанавливать значение, потеря
    // произошла не по воле пользователя
    // TODO: раскомментить вызов метода process (уже раскомментировано, оставляю комментарий, чтобы помнить об этом)!
      _impl->modbus_wrapper->process();
//      bool is_connected = false;
//      isConnected(is_connected);

    std::cout << "PROCESS END (BEFORE FIRST UNLOCK) " << thread_id << " " << __func__;

//    auto error_code = _modbus_wrapper->isConnected(_is_connected);
//    lock.unlock();
//    if (error_code == SUCCESS) {
//      error_code = _modbus_wrapper->readInputRegisters(0, _input_regs_count, _input_regs);
//      error_code = _modbus_wrapper->readHoldingRegisters(0, _holding_regs_count, _holding_regs);
//    }


    std::vector<uint16_t> input_regs_current;
    auto error_code = _impl->modbus_wrapper->readInputRegisters(_impl->first_input_reg_num, _impl->input_regs_count, input_regs_current, 1, {true, MAX_PRIORITY});
//    std::cout << "error code is: " << error_code << " " << getModbusID() << std::endl;
    if (error_code == SUCCESS && input_regs_current.size() == _impl->input_regs.size()) {
//      std::unique_lock<std::recursive_mutex> lock(_mutex);
      _impl->input_regs = std::move(input_regs_current);
    }

    checkConnectionStatusByResponse(error_code);
    if (!_impl->is_connected) {
      return;
    }

    std::vector<uint16_t> holding_regs_current;
    error_code = _impl->modbus_wrapper->readHoldingRegisters(_impl->first_holding_reg_num, _impl->holding_regs_count, holding_regs_current, 1, {true, MAX_PRIORITY});
//    std::cout << "error code is: " << error_code << " " << getModbusID() << std::endl;
    if (error_code == SUCCESS && holding_regs_current.size() == _impl->holding_regs.size()) {
//      std::unique_lock<std::recursive_mutex> lock(_mutex);
      _impl->holding_regs = std::move(holding_regs_current);
    }

    checkConnectionStatusByResponse(error_code);

    // TODO: Добавить потом логирование и обработку ошибок

  }
  std::cout << "PROCESS INSIDE ASYNC ENDED" << std::endl;
}


ErrorCode ModbusAsyncClientWrapper2::connect() {
//  std::cout << __func__ << " " << std::this_thread::get_id() <<  std::endl;
  auto thread_id = std::this_thread::get_id();
  std::cout << "Connect WAIT " << thread_id << " " << __func__;
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  std::cout << "Connect INSIDE " << thread_id << " " << __func__;
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    _impl->is_connected = true;
    result = _impl->modbus_wrapper->connect();
    checkConnectionStatusByResponse(result);
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper2::disconnect() {
//  std::cout << __func__ << " " << std::this_thread::get_id() <<  std::endl;
  auto thread_id = std::this_thread::get_id();
  std::cout << "DisConnect WAIT " << thread_id << " " << __func__;
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  std::cout << "DisConnect INSIDE " << thread_id << " " << __func__;
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    _impl->is_connected = false;
    result = _impl->modbus_wrapper->disconnect();
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper2::isConnected(bool &is_connected, int modbus_id) {
//  std::cout << __func__ << " " << std::this_thread::get_id() <<  std::endl;
//   std::unique_lock<std::recursive_mutex> lock(_mutex);
  auto result = OPERATION_INTERRUPTED;
//  if (_modbus_wrapper != nullptr) {
//    result = _modbus_wrapper->isConnected(is_connected);
//  }

  is_connected = _impl->is_connected;

  return result;
}

ErrorCode ModbusAsyncClientWrapper2::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                         Priority priority) {
//  std::cout << __func__ << " " << std::this_thread::get_id() <<  std::endl;
  if (!_impl->is_connected) return DEVICE_NOT_CONNECTED;

  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    auto reg_num_vector_id = reg_num - _impl->first_holding_reg_num;
    if (reg_num_vector_id >= 0 && reg_num_vector_id < _impl->holding_regs.size()) {
      value = _impl->holding_regs[reg_num_vector_id];
      result = SUCCESS;
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper2::readHoldingRegisters(int reg_num,
                                                         int reg_count,
                                                         std::vector<uint16_t> &values,
                                                         int modbus_id,
                                                          Priority priority) {
//  std::cout << __func__ << " " << std::this_thread::get_id() <<  std::endl;
  if (!_impl->is_connected) return DEVICE_NOT_CONNECTED;
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;
  values.resize(reg_count);
  for (int current_reg_num = reg_num; current_reg_num < reg_num + reg_count; current_reg_num++) {
    readHoldingRegister(current_reg_num, values[current_reg_num - reg_num], modbus_id, priority);
    result = SUCCESS;
  }
//  if (_modbus_wrapper != nullptr) {
//
//    if (reg_num >= 0 && reg_num < _holding_regs.size()) {
//      values = std::vector<uint16_t>(_holding_regs.begin() + reg_num, _holding_regs.begin() + reg_num + reg_count);
//      result = SUCCESS;
//    }
//  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper2::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                          Priority priority) {
//  std::cout << __func__ << " " << std::this_thread::get_id() <<  std::endl;
//  std::unique_lock<std::recursive_mutex> lock(_mutex);
  if (!_impl->is_connected) return DEVICE_NOT_CONNECTED;
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->writeHoldingRegister(reg_num, value, modbus_id, priority);

    // Обновление

    if (result == SUCCESS) {
      uint16_t reg_value;
//      result = _modbus_wrapper->readHoldingRegister(reg_num, reg_value, modbus_id, priority);

      reg_value = value;
      if (result == SUCCESS) {
        std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
        auto reg_num_vector_id = reg_num - _impl->first_holding_reg_num;
        if (reg_num_vector_id >= 0 && reg_num_vector_id < _impl->holding_regs.size()) {
          _impl->holding_regs[reg_num_vector_id] = reg_value;
        }
      }
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper2::writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                                           Priority priority) {
//  std::cout << __func__ << std::endl;
//  std::unique_lock<std::recursive_mutex> lock(_mutex);
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->writeHoldingRegisters(reg_num, value, modbus_id, priority);

    // Обновление
    if (result == SUCCESS) {
      auto reg_count = (uint16_t)value.size();
      std::vector<uint16_t> reg_values;
//      result = _modbus_wrapper->readHoldingRegisters(reg_num, reg_count, reg_values, modbus_id, priority);
      reg_values = value;

      if (result == SUCCESS) {
        auto reg_num_vector_id = reg_num - _impl->first_holding_reg_num;
        if (reg_num_vector_id >= 0 && reg_num_vector_id < _impl->holding_regs.size() && reg_num_vector_id + reg_count <= _impl->holding_regs.size()) {
            std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
          for (int i = reg_num_vector_id; i < reg_num_vector_id + reg_count; i++) {
            _impl->holding_regs[i] = reg_values[i - reg_num];
          }
        }
      }
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper2::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                       Priority priority) {
//  std::cout << __func__ << std::endl;
  if (!_impl->is_connected) return DEVICE_NOT_CONNECTED;
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    auto reg_num_vector_id = reg_num - _impl->first_input_reg_num;
    if (reg_num_vector_id >= 0 && reg_num_vector_id < _impl->input_regs.size()) {
      value = _impl->input_regs[reg_num_vector_id];
      result = SUCCESS;
    }
  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper2::readInputRegisters(int reg_num,
                                                       int reg_count,
                                                       std::vector<uint16_t> &values,
                                                       int modbus_id,
                                                       Priority priority) {
//  std::cout << __func__ << std::endl;
  if (!_impl->is_connected) return DEVICE_NOT_CONNECTED;
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;
  values.resize(reg_count);
  for (int current_reg_num = reg_num; current_reg_num < reg_num + reg_count; current_reg_num++) {
    readInputRegister(current_reg_num, values[current_reg_num - reg_num], modbus_id, priority);
    result = SUCCESS;
  }

//  if (_modbus_wrapper != nullptr) {
//    if (reg_num >= 0 && reg_num < _input_regs.size()) {
//      values = std::vector<uint16_t>(_input_regs.begin() + reg_num, _input_regs.begin() + reg_num + reg_count);
//      result = SUCCESS;
//    }
//  }

  return result;
}

ErrorCode ModbusAsyncClientWrapper2::saveToEEPROM(int modbus_id,
                                                  Priority priority) {
//  std::cout << __func__ << std::endl;
  if (!_impl->is_connected) return DEVICE_NOT_CONNECTED;
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->saveToEEPROM(modbus_id, priority);
  }

  return result;
}


const std::vector<uint16_t> &ModbusAsyncClientWrapper2::getHoldingRegs() {
  return _impl->holding_regs;
}

const std::vector<uint16_t> &ModbusAsyncClientWrapper2::getInputRegs() {
  return _impl->input_regs;
}

void ModbusAsyncClientWrapper2::addConnectable(Connectable *connectable) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->addConnectable(connectable);
  }
}

void ModbusAsyncClientWrapper2::sendConnectionStatus(bool connection_status) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->sendConnectionStatus(connection_status);
  }
}

void ModbusAsyncClientWrapper2::setEnabled(bool enabled, int modbus_id) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->setEnabled(enabled, modbus_id);
  }

  ModbusWrapper::setEnabled(enabled, modbus_id);
}

bool ModbusAsyncClientWrapper2::isEnabled(int modbus_id) {
  bool result = ModbusWrapper::isEnabled(modbus_id);;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isEnabled(modbus_id);
  }
  return result;
}

  void ModbusAsyncClientWrapper2::setIsOpened(bool is_opened, int modbus_id) {
  _impl->modbus_wrapper->setIsOpened(is_opened, modbus_id);
  ModbusWrapper::setIsOpened(is_opened, modbus_id);
}

bool ModbusAsyncClientWrapper2::isOpened(int modbus_id) {
  return _impl->modbus_wrapper->isOpened(modbus_id);
}

int ModbusAsyncClientWrapper2::getHoldingRegsCount() {
  return (int)_impl->holding_regs.size();
}

int ModbusAsyncClientWrapper2::getInputRegsCount() {
  return (int)_impl->input_regs.size();
}

ErrorCode ModbusAsyncClientWrapper2::setHoldingRegsCount(int reg_count) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  ErrorCode error_code = OPERATION_INTERRUPTED;

  if (reg_count >= 0){
    _impl->holding_regs_count = reg_count;
    _impl->holding_regs.resize(_impl->holding_regs_count);
    error_code = SUCCESS;
  } else{
    error_code = INVALID_REQUEST;
  }

  return error_code;
}

ErrorCode ModbusAsyncClientWrapper2::setInputRegsCount(int reg_count) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  ErrorCode error_code = OPERATION_INTERRUPTED;

  if (reg_count >= 0){
    _impl->input_regs_count = reg_count;
    _impl->input_regs.resize(_impl->input_regs_count);
    error_code = SUCCESS;
  } else{
    error_code = INVALID_REQUEST;
  }

  return error_code;
}

int ModbusAsyncClientWrapper2::getStartHoldingRegsNum() {
  return _impl->first_holding_reg_num;
}

int ModbusAsyncClientWrapper2::getStartInputRegsNum() {
  return _impl->first_input_reg_num;
}

ErrorCode ModbusAsyncClientWrapper2::setStartHoldingRegsNum(int reg_num) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  _impl->first_holding_reg_num = reg_num;
  return SUCCESS;
}

ErrorCode ModbusAsyncClientWrapper2::setStartInputRegsNum(int reg_num) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  _impl->first_input_reg_num = reg_num;
  return SUCCESS;
}
