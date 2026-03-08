#include "modbuswrappers/logaddition/modbuslogwrapper.h"

#include <mutex>

#include <utils/enumconverter.h>

#include <map>

#include "modbuswrappers/modbuswrapperfactory.h"

struct ModbusLogWrapper::ModbusLogWrapperPrivate{
  std::recursive_mutex mutex;

  std::string ip;
  int port = 4001;
  int modbus_id = 2;
  ModbusLogger modbus_logger = ModbusLogger("base.log");

  std::map<DeviceState, std::shared_ptr<ModbusWrapper>> states;
  std::shared_ptr<ModbusWrapper> current_state = nullptr;
  DeviceState current_state_key = EMPTY;

  std::set<int> disable_modbus_ids;
  std::set<int> enabled_modbus_ids;
  std::shared_mutex disabled_modbus_ids_mutex;

  // slave_id to [enabled, opened]
  std::map<int, std::pair<bool, bool>> slave_enabled;

  ErrorCode connect();
  ErrorCode disconnect();
  void checkConnection();

  ModbusLogWrapperPrivate(const std::string& ip_, int port_): ip(ip_), port(port_){};

};

ModbusLogWrapper::ModbusLogWrapper(std::shared_ptr<ModbusWrapper> &baseWrapper ,std::string &ip, int port,std::string &logPath): ModbusWrapper(), _impl(new ModbusLogWrapperPrivate(ip, port)) {
  _impl->modbus_logger = ModbusLogger(logPath);
  _impl->current_state = baseWrapper;
}

ModbusLogWrapper::~ModbusLogWrapper() {
  delete _impl;
}

ErrorCode ModbusLogWrapper::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    disconnect();
    result = _impl->current_state->changeAddress(ip, port, modbus_id, reconnect);

    changeState(NOT_CONNECTED);
    if (reconnect) {
      connect();
    }
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::CHANGE_ADDRESS, result);
  return result;
}

std::string ModbusLogWrapper::getIP() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  std::string result;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->getIP();
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::GET_IP, ErrorCode::SUCCESS , "IP IS " + result);
  return result;
}

int ModbusLogWrapper::getPort() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  int result = 0;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->getPort();
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::GET_PORT, ErrorCode::SUCCESS ,"port is " + std::to_string(result));
  return result;
}

int ModbusLogWrapper::getModbusID() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  int result = 0;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->getModbusID();
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::GET_MODBUS_ID, ErrorCode::SUCCESS ,"ModbusID is " + std::to_string(result));

  return result;
}

ErrorCode ModbusLogWrapper::connect() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = _impl->connect();
  _impl->modbus_logger.log(_impl->current_state_key, Operation::CONNECT, result);
  return result;
}

ErrorCode ModbusLogWrapper::disconnect() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = _impl->disconnect();;
  _impl->modbus_logger.log(_impl->current_state_key, Operation::DISCONNECT, result);
  return result;
}

ErrorCode ModbusLogWrapper::isConnected(bool &is_connected, int modbus_id) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);

  // TODO: Потом необходимо сделать нормальную индикацию состояния коннекта: Modbus, TCP, Disabled
  is_connected = false;

  if (!isEnabled(modbus_id)) {
    return DEVICE_NOT_CONNECTED;
  }

  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->isConnected(is_connected, modbus_id);
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::IS_CONNECTED, result);

  return result;
}

ErrorCode ModbusLogWrapper::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                   Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;

  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->readHoldingRegister(reg_num, value, modbus_id, priority);
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::READ_HOLDING_REGISTERS, result , "ReadHoldingRegister " + std::to_string(reg_num) + " " + std::to_string(value) + " " + std::to_string(modbus_id));

  return result;
}

ErrorCode ModbusLogWrapper::readHoldingRegisters(int reg_num,
                                                    int reg_count,
                                                    std::vector<uint16_t> &values,
                                                    int modbus_id,
                                                    Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->readHoldingRegisters(reg_num, reg_count, values, modbus_id, priority);
  }
  for (auto value : values)
  {
    _impl->modbus_logger.log(_impl->current_state_key, Operation::READ_HOLDING_REGISTERS, result , "ReadHoldingRegisters " + std::to_string(reg_num) + " " + std::to_string(value) + " " + std::to_string(modbus_id));
  }
  return result;
}

ErrorCode ModbusLogWrapper::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                    Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->writeHoldingRegister(reg_num, value, modbus_id, priority);
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::READ_HOLDING_REGISTERS, result , "WriteHoldingRegister " + std::to_string(reg_num) + " " + std::to_string(value) + " " + std::to_string(modbus_id));

  return result;
}

ErrorCode ModbusLogWrapper::writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                                     Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->writeHoldingRegisters(reg_num, value, modbus_id, priority);
  }
  for (auto single_value : value)
  {
    _impl->modbus_logger.log(_impl->current_state_key, Operation::READ_HOLDING_REGISTERS, result , "ReadHoldingRegisters " + std::to_string(reg_num) + " " + std::to_string(single_value) + " " + std::to_string(modbus_id));
  }
  return result;
}

ErrorCode ModbusLogWrapper::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                 Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->readInputRegister(reg_num, value, modbus_id, priority);
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::READ_HOLDING_REGISTERS, result , "ReadInputRegister " + std::to_string(reg_num) + " " + std::to_string(value) + " " + std::to_string(modbus_id));

  return result;
}

ErrorCode ModbusLogWrapper::readInputRegisters(int reg_num,
                                                  int reg_count,
                                                  std::vector<uint16_t> &values,
                                                  int modbus_id,
                                                  Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  auto result = OPERATION_INTERRUPTED;

  std::cout << "READ HOLDING REGSITERS IN MODBUS WRAPPER STATE MACHINE" << std::endl;

  if (_impl->current_state != nullptr) {
    std::cout << "CURRENT STATE IS " << _impl->current_state_key << std::endl;
    result = _impl->current_state->readInputRegisters(reg_num, reg_count, values, modbus_id, priority);
  }
  for (auto value : values)
  {
    _impl->modbus_logger.log(_impl->current_state_key, Operation::READ_HOLDING_REGISTERS, result , "ReadHoldingRegisters " + std::to_string(reg_num) + " " + std::to_string(value) + " " + std::to_string(modbus_id));
  }
  return result;
}

void ModbusLogWrapper::checkConnectionStatusByResponse(modbus::ModbusResult response) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  std::cout << "RESPONSE MODBUS: " << response << " " << __func__ << std::endl;
  if (response == modbus::TCP_TIMEOUT_ERROR || response == modbus::NO_SOCKET_CONNECTION) {
    changeState(DISCONNECTED);
    _impl->modbus_logger.log(_impl->current_state_key, Operation::CHECK_CONNECTION_STATUS_BY_RESPONSE, SUCCESS);
  } else {
    changeState(CONNECTED);
    _impl->modbus_logger.log(_impl->current_state_key, Operation::CHECK_CONNECTION_STATUS_BY_RESPONSE, SUCCESS);
  }

}

void ModbusLogWrapper::changeState(DeviceState state) {
  if (_impl->states.count(state) != 0) {
    if (state != _impl->current_state_key) {
      std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
      _impl->current_state = _impl->states[state];
      _impl->current_state_key = state;
      switch (state) {
        case CONNECTED:sendConnectionStatus(true);
          _impl->modbus_logger.log(state, Operation::CHANGE_STATE, SUCCESS,"CONNECTION STATUS SENT CONNECTED ");
          break;
        case NOT_CONNECTED:sendConnectionStatus(false);
          _impl->modbus_logger.log(state, Operation::CHANGE_STATE, SUCCESS,"CONNECTION STATUS SENT NOT CONNECTED ");
          break;
        case DISCONNECTED:sendConnectionStatus(false);
          _impl->modbus_logger.log(state, Operation::CHANGE_STATE, SUCCESS,"CONNECTION STATUS SENT DISCONNECTED ");
          break;
        case READ_ONLY:break;
        case OPEN:break;
        case CLOSE:break;
        case INVALID:break;
        case EMPTY:break;
      }
    }
  }
}

ErrorCode ModbusLogWrapper::saveToEEPROM(int modbus_id,
                                            Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->saveToEEPROM(modbus_id, priority);
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::SAVE_TO_EEPROM, result);
  return result;
}

void ModbusLogWrapper::addState(const std::pair<DeviceState, std::shared_ptr<ModbusWrapper>> &state) {
  _impl->states.insert(state);
}

void ModbusLogWrapper::addConnectable(Connectable *connectable) {

}

void ModbusLogWrapper::sendConnectionStatus(bool connection_status) {

}

void ModbusLogWrapper::process() {
  ModbusWrapper::process();
}

void ModbusLogWrapper::addSlave(int modbus_id) {
  std::unique_lock<std::shared_mutex> lock(_impl->disabled_modbus_ids_mutex);
  if (!_impl->slave_enabled.count(modbus_id)) {
    _impl->slave_enabled.insert({modbus_id, {false, false}});
    _impl->modbus_logger.log(_impl->current_state_key, Operation::ADD_SLAVE, SUCCESS,"ADDED SLAVE, ID : " + std::to_string(modbus_id));
    return;
  }
  _impl->modbus_logger.log(_impl->current_state_key, Operation::ADD_SLAVE, SUCCESS,"SLAVE ALREADY EXISTS, ID : " + std::to_string(modbus_id));

}

void ModbusLogWrapper::removeSlave(int modbus_id) {
  std::unique_lock<std::shared_mutex> lock(_impl->disabled_modbus_ids_mutex);
  if (_impl->slave_enabled.count(modbus_id)) {
    _impl->slave_enabled.erase(modbus_id);
    _impl->modbus_logger.log(_impl->current_state_key, Operation::REMOVE_SLAVE, SUCCESS,"REMOVED SLAVE, ID : " + std::to_string(modbus_id));
  }
}

void ModbusLogWrapper::setEnabled(bool enabled, int modbus_id) {
  auto &disabled_modbus_ids_mutex = _impl->disabled_modbus_ids_mutex;
  std::unique_lock<std::shared_mutex> lock(disabled_modbus_ids_mutex);

  auto &slave_enabled = _impl->slave_enabled;
  if (slave_enabled.count(modbus_id)) {
    slave_enabled[modbus_id].first = enabled;
  }

  _impl->checkConnection();
}

bool ModbusLogWrapper::isEnabled(int modbus_id) {
  bool result = false;

  auto &disabled_modbus_ids_mutex =  _impl->disabled_modbus_ids_mutex;
  auto &slave_enabled = _impl->slave_enabled;
  std::shared_lock<std::shared_mutex> lock(disabled_modbus_ids_mutex);

  if (slave_enabled.count(modbus_id)) {
    result = slave_enabled[modbus_id].first;
  }
  if (result) {
    _impl->modbus_logger.log(_impl->current_state_key, Operation::IS_ENABLED, SUCCESS,"IS ENABLED : " + std::to_string(modbus_id));
  }

  return result;
}

void ModbusLogWrapper::setIsOpened(bool is_opened, int modbus_id) {
  std::unique_lock<std::shared_mutex> lock(_impl->disabled_modbus_ids_mutex);
  auto &slave_opened = _impl->slave_enabled;
  if (slave_opened.count(modbus_id)) {
    slave_opened[modbus_id].second = is_opened;
    _impl->modbus_logger.log(_impl->current_state_key, Operation::SET_IS_OPENED, SUCCESS);
  }


  ModbusWrapper::setIsOpened(is_opened, modbus_id);
}

bool ModbusLogWrapper::isOpened(int modbus_id) {
  return ModbusWrapper::isOpened(modbus_id);
}

void ModbusLogWrapper::ModbusLogWrapperPrivate::checkConnection() {
  auto it = std::find_if(std::begin(slave_enabled), std::end(slave_enabled), [&](const auto &slave) {
    return slave.second.first == true && slave.second.second == true;
  });

  if (it == std::end(slave_enabled)) {
    disconnect();
  }
}

ErrorCode ModbusLogWrapper::ModbusLogWrapperPrivate::connect() {
  auto result = OPERATION_INTERRUPTED;

  if (current_state != nullptr) {
    result = current_state->connect();
  }

  return result;
}

ErrorCode ModbusLogWrapper::ModbusLogWrapperPrivate::disconnect() {
  auto result = OPERATION_INTERRUPTED;

  if (current_state != nullptr) {
    result = current_state->disconnect();
  }

  modbus_logger.log(current_state_key, DISCONNECT, result);

  return result;

}


