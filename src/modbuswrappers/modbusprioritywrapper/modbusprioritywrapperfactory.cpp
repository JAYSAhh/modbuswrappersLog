#include "modbuswrappers/modbusprioritywrapper/modbusprioritywrapperfactory.h"
#include "modbuswrappers/modbusprioritywrapper/modbusprioritywrapper.h"

#include <iostream>

struct ModbusPriorityWrapperFactory::ModbusPriorityWrapperFactoryPrivate{
  std::shared_ptr<ModbusWrapperFactory> base_modbus_wrapper_factory = nullptr;
};

ModbusPriorityWrapperFactory::ModbusPriorityWrapperFactory(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory):
                                                           _impl(new ModbusPriorityWrapperFactoryPrivate()){
  _impl->base_modbus_wrapper_factory = base_modbus_wrapper_factory;
}

ModbusPriorityWrapperFactory::~ModbusPriorityWrapperFactory() {
  delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusPriorityWrapperFactory::createModbusWrapper(const std::string &ip,
                                                                                 int port,
                                                                                 int modbus_id,
                                                                                 int start_holding_reg_num,
                                                                                 int holding_regs_count,
                                                                                   int start_input_reg_num,
                                                                                 int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  std::cerr << "CANNOT CREATE MODBUS PRIORITY WRAPPER ON ID MODBUS WRAPPER" << std::endl;
  return result;
}
std::shared_ptr<ModbusWrapper> ModbusPriorityWrapperFactory::createBaseModbusWrapper(const std::string &ip, int port) {
  std::shared_ptr<ModbusWrapper> result = nullptr;

  if (_impl->base_modbus_wrapper_factory != nullptr) {
    auto base_modbus_wrapper = _impl->base_modbus_wrapper_factory->createBaseModbusWrapper(ip, port);
    if (base_modbus_wrapper != nullptr) {
      result = std::make_shared<ModbusPriorityWrapper>(base_modbus_wrapper);
    }
  }

  return result;
}
