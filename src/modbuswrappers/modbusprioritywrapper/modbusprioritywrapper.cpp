#include "modbuswrappers/modbusprioritywrapper/modbusprioritywrapper.h"

#include <queue>
#include <map>
#include <mutex>
#include <thread>

#include <utils/services/uidgeneratorimpls/boost/uidgeneratorboostimpl.h>

static const int SAVE_TO_EEPROM_FUNCTION_PRIORITY = 4;
static const int WRITE_HOLDING_REGISTER_PRIORITY = 3;
static const int WRITE_HOLDING_REGISTERS_PRIORITY = 3;
static const int READ_HOLDING_REGISTER_PRIORITY = 2;
static const int READ_HOLDING_REGISTERS_PRIORITY = 2;
static const int READ_INPUT_REGISTER_PRIORITY = 1;
static const int READ_INPUT_REGISTERS_PRIORITY = 1;

struct ModbusPriorityWrapper::ModbusPriorityWrapperPrivate{
  std::shared_ptr<UIDGenerator> uid_generator = nullptr;

  std::shared_ptr<ModbusWrapper> modbus_wrapper = nullptr;

  std::mutex request_queue_mutex;
  std::priority_queue<ModbusPriorityWrapperRequest> modbus_request_queue;

  std::mutex response_map_mutex;
  std::map<std::string, ModbusPriorityWrapperResponse> modbus_response_map;

  int sleep_in_ms = 300;
  int timeout_in_ms = 5000;
};

ModbusPriorityWrapper::ModbusPriorityWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper):
                                             _impl(new ModbusPriorityWrapperPrivate()){
  _impl->modbus_wrapper = modbus_wrapper;
  _impl->uid_generator = std::make_shared<UIDGeneratorBoostImpl>();
}

ModbusPriorityWrapper::~ModbusPriorityWrapper() {
  delete _impl;
}

std::string ModbusPriorityWrapper::getIP() {
  std::string result;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getIP();
  }

  return result;
}

int ModbusPriorityWrapper::getPort() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getPort();
  }

  return result;
}

int ModbusPriorityWrapper::getModbusID() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getModbusID();
  }

  return result;
}

ErrorCode ModbusPriorityWrapper::connect() {
  auto result = DEVICE_NOT_RESPONDING_LAN;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->connect();
  }

  return result;
}

ErrorCode ModbusPriorityWrapper::disconnect() {
  auto result = DEVICE_NOT_RESPONDING_LAN;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->disconnect();
  }

  return result;
}

ErrorCode ModbusPriorityWrapper::isConnected(bool &is_connected, int modbus_id) {
  auto result = DEVICE_NOT_RESPONDING_LAN;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isConnected(is_connected, modbus_id);
  }

  return result;
}

ModbusPriorityWrapperResponse ModbusPriorityWrapper::getResponse(const std::string &uid, bool &is_ok) {
  ModbusPriorityWrapperResponse response{};
  is_ok = false;

  bool found = false;
  int time_count = 0;
  while (!found && time_count < _impl->timeout_in_ms) {
    std::unique_lock<std::mutex> lock(_impl->response_map_mutex);
    found = _impl->modbus_response_map.count(uid);
    if (found) {
      break;
    }

    time_count += _impl->sleep_in_ms;
    std::this_thread::sleep_for(std::chrono::milliseconds(_impl->sleep_in_ms));
  }

  if (!found) {
    is_ok = false;
    return response;
  }

  is_ok = true;
  std::unique_lock<std::mutex> lock(_impl->response_map_mutex);
  response = _impl->modbus_response_map[uid];
  _impl->modbus_response_map.erase(uid);

  return response;
}

ErrorCode ModbusPriorityWrapper::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                     Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->uid_generator == nullptr) {
    return result;
  }

  auto uid = _impl->uid_generator->generateUIDString();
  std::vector<uint16_t> values;
  int reg_count = 1;

  int priority_level = READ_HOLDING_REGISTER_PRIORITY;
  if (priority.is_custom_priority) {
    priority_level = priority.custom_priority;
  }

  std::unique_lock<std::mutex> lock(_impl->request_queue_mutex);
  _impl->modbus_request_queue.push({ModbusPriorityWrapper_READ_HOLDING,
                              priority_level,
                              uid,
                              reg_num,
                              reg_count,
                              values,
                              modbus_id});
  lock.unlock();
  processRequest();


  bool is_ok = false;
  auto response = getResponse(uid, is_ok);

  if (is_ok) {
    result = response.error_code;
    value = response.reg_values[0];
  }

  return result;
}

ErrorCode ModbusPriorityWrapper::readHoldingRegisters(int reg_num,
                                                      int reg_count,
                                                      std::vector<uint16_t> &values,
                                                      int modbus_id,
                                                      Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->uid_generator == nullptr) {
    return result;
  }

  auto uid = _impl->uid_generator->generateUIDString();
//  std::vector<uint16_t> values;
//  int reg_count = 1;
  int priority_level = READ_HOLDING_REGISTERS_PRIORITY;
  if (priority.is_custom_priority) {
    priority_level = priority.custom_priority;
  }

  std::unique_lock<std::mutex> lock(_impl->request_queue_mutex);

  _impl->modbus_request_queue.push({ModbusPriorityWrapper_READ_HOLDING_MULTIPLE,
                              priority_level,
                              uid,
                              reg_num,
                              reg_count,
                              values,
                              modbus_id});

  lock.unlock();
  processRequest();
  bool is_ok = false;
  auto response = getResponse(uid, is_ok);

  if (is_ok) {
    result = response.error_code;
    values = response.reg_values;
  }

  return result;

}
ErrorCode ModbusPriorityWrapper::writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                                      Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->uid_generator == nullptr) {
    return result;
  }

  auto uid = _impl->uid_generator->generateUIDString();
  std::vector<uint16_t> values = {value};
  int reg_count = 1;

  int priority_level = WRITE_HOLDING_REGISTER_PRIORITY;
  if (priority.is_custom_priority) {
    priority_level = priority.custom_priority;
  }

  std::unique_lock<std::mutex> lock(_impl->request_queue_mutex);
  _impl->modbus_request_queue.push({ModbusPriorityWrapper_WRITE_HOLDING,
                              priority_level,
                              uid,
                              reg_num,
                              reg_count,
                              values,
                              modbus_id});

  lock.unlock();
  processRequest();
  bool is_ok = false;
  auto response = getResponse(uid, is_ok);

  if (is_ok) {
    result = response.error_code;
    value = response.reg_values[0];
  }

  return result;
}

ErrorCode ModbusPriorityWrapper::writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                                       Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->uid_generator == nullptr) {
    return result;
  }

  auto uid = _impl->uid_generator->generateUIDString();
  int reg_count = (int)value.size();

  int priority_level = WRITE_HOLDING_REGISTERS_PRIORITY;
  if (priority.is_custom_priority) {
    priority_level = priority.custom_priority;
  }
  std::unique_lock<std::mutex> lock(_impl->request_queue_mutex);
  _impl->modbus_request_queue.push({ModbusPriorityWrapper_WRITE_HOLDING_MULTIPLE,
                              priority_level,
                              uid,
                              reg_num,
                              reg_count,
                              value,
                              modbus_id});

  lock.unlock();
  processRequest();
  bool is_ok = false;
  auto response = getResponse(uid, is_ok);

  if (is_ok) {
    result = response.error_code;
    value = response.reg_values;
  }

  return result;
}

ErrorCode ModbusPriorityWrapper::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                   Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->uid_generator == nullptr) {
    return result;
  }

  auto uid = _impl->uid_generator->generateUIDString();
  std::vector<uint16_t> values;
  int reg_count = 1;

  int priority_level = READ_INPUT_REGISTER_PRIORITY;
  if (priority.is_custom_priority) {
    priority_level = priority.custom_priority;
  }
  std::unique_lock<std::mutex> lock(_impl->request_queue_mutex);
  _impl->modbus_request_queue.push({ModbusPriorityWrapper_READ_INPUT,
                              priority_level,
                              uid,
                              reg_num,
                              reg_count,
                              values,
                              modbus_id});

  lock.unlock();
  processRequest();
  bool is_ok = false;
  auto response = getResponse(uid, is_ok);

  if (is_ok) {
    result = response.error_code;
    value = response.reg_values[0];
  }

  return result;
}

ErrorCode ModbusPriorityWrapper::readInputRegisters(int reg_num,
                                                    int reg_count,
                                                    std::vector<uint16_t> &values,
                                                    int modbus_id,
                                                    Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->uid_generator == nullptr) {
    return result;
  }

  auto uid = _impl->uid_generator->generateUIDString();

  int priority_level = READ_INPUT_REGISTERS_PRIORITY;
  if (priority.is_custom_priority) {
    priority_level = priority.custom_priority;
  }
  std::unique_lock<std::mutex> lock(_impl->request_queue_mutex);
  _impl->modbus_request_queue.push({ModbusPriorityWrapper_READ_INPUT_MULTIPLE,
                              priority_level,
                              uid,
                              reg_num,
                              reg_count,
                              values,
                              modbus_id});

  lock.unlock();
  processRequest();
  bool is_ok = false;
  auto response = getResponse(uid, is_ok);

  if (is_ok) {
    result = response.error_code;
    values = response.reg_values;
  }

  return result;
}

ErrorCode ModbusPriorityWrapper::saveToEEPROM(int modbus_id, Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->uid_generator == nullptr) {
    return result;
  }

  auto uid = _impl->uid_generator->generateUIDString();

  int priority_level = SAVE_TO_EEPROM_FUNCTION_PRIORITY;
  if (priority.is_custom_priority) {
    priority_level = priority.custom_priority;
  }
  std::unique_lock<std::mutex> lock(_impl->request_queue_mutex);
  _impl->modbus_request_queue.push({ModbusPriorityWrapper_SAVE_TO_EEPROM_FUNCTION,
                              priority_level,
                              uid,
                              0,
                              0,
                              {},
                              modbus_id});

  lock.unlock();
  processRequest();
  bool is_ok = false;
  auto response = getResponse(uid, is_ok);

  if (is_ok) {
    result = response.error_code;
  }

  return result;
}

void ModbusPriorityWrapper::addConnectable(Connectable *connectable) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->addConnectable(connectable);
  }
}

void ModbusPriorityWrapper::sendConnectionStatus(bool connection_status) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->sendConnectionStatus(connection_status);
  }
}

void ModbusPriorityWrapper::process() {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->process();
  }
}

void ModbusPriorityWrapper::processRequest() {
  std::lock_guard<std::mutex> lock(_impl->request_queue_mutex);

  if (!_impl->modbus_request_queue.empty()) {
    ModbusPriorityWrapperRequest request;
    {
//      std::lock_guard<std::mutex> lock(request_queue_mutex);
      request = _impl->modbus_request_queue.top();
      _impl->modbus_request_queue.pop();
    }

    if (_impl->modbus_wrapper != nullptr) {
      std::string uid = request.uid;

      ErrorCode result = OPERATION_INTERRUPTED;

      int reg_num = request.reg_num;
      int reg_count = request.reg_count;
      std::vector<uint16_t> values = request.reg_values;
      int modbus_id = request.modbus_id;

      switch (request.request_type) {
        case ModbusPriorityWrapper_READ_HOLDING:
          values.resize(1);
          result = _impl->modbus_wrapper->readHoldingRegister(reg_num, values[0], modbus_id);
          break;
        case ModbusPriorityWrapper_READ_HOLDING_MULTIPLE:
          values.resize(reg_count);
          result = _impl->modbus_wrapper->readHoldingRegisters(reg_num, reg_count, values, modbus_id);
          break;
        case ModbusPriorityWrapper_READ_INPUT:
          values.resize(1);
          result = _impl->modbus_wrapper->readInputRegister(reg_num, values[0], modbus_id);
          break;
        case ModbusPriorityWrapper_READ_INPUT_MULTIPLE:
          values.resize(reg_count);
          result = _impl->modbus_wrapper->readInputRegisters(reg_num, reg_count, values, modbus_id);
          break;
        case ModbusPriorityWrapper_WRITE_HOLDING:
          values.resize(1);
          result = _impl->modbus_wrapper->writeHoldingRegister(reg_num, values[0], modbus_id);
          break;
        case ModbusPriorityWrapper_WRITE_HOLDING_MULTIPLE:
          values.resize(reg_count);
          result = _impl->modbus_wrapper->writeHoldingRegisters(reg_num, values, modbus_id);
          break;
        case ModbusPriorityWrapper_SAVE_TO_EEPROM_FUNCTION:
          values.resize(1);
          result = _impl->modbus_wrapper->saveToEEPROM(modbus_id);
          break;
      }


      std::unique_lock<std::mutex> response_lock(_impl->response_map_mutex);
      _impl->modbus_response_map.insert({uid,
                                   {
                                       uid, result, reg_num, reg_count, values, modbus_id
                                   }});
    }
  }
}

void ModbusPriorityWrapper::setEnabled(bool enabled, int modbus_id) {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->setEnabled(enabled, modbus_id);
  }
  ModbusWrapper::setEnabled(enabled, modbus_id);
}

bool ModbusPriorityWrapper::isEnabled(int modbus_id) {
  bool result = ModbusWrapper::isEnabled(modbus_id);;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isEnabled(modbus_id);
  }
  return result;
}

ErrorCode ModbusPriorityWrapper::changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->changeAddress(ip, port, modbus_id, reconnect);
  }

  return result;
}
