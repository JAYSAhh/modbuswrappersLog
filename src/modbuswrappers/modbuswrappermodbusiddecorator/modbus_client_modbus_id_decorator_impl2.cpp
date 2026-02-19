#include "modbuswrappers/modbuswrappermodbusiddecorator/modbus_client_modbus_id_decorator_impl2.h"

#include <iostream>
#include <atomic>
#include <algorithm>
#include <mutex>

#include "modbuswrappers/connectable.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapper.h"
#include <utils/services/retryer.h>

struct ModbusClientModbusIDDecoratorImpl2::ModbusClientModbusIDDecoratorImpl2Private{
  int modbus_id = 1;
  std::shared_ptr<ModbusClientWrapper> modbus_wrapper = nullptr;

  /**
   * Указывает на то, открыт ли данный Modbus слейв к общению. Если false, то даже в случае успешного Modbus подключения
   * по TCP, по текущему ID не будет осуществляться взаимодействие
   */
  std::atomic<bool> is_opened = false;
  std::atomic<bool> is_connected = false;

  std::atomic<bool> is_connected_sent = false;

  std::recursive_mutex mutex;

  std::vector<Connectable *> connectables;

  std::shared_ptr<Retryer> retryer = nullptr;
};

// TODO: Необходимо добавить retryer на уровне MODBUS_ID, чтобы восстановление подключение MODBUS не тормозило шину

ModbusClientModbusIDDecoratorImpl2::ModbusClientModbusIDDecoratorImpl2(const std::shared_ptr<ModbusClientWrapper> &modbus_wrapper,
                                                                        int modbus_id,
                                                                        const std::shared_ptr<Retryer> &retryer):
                                                                        _impl(new ModbusClientModbusIDDecoratorImpl2Private()){
  _impl->modbus_wrapper = modbus_wrapper;
  _impl->modbus_id = modbus_id;
  _impl->retryer = retryer;
  _impl->is_connected_sent = false;

  _impl->modbus_wrapper->addSlave(_impl->modbus_id);
}

ModbusClientModbusIDDecoratorImpl2::~ModbusClientModbusIDDecoratorImpl2() {
  _impl->modbus_wrapper->removeSlave(_impl->modbus_id);
  delete _impl;
}

std::string ModbusClientModbusIDDecoratorImpl2::getIP() {
  std::string result;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getIP();
  }

  return result;
}

int ModbusClientModbusIDDecoratorImpl2::getPort() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->getPort();
  }

  return result;
}

int ModbusClientModbusIDDecoratorImpl2::getModbusID() {
  int result = 0;

  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_id;
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::connect() {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->connect();
    setIsOpened(true, _impl->modbus_id);
    _impl->modbus_wrapper->setEnabled(true, _impl->modbus_id);
    bool is_connected = false;
    _impl->is_connected = true;
    result = isConnected(is_connected, _impl->modbus_id);
    _impl->is_connected = is_connected;
    sendConnectionStatus(is_connected);
    _impl->is_connected_sent = is_connected;
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::disconnect() {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr) {
//    result = _impl->modbus_wrapper->disconnect();
    setIsOpened(false, _impl->modbus_id);
    _impl->modbus_wrapper->setEnabled(false, _impl->modbus_id);
    _impl->is_connected = false;
    result = SUCCESS;
    sendConnectionStatus(false);
    _impl->is_connected_sent = false;
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::isConnected(bool &is_connected, int modbus_id) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->modbus_wrapper != nullptr && _impl->retryer != nullptr) {
//    if (!_is_connected) return DEVICE_NOT_CONNECTED;

    is_connected = _impl->is_connected;

    if (_impl->is_opened) {
      bool to_retry = _impl->retryer->isToRetry();
      if (to_retry) {
        result = _impl->modbus_wrapper->isConnected(is_connected, _impl->modbus_id);
        is_connected &= _impl->is_connected;
        _impl->is_connected = is_connected;
        if (is_connected) _impl->retryer->reset();
      } else {
        is_connected = _impl->is_connected;
      }
    }

//    checkConnectionStatusByResponse(result);
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                                             Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_opened) {
    if (_impl->modbus_wrapper != nullptr) {
      result = _impl->modbus_wrapper->readHoldingRegister(reg_num, value, _impl->modbus_id, priority);
      checkConnectionStatusByResponse(result);
    }
  } else {
    result = DEVICE_NOT_CONNECTED;
  }

  std::cout << "READ HOLDING REGISTERS MODBUS ID IMPL2 " << result << std::endl;
  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::readHoldingRegisters(int reg_num,
                                                              int reg_count,
                                                              std::vector<uint16_t> &values,
                                                              int modbus_id,
                                                              Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->is_opened) {
    if (_impl->modbus_wrapper != nullptr) {
      result = _impl->modbus_wrapper->readHoldingRegisters(reg_num, reg_count, values, _impl->modbus_id, priority);
      checkConnectionStatusByResponse(result);
    }
  } else {
    result = DEVICE_NOT_CONNECTED;
  }

  std::cout << "READ HOLDING REGISTERS SSS MODBUS ID IMPL2 " << result << std::endl;

  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::writeHoldingRegister(int reg_num,
                                                              uint16_t value,
                                                              int modbus_id,
                                                              Priority priority) {
  auto result = OPERATION_INTERRUPTED;
  if (_impl->is_opened) {
    if (_impl->modbus_wrapper != nullptr) {
      result = _impl->modbus_wrapper->writeHoldingRegister(reg_num, value, _impl->modbus_id, priority);
      checkConnectionStatusByResponse(result);
    }
  } else {
    result = DEVICE_NOT_CONNECTED;
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::writeHoldingRegisters(int reg_num,
                                                               std::vector<uint16_t> value,
                                                               int modbus_id,
                                                               Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_opened) {
    if (_impl->modbus_wrapper != nullptr) {
      result = _impl->modbus_wrapper->writeHoldingRegisters(reg_num, value, _impl->modbus_id, priority);
      checkConnectionStatusByResponse(result);
    }
  } else {
    result = DEVICE_NOT_CONNECTED;
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                                                           Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_opened) {
    if (_impl->modbus_wrapper != nullptr) {
      result = _impl->modbus_wrapper->readInputRegister(reg_num, value, _impl->modbus_id, priority);
      checkConnectionStatusByResponse(result);
    }
  } else {
    result = DEVICE_NOT_CONNECTED;
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::readInputRegisters(int reg_num,
                                                            int reg_count,
                                                            std::vector<uint16_t> &values,
                                                            int modbus_id,
                                                            Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->is_opened) {
    if (_impl->modbus_wrapper != nullptr) {
      result = _impl->modbus_wrapper->readInputRegisters(reg_num, reg_count, values, _impl->modbus_id, priority);
      checkConnectionStatusByResponse(result);
    }
  } else {
    result = DEVICE_NOT_CONNECTED;
  }

  return result;
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::saveToEEPROM(int modbus_id, Priority priority) {
  ErrorCode result = OPERATION_INTERRUPTED;

  if (_impl->is_opened) {
    if (_impl->modbus_wrapper != nullptr) {
      result = _impl->modbus_wrapper->saveToEEPROM(_impl->modbus_id, priority);
      checkConnectionStatusByResponse(result);
    }
  } else {
    result = DEVICE_NOT_CONNECTED;
  }

  return result;
}

void ModbusClientModbusIDDecoratorImpl2::addConnectable(Connectable *connectable) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);

  std::cout << "Connectable adding... " << _impl->connectables.size() << " " << this << " " << __func__ << std::endl;
  _impl->connectables.push_back(connectable);
  std::cout << "Connectable added: " << _impl->connectables.size() << " " << this << " " << __func__ << std::endl;
}

void ModbusClientModbusIDDecoratorImpl2::sendConnectionStatus(bool connection_status) {
  std::unique_lock<std::recursive_mutex> lock(_impl->mutex);

  std::cout << "11Sending to " << _impl->connectables.size() << " connectables  (SUBTHREAD)" << this << " " << __func__ << std::endl;
  std::for_each(std::begin(_impl->connectables), std::end(_impl->connectables), [&](Connectable *connectable) {
    if (connectable != nullptr) {
      std::cout << "1566Sending to " << __func__ << std::endl;
      connectable->setConnectionStatus(connection_status);
    }
  });
}

void ModbusClientModbusIDDecoratorImpl2::checkConnectionStatusByResponse(ErrorCode response) {
  std::cout << "CHECK CONNECTION STATUS BY RESPONSE " << response << " " << _impl->is_connected << std::endl;
  if (response == DEVICE_NOT_RESPONDING_LAN || response == DEVICE_NOT_CONNECTED || response == OPERATION_INTERRUPTED) {
    if (_impl->is_connected_sent) {
      sendConnectionStatus(false);
      _impl->is_connected = false;
      _impl->is_connected_sent = false;
    }
  } else if (response == SUCCESS) {
    if (!_impl->is_connected_sent) {
      sendConnectionStatus(true);
      _impl->is_connected = true;
      _impl->is_connected_sent = true;
    }
  }
}

void ModbusClientModbusIDDecoratorImpl2::setEnabled(bool enabled, int modbus_id) {
  auto actual_enabled = enabled && _impl->is_opened;

  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->setEnabled(actual_enabled, modbus_id);
  }

  if (modbus_id == _impl->modbus_id) {
    sendConnectionStatus(actual_enabled);
  }
  ModbusWrapper::setEnabled(actual_enabled, modbus_id);
}

bool ModbusClientModbusIDDecoratorImpl2::isEnabled(int modbus_id) {
  bool result = ModbusWrapper::isEnabled(modbus_id);;
  if (_impl->modbus_wrapper != nullptr) {
    result = _impl->modbus_wrapper->isEnabled(modbus_id);
  }
  return result;
}

void ModbusClientModbusIDDecoratorImpl2::setIsOpened(bool is_opened, int modbus_id) {
  _impl->is_opened = is_opened;
  _impl->modbus_wrapper->setIsOpened(is_opened, modbus_id);
  ModbusWrapper::setIsOpened(is_opened, modbus_id);
}

bool ModbusClientModbusIDDecoratorImpl2::isOpened(int modbus_id) {
  return _impl->modbus_wrapper->isOpened( modbus_id);
}


void ModbusClientModbusIDDecoratorImpl2::process() {
  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->process();
    auto is_connected = false;
//    isConnected(is_connected);
  }
}

ErrorCode ModbusClientModbusIDDecoratorImpl2::changeAddress(const std::string &ip,
                                                            int port,
                                                            int modbus_id,
                                                            bool reconnect) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->modbus_wrapper != nullptr) {
    _impl->modbus_wrapper->changeAddress(ip, port, modbus_id, reconnect);
  }

  return result;
}
