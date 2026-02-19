#include "modbuswrappers/service/modbus_wrapper_model.h"

#include <iostream>

#include <utils/services/uidgenerator.h>
#include "modbuswrappers/extra/modbus_slave_factory.h"

struct ModbusWrapperModel::ModbusWrapperModelPrivate{
  ModbusWrapperModelStruct data;
  std::shared_ptr<UIDGenerator> uid_generator = nullptr;

  /**
 * Фабрика, для инициализации слейвов
 */
  std::shared_ptr<ModbusSlaveFactory> modbus_slave_factory = nullptr;

  /**
   * Фабрика для инициализации оберток слейвов
   */
  std::shared_ptr<ModbusSlaveWrapperFactory> modbus_slave_wrapper_factory = nullptr;
};

ModbusWrapperModel::ModbusWrapperModel(const std::shared_ptr<UIDGenerator> &uid_generator,
                                       const std::shared_ptr<ModbusWrapper> &base_modbus_wrapper,
                                       const std::shared_ptr<ModbusSlaveFactory> &modbus_slave_factory,
                                       const std::shared_ptr<ModbusSlaveWrapperFactory> &modbus_slave_wrapper_factory):
                                       _impl(new ModbusWrapperModelPrivate()){
  _impl->uid_generator = uid_generator;
  _impl->modbus_slave_factory = modbus_slave_factory;
  _impl->modbus_slave_wrapper_factory = modbus_slave_wrapper_factory;
  _impl->data.base_modbus_wrapper = base_modbus_wrapper;
}

ModbusWrapperModel::~ModbusWrapperModel() {
  delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusWrapperModel::getBaseModbusWrapper() {
  std::shared_ptr<ModbusWrapper> result = _impl->data.base_modbus_wrapper;
  return result;
}

std::shared_ptr<ModbusWrapper> ModbusWrapperModel::getModbusSlave(int modbus_id) {
  std::shared_ptr<ModbusWrapper> result = nullptr;


  auto &modbus_slaves = _impl->data.slaves;
  if (modbus_slaves.count(modbus_id)) {
    result = modbus_slaves.at(modbus_id).slave;
  }

  return result;
}

std::shared_ptr<ModbusSlaveWrapper> ModbusWrapperModel::getModbusSlaveWrapper(const UID &uid) {
  std::shared_ptr<ModbusSlaveWrapper> result = nullptr;

  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  result = modbus_slave_wrapper_struct.slave_wrapper;

  return result;
}

std::string ModbusWrapperModel::getBaseModbusWrapperIP() {
  std::string result;

  auto &base_modbus_wrapper = _impl->data.base_modbus_wrapper;
  if (base_modbus_wrapper != nullptr) {
    result = base_modbus_wrapper->getIP();
  }

  return result;
}

int ModbusWrapperModel::getBaseModbusWrapperPort() {
  int result = 0;

  auto &base_modbus_wrapper = _impl->data.base_modbus_wrapper;
  if (base_modbus_wrapper != nullptr) {
    result = base_modbus_wrapper->getPort();
  }

  return result;
}

int ModbusWrapperModel::getModbusSlaveWrapperStartHoldingRegNum(const UID &uid) {
  int result = 0;

  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_slave_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_slave_wrapper != nullptr) {
    result = modbus_slave_wrapper->getStartHoldingRegsNum();
  }
  return result;
}

int ModbusWrapperModel::getModbusSlaveWrapperHoldingRegsCount(const UID &uid) {
  int result = 0;

  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_slave_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_slave_wrapper != nullptr) {
    result = modbus_slave_wrapper->getHoldingRegsCount();
  }
  return result;
}

int ModbusWrapperModel::getModbusSlaveWrapperStartInputRegNum(const UID &uid) {
  int result = 0;

  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_slave_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_slave_wrapper != nullptr) {
    result = modbus_slave_wrapper->getStartInputRegsNum();
  }
  return result;
}

int ModbusWrapperModel::getModbusSlaveWrapperInputRegsCount(const UID &uid) {
  int result = 0;

  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_slave_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_slave_wrapper != nullptr) {
    result = modbus_slave_wrapper->getInputRegsCount();
  }
  return result;
}

int ModbusWrapperModel::getModbusSlaveWrapperPeriodInMS(const UID &uid) {
  int result = 0;

  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_slave_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_slave_wrapper != nullptr) {
    result = modbus_slave_wrapper->getMinUpdatePeriodInMs();
  }
  return result;
}

void ModbusWrapperModel::setModbusSlaveWrapperStartHoldingRegNum(const UID &uid, int reg_num) {
  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_wrapper != nullptr) {
    auto error_code = modbus_wrapper->setStartHoldingRegsNum(reg_num);
  }
}

void ModbusWrapperModel::setModbusSlaveWrapperHoldingRegsCount(const UID &uid, int holding_regs_count) {
  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_wrapper != nullptr) {
    auto error_code = modbus_wrapper->setHoldingRegsCount(holding_regs_count);
  }
}

void ModbusWrapperModel::setModbusSlaveWrapperStartInputRegNum(const UID &uid, int reg_num) {
  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_wrapper != nullptr) {
    auto error_code = modbus_wrapper->setStartInputRegsNum(reg_num);
  }
}

void ModbusWrapperModel::setModbusSlaveWrapperInputRegsCount(const UID &uid, int input_regs_count) {
  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_wrapper != nullptr) {
    auto error_code = modbus_wrapper->setHoldingRegsCount(input_regs_count);
  }
}

void ModbusWrapperModel::setModbusSlaveWrapperPeriodInMS(const UID &uid, int period_in_ms) {
  auto modbus_slave_wrapper_struct = getModbusSlaveWrapperByUID(uid);
  auto &modbus_wrapper = modbus_slave_wrapper_struct.slave_wrapper;
  if (modbus_wrapper != nullptr) {
    auto error_code = modbus_wrapper->setMinUpdatePeriodInMs(period_in_ms);
  }
}



void ModbusWrapperModel::changeBaseModbusWrapperAddress(const std::string &ip, int port) {
  auto &base_modbus_wrapper = _impl->data.base_modbus_wrapper;
  if (base_modbus_wrapper != nullptr) {
    base_modbus_wrapper->changeAddress(ip, port);
  }
}

std::vector<int> ModbusWrapperModel::getModbusSlaves() {
  std::vector<int> result = {};


  auto &slaves = _impl->data.slaves;
  result.reserve(slaves.size());
  for (auto & slave : slaves) {
    result.push_back(slave.first);
  }

  return result;
}

bool ModbusWrapperModel::hasModbusSlave(int modbus_id) {
  auto result = false;
  result = _impl->data.slaves.count(modbus_id);
  return result;
}

void ModbusWrapperModel::addModbusSlave(int modbus_id) {
  if (hasModbusSlave(modbus_id)) {
    std::cout << "ALREADY HAS MODBUS SLVAE " << modbus_id;
    return;
  }

  auto &base_modbus_wrapper = _impl->data.base_modbus_wrapper;
  if (_impl->modbus_slave_factory != nullptr && base_modbus_wrapper != nullptr) {
    auto modbus_slave = _impl->modbus_slave_factory->createModbusSlave(base_modbus_wrapper, modbus_id);
    _impl->data.slaves.insert({modbus_id, {modbus_slave, {}}});
  }
}

void ModbusWrapperModel::removeModbusSlave(int modbus_id) {
  if (hasModbusSlave(modbus_id)) {
    _impl->data.slaves.erase(modbus_id);
  }
}

std::vector<UID> ModbusWrapperModel::getModbusSlaveWrappers(int modbus_id) {
  std::vector<UID> result = {};

  if (hasModbusSlave(modbus_id)) {
    auto modbus_slave_struct = _impl->data.slaves[modbus_id];
    auto &slave_wrappers = modbus_slave_struct.slave_wrappers;
    result.reserve(slave_wrappers.size());

    for (const auto &slave_wrapper: slave_wrappers) {
      result.push_back(slave_wrapper.first);
    }
  }

  return result;
}

bool ModbusWrapperModel::hasModbusSlaveWrapper(const UID &uid) {
  auto result = false;

  auto &slaves = _impl->data.slaves;
  for (const auto &slave: slaves) {
    auto &slave_wrappers = slave.second.slave_wrappers;
    for (const auto &slave_wrapper: slave_wrappers) {
      result = slave_wrapper.first == uid;
      if (result) return result;
    }
  }

  return result;
}

UID ModbusWrapperModel::addModbusSlaveWrapper(int modbus_id,
                                              int start_holding_reg_num,
                                              int holding_regs_count,
                                              int start_input_reg_num,
                                              int input_regs_count,
                                              int period_in_ms) {
  UID result;

  auto &slaves = _impl->data.slaves;

  if (_impl->uid_generator != nullptr && _impl->modbus_slave_wrapper_factory != nullptr) {
    auto new_uid = _impl->uid_generator->generateUIDString();

    if (!hasModbusSlave(modbus_id)) {
      addModbusSlave(modbus_id);
    }

    auto &modbus_slave = slaves[modbus_id];
    auto modbus_slave_wrapper = _impl->modbus_slave_wrapper_factory->createModbusSlaveWrapper(modbus_slave.slave,
                                                                                        start_holding_reg_num,
                                                                                        holding_regs_count,
                                                                                        start_input_reg_num,
                                                                                        input_regs_count,
                                                                                        period_in_ms);

    modbus_slave.slave_wrappers.insert({new_uid, {modbus_slave_wrapper}});
    result = new_uid;

  }

  return result;
}

void ModbusWrapperModel::removeModbusSlaveWrapper(const UID &uid) {
  auto &slaves = _impl->data.slaves;
  for (auto &slave: slaves) {
    auto &slave_wrappers = slave.second.slave_wrappers;
    slave_wrappers.erase(uid);
  }
}

ModbusSlaveWrapperStruct ModbusWrapperModel::getModbusSlaveWrapperByUID(const UID &uid) {
  ModbusSlaveWrapperStruct result;

  auto &modbus_slaves = _impl->data.slaves;
  for (auto &modbus_slave: modbus_slaves) {
    auto &modbus_slave_wrappers = modbus_slave.second.slave_wrappers;

    if (modbus_slave_wrappers.count(uid)) {
      result = modbus_slave_wrappers.at(uid);
      break;
    }
  }

  return result;
}
