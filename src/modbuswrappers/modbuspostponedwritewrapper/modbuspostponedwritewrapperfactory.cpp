#include "modbuswrappers/modbuspostponedwritewrapper/modbuspostponedwritewrapperfactory.h"
#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapper2.h"
#include "modbuswrappers/modbuspostponedwritewrapper/modbuspostponedwritewrapper.h"

#include <iostream>

struct ModbusPostponedWriteWrapperFactory::ModbusPostponedWriteWrapperFactoryPrivate{
  std::shared_ptr<ModbusWrapperFactory> base_modbus_wrapper_factory = nullptr;
};

ModbusPostponedWriteWrapperFactory::ModbusPostponedWriteWrapperFactory(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory):
                                                                       _impl(new ModbusPostponedWriteWrapperFactoryPrivate()){
  _impl->base_modbus_wrapper_factory = base_modbus_wrapper_factory;
}

ModbusPostponedWriteWrapperFactory::~ModbusPostponedWriteWrapperFactory() {
  delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusPostponedWriteWrapperFactory::createModbusWrapper(const std::string &ip,
                                                                                       int port,
                                                                                       int modbus_id,
                                                                                       int start_holding_reg_num,
                                                                                       int holding_regs_count,
                                                                                       int start_input_reg_num,
                                                                                       int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  if (_impl->base_modbus_wrapper_factory != nullptr) {
    auto base_modbus_wrapper = _impl->base_modbus_wrapper_factory->createModbusWrapper(ip,
                                                                                 port,
                                                                                 modbus_id,
                                                                                 0,
                                                                                 holding_regs_count,
                                                                                 0,
                                                                                 input_regs_count);
    if (base_modbus_wrapper != nullptr) {
      auto async_client_wrapper = std::make_shared<ModbusAsyncClientWrapper2>(base_modbus_wrapper,
                                                                              start_holding_reg_num,
                                                                              holding_regs_count,
                                                                              start_input_reg_num,
                                                                              input_regs_count);
      result = std::make_shared<ModbusPostponedWriteWrapper>(async_client_wrapper);
    }
  }
  return result;
}


std::shared_ptr<ModbusWrapper> ModbusPostponedWriteWrapperFactory::createBaseModbusWrapper(const std::string &ip,
                                                                                           int port) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  std::cerr << "Cannot create modbus wrapper of PostponedWriteModbusWrapper decorator without MODBUS ID and holding and input regs count " << std::endl;
  return result;
}
