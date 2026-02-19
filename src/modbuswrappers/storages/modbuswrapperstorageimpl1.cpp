#include "modbuswrappers/storages/modbuswrapperstorageimpl1.h"

#include <map>

struct ModbusWrapperStorageImpl1::ModbusWrapperStorageImpl1Private{
  std::map<std::pair<std::string, int>, std::shared_ptr<ModbusWrapper>> base_modbus_wrappers;
};

ModbusWrapperStorageImpl1::ModbusWrapperStorageImpl1():ModbusWrapperStorage(), _impl(new ModbusWrapperStorageImpl1Private()) {

}

ModbusWrapperStorageImpl1::~ModbusWrapperStorageImpl1() {
  delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusWrapperStorageImpl1::getBaseModbusWrapper(const std::string &ip, int port) {
  std::shared_ptr<ModbusWrapper> result;
  auto ip_port = std::pair<std::string, int>(ip, port);
  if (_impl->base_modbus_wrappers.count(ip_port) != 0) {
    result = _impl->base_modbus_wrappers[ip_port];
  }
  return result;
}

void ModbusWrapperStorageImpl1::addBaseModbusWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                                     const std::string &ip,
                                                     int port) {
  auto ip_port = std::pair<std::string, int>(ip, port);
  _impl->base_modbus_wrappers.insert({ip_port, modbus_wrapper});
}
