#include "modbuswrappers/mocks/modbuswrappermock2.h"

#include <atomic>
#include <iostream>
#include <thread>
#include <mutex>

static const int MAX_REG_NUM = 10000;
static const int MAX_HOLDING_REG_NUM = 10000;
static const int MAX_INPUT_REG_NUM = 10000;

struct ModbusWrapperMock2::ModbusWrapperMock2Private{
  std::recursive_mutex mutex;

  std::string ip;
  int port = 4001;
  int modbus_id = 1;

  std::atomic<bool> is_connected = false;

  RegCardType holding_reg_values;
  RegCardType input_reg_values;
};

void ModbusWrapperMock2::delaySim() {
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

ModbusWrapperMock2::ModbusWrapperMock2(const std::string &ip, int port, int modbus_id):
                                       _impl(new ModbusWrapperMock2Private()) {
  _impl->ip = ip;
  _impl->port = port;
  _impl->modbus_id = modbus_id;
}

ModbusWrapperMock2::~ModbusWrapperMock2() {
  delete _impl;
}

std::string ModbusWrapperMock2::getIP() {
  return _impl->ip;
}

int ModbusWrapperMock2::getPort() {
  return _impl->port;
}

int ModbusWrapperMock2::getModbusID() {
  return _impl->modbus_id;
}

ErrorCode ModbusWrapperMock2::connect() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (!_impl->is_connected) {
    _impl->is_connected = true;
    result = SUCCESS;

    delaySim();

    sendConnectionStatus(_impl->is_connected);
    std::cout << "CONNECTED" << std::endl;
  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::disconnect() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_connected) {
    _impl->is_connected = false;
    result = SUCCESS;

    sendConnectionStatus(_impl->is_connected);
    std::cout << "DISCONNECTED" << std::endl;
  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::isConnected(bool &is_connected, int modbus_id) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  delaySim();
  is_connected = _impl->is_connected;
  if (_impl->is_connected) {
    result = SUCCESS;
  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                  Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_connected) {
    result = readRegCard(reg_num, value, modbus_id, _impl->holding_reg_values);

//    std::cout << "Read from Holding Register: " << reg_num << " " << value << " " << modbus_id << result << " "  <<  " " << __func__ << std::endl;
  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::readHoldingRegisters(int reg_num,
                                                   int reg_count,
                                                   std::vector<uint16_t> &values,
                                                   int modbus_id,
                                                   Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_connected) {
    delaySim();
    values.resize(reg_count);
    for (int current_reg_num = reg_num; current_reg_num < reg_num + reg_count; current_reg_num++) {
      uint16_t current_reg_value;
      result = readHoldingRegister(current_reg_num, current_reg_value, modbus_id);

      values[current_reg_num - reg_num] = current_reg_value;

      if (result != SUCCESS) {
        break;
      }
    }
  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                   Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_connected) {
//    delaySim();
    result = writeRegCard(reg_num, value, modbus_id, _impl->holding_reg_values);
//    std::cout << "Write to Holding Register: " << reg_num << " " << value << " " << modbus_id << " " << result << " "  << __func__ << std::endl;
  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                                    Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_connected) {
    delaySim();
    auto reg_count = value.size();
    for (int current_reg_num = reg_num; current_reg_num < reg_num + reg_count; current_reg_num++) {
      auto current_reg_value = value[current_reg_num - reg_num];
      result = writeHoldingRegister(current_reg_num, current_reg_value, modbus_id);
    }
  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_connected) {
//    delaySim();
    result = readRegCard(reg_num, value, modbus_id, _impl->input_reg_values);
//    std::cout << "Read from Input Register: " << reg_num << " " << value << " " << modbus_id << " " << result << " " << __func__ << std::endl;

  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::readInputRegisters(int reg_num,
                                                 int reg_count,
                                                 std::vector<uint16_t> &values,
                                                 int modbus_id,
                                                 Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_connected) {
    delaySim();

    values.resize(reg_count);
    for (int current_reg_num = reg_num; current_reg_num < reg_num + reg_count; current_reg_num++) {
      uint16_t current_reg_value;
      result = readInputRegister(current_reg_num, current_reg_value, modbus_id);

      values[current_reg_num - reg_num] = current_reg_value;

      if (result != SUCCESS) {
        break;
      }
    }
  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

void ModbusWrapperMock2::addConnectable(Connectable *connectable) {

}

void ModbusWrapperMock2::sendConnectionStatus(bool connection_status) {

}

void ModbusWrapperMock2::process() {
  ModbusWrapper::process();
}

ErrorCode ModbusWrapperMock2::readRegCard(int reg_num, uint16_t &value, int modbus_id, RegCardType &reg_card) {
  auto result = OPERATION_INTERRUPTED;

//  delaySim();
  if (reg_card.count(modbus_id) == 0) {
    reg_card.insert({modbus_id, {}});
  }

  auto &reg_vector = reg_card.at(modbus_id);

  if (reg_num >= 0 && reg_num < MAX_REG_NUM) {
    if (reg_num >= reg_vector.size()) {
      reg_vector.resize(reg_num + 1);
    }

    value = reg_vector[reg_num];
    result = SUCCESS;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::writeRegCard(int reg_num, uint16_t value, int modbus_id, RegCardType &reg_card) {
  auto result = OPERATION_INTERRUPTED;

  if (reg_card.count(modbus_id) == 0) {
    reg_card.insert({modbus_id, {}});
  }

  auto &reg_vector = reg_card.at(modbus_id);

  if (reg_num >= 0 && reg_num < MAX_REG_NUM) {
    if (reg_num >= reg_vector.size()) {
      reg_vector.resize(reg_num + 1);
    }

    reg_vector[reg_num] = value;
    result = SUCCESS;
  }

  return result;
}

void ModbusWrapperMock2::checkConnectionStatusByResponse(modbus::ModbusResult response) {
  if (response == modbus::TCP_TIMEOUT_ERROR || response == modbus::NO_SOCKET_CONNECTION) {
    sendConnectionStatus(false);
  } else {
    sendConnectionStatus(true);
  }
}

ErrorCode ModbusWrapperMock2::saveToEEPROM(int modbus_id, Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  delaySim();
  auto result = OPERATION_INTERRUPTED;
  if (_impl->is_connected) {
    result = SUCCESS;
  } else {
    result = DEVICE_NOT_RESPONDING_LAN;
  }

  return result;
}

ErrorCode ModbusWrapperMock2::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  _impl->ip = ip;
  _impl->port = port;
  _impl->modbus_id = modbus_id;
  result = SUCCESS;

  return result;
}


