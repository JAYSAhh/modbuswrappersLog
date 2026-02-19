#include "modbuswrappers/modbusclientwrapper/modbusclientwrapper.h"
#include <mutex>
#include <utils/enumconverter.h>
#include <map>

struct ModbusClientWrapper::ModbusClientWrapperPrivate{
  std::recursive_mutex mutex;

  std::string ip;
  int port = 4001;
  int modbus_id = 2;

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
};

ModbusClientWrapper::ModbusClientWrapper(const std::string &ip, int port, int modbus_id): ModbusWrapper(), _impl(new ModbusClientWrapperPrivate()) {
  _impl->ip = ip;
  _impl->port = port;
  _impl->modbus_id = modbus_id;
}

ModbusClientWrapper::~ModbusClientWrapper() {
  delete _impl;
}

ErrorCode ModbusClientWrapper::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
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

  return result;
}

std::string ModbusClientWrapper::getIP() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  std::string result;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->getIP();
  }

  return result;
}

int ModbusClientWrapper::getPort() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  int result = 0;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->getPort();
  }

  return result;
}

int ModbusClientWrapper::getModbusID() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  int result = 0;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->getModbusID();
  }

  return result;
}

ErrorCode ModbusClientWrapper::connect() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = _impl->connect();
  return result;
}

ErrorCode ModbusClientWrapper::disconnect() {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  auto result = _impl->disconnect();;
  return result;
}

ErrorCode ModbusClientWrapper::isConnected(bool &is_connected, int modbus_id) {
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

  return result;
}

ErrorCode ModbusClientWrapper::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                   Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;

  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->readHoldingRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusClientWrapper::readHoldingRegisters(int reg_num,
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

  return result;
}

ErrorCode ModbusClientWrapper::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                    Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->writeHoldingRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusClientWrapper::writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                                     Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->writeHoldingRegisters(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusClientWrapper::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                 Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  auto result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->readInputRegister(reg_num, value, modbus_id, priority);
  }

  return result;
}

ErrorCode ModbusClientWrapper::readInputRegisters(int reg_num,
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

  return result;
}

void ModbusClientWrapper::checkConnectionStatusByResponse(modbus::ModbusResult response) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  std::cout << "RESPONSE MODBUS: " << response << " " << __func__ << std::endl;
  if (response == modbus::TCP_TIMEOUT_ERROR || response == modbus::NO_SOCKET_CONNECTION) {
    changeState(DISCONNECTED);
  } else {
    changeState(CONNECTED);
  }
}

void ModbusClientWrapper::changeState(DeviceState state) {
  if (_impl->states.count(state) != 0) {
    if (state != _impl->current_state_key) {
      std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
      _impl->current_state = _impl->states[state];
      _impl->current_state_key = state;
      switch (state) {
        case CONNECTED:sendConnectionStatus(true);
          std::cout << "CONNECTION STATUS SENT CONNECTED " << __func__ << std::endl;
          break;
        case NOT_CONNECTED:sendConnectionStatus(false);
          std::cout << "CONNECTION STATUS SENT NOT_CONNECTED " << __func__ << std::endl;
          break;
        case DISCONNECTED:sendConnectionStatus(false);
          std::cout << "CONNECTION STATUS SENT DISCONNECTED " << __func__ << std::endl;
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

ErrorCode ModbusClientWrapper::saveToEEPROM(int modbus_id,
                                            Priority priority) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);
  if (!isEnabled(modbus_id)) return DEVICE_NOT_CONNECTED;
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->current_state != nullptr) {
    result = _impl->current_state->saveToEEPROM(modbus_id, priority);
  }

  return result;
}

void ModbusClientWrapper::addState(const std::pair<DeviceState, std::shared_ptr<ModbusWrapper>> &state) {
  _impl->states.insert(state);
}

void ModbusClientWrapper::addConnectable(Connectable *connectable) {

}

void ModbusClientWrapper::sendConnectionStatus(bool connection_status) {

}

void ModbusClientWrapper::process() {
  ModbusWrapper::process();
}

void ModbusClientWrapper::addSlave(int modbus_id) {
  std::unique_lock<std::shared_mutex> lock(_impl->disabled_modbus_ids_mutex);
  if (!_impl->slave_enabled.count(modbus_id)) {
    _impl->slave_enabled.insert({modbus_id, {false, false}});
  }
}

void ModbusClientWrapper::removeSlave(int modbus_id) {
  std::unique_lock<std::shared_mutex> lock(_impl->disabled_modbus_ids_mutex);
  if (_impl->slave_enabled.count(modbus_id)) {
    _impl->slave_enabled.erase(modbus_id);
  }
}

void ModbusClientWrapper::setEnabled(bool enabled, int modbus_id) {
  auto &disabled_modbus_ids_mutex = _impl->disabled_modbus_ids_mutex;
  std::unique_lock<std::shared_mutex> lock(disabled_modbus_ids_mutex);

  auto &slave_enabled = _impl->slave_enabled;
  if (slave_enabled.count(modbus_id)) {
    slave_enabled[modbus_id].first = enabled;
  }

  _impl->checkConnection();
}

bool ModbusClientWrapper::isEnabled(int modbus_id) {
  bool result = false;

  auto &disabled_modbus_ids_mutex =  _impl->disabled_modbus_ids_mutex;
  auto &slave_enabled = _impl->slave_enabled;
  std::shared_lock<std::shared_mutex> lock(disabled_modbus_ids_mutex);

  if (slave_enabled.count(modbus_id)) {
    result = slave_enabled[modbus_id].first;
  }

  return result;
}

void ModbusClientWrapper::setIsOpened(bool is_opened, int modbus_id) {
  std::unique_lock<std::shared_mutex> lock(_impl->disabled_modbus_ids_mutex);
  auto &slave_opened = _impl->slave_enabled;
  if (slave_opened.count(modbus_id)) {
    slave_opened[modbus_id].second = is_opened;
  }

  ModbusWrapper::setIsOpened(is_opened, modbus_id);
}

bool ModbusClientWrapper::isOpened(int modbus_id) {
  return ModbusWrapper::isOpened(modbus_id);
}

void ModbusClientWrapper::ModbusClientWrapperPrivate::checkConnection() {
  auto it = std::find_if(std::begin(slave_enabled), std::end(slave_enabled), [&](const auto &slave) {
    return slave.second.first == true && slave.second.second == true;
  });

  if (it == std::end(slave_enabled)) {
    disconnect();
  }
}

ErrorCode ModbusClientWrapper::ModbusClientWrapperPrivate::connect() {
  auto result = OPERATION_INTERRUPTED;

  if (current_state != nullptr) {
    result = current_state->connect();
  }

  return result;
}

ErrorCode ModbusClientWrapper::ModbusClientWrapperPrivate::disconnect() {
  auto result = OPERATION_INTERRUPTED;

  if (current_state != nullptr) {
    result = current_state->disconnect();
  }

  return result;
}
