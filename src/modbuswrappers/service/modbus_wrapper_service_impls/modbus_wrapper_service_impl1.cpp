#include "modbuswrappers/service/modbus_wrapper_service_impls/modbus_wrapper_service_impl1.h"

#include <iostream>

#include "modbuswrappers/service/modbus_wrapper_model.h"

struct ModbusWrapperServiceImpl1::ModbusWrapperServiceImpl1Private{
  std::shared_ptr<ModbusWrapperModel> model = nullptr;
};

ModbusWrapperServiceImpl1::ModbusWrapperServiceImpl1(const std::shared_ptr<ModbusWrapperModel> &model):
                                                     _impl(new ModbusWrapperServiceImpl1Private()){
  _impl->model = model;
}

ModbusWrapperServiceImpl1::~ModbusWrapperServiceImpl1() {
  delete _impl;
}

std::tuple<ErrorCode, std::string, int> ModbusWrapperServiceImpl1::getTCPConnectionData() {
  std::tuple<ErrorCode, std::string, int> result = {OPERATION_INTERRUPTED, "", 0};


  if (_impl->model != nullptr) {
    auto ip = _impl->model->getBaseModbusWrapperIP();
    auto port = _impl->model->getBaseModbusWrapperPort();

    std::get<0>(result) = SUCCESS;
    std::get<1>(result) = ip;
    std::get<2>(result) = port;
  }

  return result;
}

ErrorCode ModbusWrapperServiceImpl1::setTCPConnectionData(const std::string &ip, int port) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->model != nullptr) {
    _impl->model->changeBaseModbusWrapperAddress(ip, port);
  }

  return result;
}

std::tuple<ErrorCode, ConnectionStatus> ModbusWrapperServiceImpl1::isConnectedTCP() {
  // TODO: Потом исправить этот метод!
  std::tuple<ErrorCode, ConnectionStatus> result = {OPERATION_INTERRUPTED, ConnectionStatus_NOT_CONNECTED};

  if (_impl->model != nullptr) {
    auto base_modbus_wrapper = _impl->model->getBaseModbusWrapper();
    if (base_modbus_wrapper != nullptr) {
      bool is_connected = false;
      std::get<0>(result) = base_modbus_wrapper->isConnected(is_connected);
      std::get<1>(result) = is_connected ? ConnectionStatus_CONNECTED: ConnectionStatus_NOT_CONNECTED;
    }
  }

  return result;
}

ErrorCode ModbusWrapperServiceImpl1::connectTCP() {
  // TODO: Исправить этот метод!
  auto result = OPERATION_INTERRUPTED;

  if (_impl->model != nullptr) {
    auto base_modbus_wrapper = _impl->model->getBaseModbusWrapper();
    if (base_modbus_wrapper != nullptr) {
      result = base_modbus_wrapper->connect();
    }
  }

  return result;
}

ErrorCode ModbusWrapperServiceImpl1::disconnectTCP() {
  // TODO: Исправить этот метод!
  auto result = OPERATION_INTERRUPTED;

  if (_impl->model != nullptr) {
    auto base_modbus_wrapper = _impl->model->getBaseModbusWrapper();
    if (base_modbus_wrapper != nullptr) {
      result = base_modbus_wrapper->disconnect();
    }
  }

  return result;
}

std::tuple<ErrorCode, ConnectionStatus> ModbusWrapperServiceImpl1::isConnected(int modbus_id) {
  std::tuple<ErrorCode, ConnectionStatus> result = {OPERATION_INTERRUPTED, ConnectionStatus_NOT_CONNECTED};

  if (_impl->model != nullptr) {
    auto base_modbus_wrapper = _impl->model->getBaseModbusWrapper();
    if (base_modbus_wrapper != nullptr) {
      bool is_connected = false;
      std::get<0>(result) = base_modbus_wrapper->isConnected(is_connected, modbus_id);
      std::get<1>(result) = is_connected ? ConnectionStatus_CONNECTED: ConnectionStatus_NOT_CONNECTED;
    }
  }

  return result;
}

ErrorCode ModbusWrapperServiceImpl1::connect(int modbus_id) {
  auto result = OPERATION_INTERRUPTED;

  auto modbus_slave = getModbusSlave(modbus_id);
  auto modbus_slave_wrapper_uids = getModbusSlaveWrappers(modbus_id);
  for (const auto &modbus_slave_wrapper_uid: modbus_slave_wrapper_uids) {
    auto modbus_slave_wrapper = getModbusSlaveWrapper(modbus_slave_wrapper_uid);
    if (modbus_slave_wrapper != nullptr) {
      // TODO: Продумать схему перехвата кода ошибки
      result = modbus_slave_wrapper->connect();
    }
  }

  return result;
}

ErrorCode ModbusWrapperServiceImpl1::disconnect(int modbus_id) {
  auto result = OPERATION_INTERRUPTED;

  auto modbus_slave = getModbusSlave(modbus_id);
  if (modbus_slave != nullptr) {
    result = modbus_slave->disconnect();
  }

  return result;
}

std::shared_ptr<ModbusWrapper> ModbusWrapperServiceImpl1::getModbusSlave(int modbus_id) {
  std::shared_ptr<ModbusWrapper> result = nullptr;

  if (_impl->model != nullptr) {
    result = _impl->model->getModbusSlave(modbus_id);
  }

  return result;
}

std::vector<int> ModbusWrapperServiceImpl1::getModbusSlaves() {
  std::vector<int> result;
  if (_impl->model != nullptr) {
    result = _impl->model->getModbusSlaves();
  }
  return result;
}

bool ModbusWrapperServiceImpl1::hasModbusSlave(int modbus_id) {
  auto result = false;

  if (_impl->model != nullptr) {
    result = _impl->model->hasModbusSlave(modbus_id);
  }

  return result;
}

std::vector<UID> ModbusWrapperServiceImpl1::getModbusSlaveWrappers(int modbus_id) {
  std::vector<UID> result;

  if (_impl->model != nullptr) {
    result = _impl->model->getModbusSlaveWrappers(modbus_id);
  }

  return result;
}


ErrorCode ModbusWrapperServiceImpl1::addModbusSlave(int modbus_slave) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->model != nullptr) {
    _impl->model->addModbusSlave(modbus_slave);
    result = SUCCESS;
  }

  return result;
}

std::tuple<ErrorCode, std::string> ModbusWrapperServiceImpl1::addModbusSlaveWrapper(int modbus_id,
                                                                                    int start_holding_reg_num,
                                                                                    int holding_regs_count,
                                                                                    int start_input_reg_num,
                                                                                    int input_regs_count,
                                                                                    int period_in_ms) {
  std::tuple<ErrorCode, std::string> result = {OPERATION_INTERRUPTED, ""};

 if (_impl->model != nullptr) {
   auto new_modbus_slave_wrapper_uid = _impl->model->addModbusSlaveWrapper(modbus_id,
                                                                     start_holding_reg_num,
                                                                     holding_regs_count,
                                                                     start_input_reg_num,
                                                                     input_regs_count,
                                                                     period_in_ms);
   result = {SUCCESS, new_modbus_slave_wrapper_uid};
 }

  return result;
}

std::tuple<int, int> ModbusWrapperServiceImpl1::getModbusSlaveWrapperHoldingRegsRange(const UID &uid) {
  std::tuple<int, int> result = {0, 0};

  if (_impl->model != nullptr) {
    auto modbus_wrapper = _impl->model->getModbusSlaveWrapper(uid);
    if (modbus_wrapper != nullptr) {
      auto modbus_slave_wrapper_start_holding_reg_num = modbus_wrapper->getStartHoldingRegsNum();
      auto modbus_slave_wrapper_holding_regs_count = modbus_wrapper->getHoldingRegsCount();

      result = {modbus_slave_wrapper_start_holding_reg_num,
                modbus_slave_wrapper_holding_regs_count};
    }
  }

  return result;
}

std::tuple<int, int> ModbusWrapperServiceImpl1::getModbusSlaveWrapperInputRegsRange(const UID &uid) {
  std::tuple<int, int> result = {0, 0};

  if (_impl->model != nullptr) {
    auto modbus_wrapper = _impl->model->getModbusSlaveWrapper(uid);
    if (modbus_wrapper != nullptr) {
      auto modbus_slave_wrapper_start_input_reg_num = modbus_wrapper->getStartInputRegsNum();
      auto modbus_slave_wrapper_input_regs_count = modbus_wrapper->getInputRegsCount();

      result = {modbus_slave_wrapper_start_input_reg_num,
                modbus_slave_wrapper_input_regs_count};
    }
  }

  return result;
}

int ModbusWrapperServiceImpl1::getModbusSlaveWrapperMinUpdatePeriodInMs(const UID &uid) {
  int result = 0;

  if (_impl->model != nullptr) {
    auto modbus_wrapper = _impl->model->getModbusSlaveWrapper(uid);
    if (modbus_wrapper != nullptr) {
      auto modbus_slave_wrapper_min_update_period_in_ms = modbus_wrapper->getMinUpdatePeriodInMs();
      result = modbus_slave_wrapper_min_update_period_in_ms;
    }
  }

  return result;
}

ErrorCode ModbusWrapperServiceImpl1::removeModbusSlave(int modbus_id) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->model != nullptr) {
    _impl->model->removeModbusSlave(modbus_id);
    result = SUCCESS;
  }

  return result;
}

ErrorCode ModbusWrapperServiceImpl1::removeModbusSlaveWrapper(const std::string &uid) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->model != nullptr) {
    _impl->model->removeModbusSlaveWrapper(uid);
  }

  return result;
}

void ModbusWrapperServiceImpl1::addConnectable(int modbus_id, Connectable *connectable) {
  auto modbus_slave = getModbusSlave(modbus_id);
  if (modbus_slave != nullptr) {
    modbus_slave->addConnectable(connectable);
  }
}

void ModbusWrapperServiceImpl1::process() {
  auto modbus_slave_ids = getModbusSlaves();
  for (auto modbus_slave_id: modbus_slave_ids) {
    auto modbus_slave_wrapper_ids = getModbusSlaveWrappers(modbus_slave_id);
    for (const auto &modbus_slave_wrapper_uid: modbus_slave_wrapper_ids) {
      auto modbus_slave_wrapper = getModbusSlaveWrapper(modbus_slave_wrapper_uid);
      if (modbus_slave_wrapper != nullptr) {
        std::cout << "Modbus Slave Wrapper (" << modbus_slave_id << ") with UID: " << modbus_slave_wrapper_uid << " ";
        modbus_slave_wrapper->process();
        std::cout << std::endl;
      }
    }
  }
}

ErrorCode ModbusWrapperServiceImpl1::readHoldingRegister(int reg_num,
                                                              uint16_t &value,
                                                              int modbus_id,
                                                              Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->model != nullptr) {
    auto modbus_slave = _impl->model->getModbusSlave(modbus_id);
    if (modbus_slave != nullptr) {
      auto modbus_slave_wrapper = getModbusSlaveWrapperByHoldingRegsRange(modbus_id, reg_num, 1);
      if (modbus_slave_wrapper != nullptr) {
        result = modbus_slave_wrapper->readHoldingRegister(reg_num, value, modbus_id, priority);
      } else {
        result = INVALID_REQUEST;
      }
    } else {
      result = DEVICE_DOESNT_EXIST;
    }
  }

  return result;
}

ErrorCode ModbusWrapperServiceImpl1::writeHoldingRegister(int reg_num,
                                                          uint16_t value,
                                                          int modbus_id,
                                                          Priority priority) {
  auto result = OPERATION_INTERRUPTED;

  if (_impl->model != nullptr) {
    auto modbus_slave = _impl->model->getModbusSlave(modbus_id);
    if (modbus_slave != nullptr) {
      auto modbus_slave_wrapper = getModbusSlaveWrapperByHoldingRegsRange(modbus_id, reg_num, 1);
      if (modbus_slave_wrapper != nullptr) {
        result = modbus_slave_wrapper->writeHoldingRegister(reg_num, value, modbus_id, priority);
      } else {
        result = INVALID_REQUEST;
      }
    } else {
      result = DEVICE_DOESNT_EXIST;
    }
  }

  return result;
}

ErrorCode ModbusWrapperServiceImpl1::readInputRegisters(int reg_num,
                                                        int reg_count,
                                                        std::vector<uint16_t> &values,
                                                        int modbus_id,
                                                        Priority priority) {
  return DEVICE_NOT_RESPONDING_LAN;
}


std::shared_ptr<ModbusSlaveWrapper> ModbusWrapperServiceImpl1::getModbusSlaveWrapper(const UID &uid) {
  std::shared_ptr<ModbusSlaveWrapper> result = nullptr;

  if (_impl->model != nullptr) {
    result = _impl->model->getModbusSlaveWrapper(uid);
  }

  return result;
}

std::shared_ptr<ModbusSlaveWrapper> ModbusWrapperServiceImpl1::getModbusSlaveWrapperByHoldingRegsRange(int modbus_id,
                                                                                                       int first_holding_reg_num,
                                                                                                       int holding_regs_count) {

  std::shared_ptr<ModbusSlaveWrapper> result = nullptr;

  auto modbus_slave_wrapper_uids = getModbusSlaveWrappers(modbus_id);
  for (const auto &modbus_slave_wrapper_uid: modbus_slave_wrapper_uids) {
    auto holding_regs_range = getModbusSlaveWrapperHoldingRegsRange(modbus_slave_wrapper_uid);

    auto first_holding_reg_num_in_range = std::get<0>(holding_regs_range);
    auto holding_regs_count_in_range = std::get<1>(holding_regs_range);
    auto last_holding_reg_num_in_range = first_holding_reg_num_in_range + holding_regs_count_in_range;

    auto last_holding_reg_num = first_holding_reg_num + holding_regs_count;

    if (first_holding_reg_num >= first_holding_reg_num_in_range &&
        first_holding_reg_num < last_holding_reg_num_in_range &&
        last_holding_reg_num < last_holding_reg_num_in_range ) {
      result = getModbusSlaveWrapper(modbus_slave_wrapper_uid);
      break;
    }
  }

  return result;
}
std::shared_ptr<ModbusSlaveWrapper> ModbusWrapperServiceImpl1::getModbusSlaveWrapperByInputRegsRange(int modbus_id,
                                                                                                     int first_input_reg_num,
                                                                                                     int input_regs_count) {
  std::shared_ptr<ModbusSlaveWrapper> result = nullptr;

  auto modbus_slave_wrapper_uids = getModbusSlaveWrappers(modbus_id);
  for (const auto &modbus_slave_wrapper_uid: modbus_slave_wrapper_uids) {
    auto input_regs_range = getModbusSlaveWrapperInputRegsRange(modbus_slave_wrapper_uid);

    auto first_input_reg_num_in_range = std::get<0>(input_regs_range);
    auto input_regs_count_in_range = std::get<1>(input_regs_range);
    auto last_input_reg_num_in_range = first_input_reg_num_in_range + input_regs_count_in_range;

    auto last_input_reg_num = first_input_reg_num + input_regs_count;

    if (first_input_reg_num >= first_input_reg_num_in_range &&
        first_input_reg_num < last_input_reg_num_in_range &&
        last_input_reg_num < last_input_reg_num_in_range ) {
      result = getModbusSlaveWrapper(modbus_slave_wrapper_uid);
      break;
    }
  }

  return result;
}

