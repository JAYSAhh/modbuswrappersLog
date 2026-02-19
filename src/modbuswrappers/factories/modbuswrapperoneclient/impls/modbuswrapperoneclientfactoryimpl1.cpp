#include "modbuswrappers/factories/modbuswrapperoneclient/impls/modbuswrapperoneclientfactoryimpl1.h"

#include <modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecorator.h>
#include <modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapper2.h>


struct ModbusWrapperOneClientFactoryImpl1::ModbusWrapperOneClientFactoryImpl1Private{
  std::shared_ptr<ModbusWrapper> base_modbus_wrapper = nullptr;
  std::shared_ptr<ModbusWrapperFactory> base_modbus_wrapper_factory = nullptr;
};

ModbusWrapperOneClientFactoryImpl1::ModbusWrapperOneClientFactoryImpl1(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory):
                                                                       _impl(new ModbusWrapperOneClientFactoryImpl1Private()){
  _impl->base_modbus_wrapper_factory = base_modbus_wrapper_factory;
}

ModbusWrapperOneClientFactoryImpl1::~ModbusWrapperOneClientFactoryImpl1() {
  delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusWrapperOneClientFactoryImpl1::createModbusWrapper(const std::string &ip,
                                                                                       int port,
                                                                                       int modbus_id,
                                                                                       int start_holding_reg_num,
                                                                                       int holding_regs_count,
                                                                                       int start_input_reg_num,
                                                                                       int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = nullptr;

  if (_impl->base_modbus_wrapper != nullptr) {
    auto modbus_id_decorator = std::make_shared<ModbusClientModbusIDDecorator>(_impl->base_modbus_wrapper, modbus_id);
    auto modbus_async_decorator = std::make_shared<ModbusAsyncClientWrapper2>(modbus_id_decorator,
                                                                              start_holding_reg_num,
                                                                              holding_regs_count,
                                                                              start_input_reg_num,
                                                                              input_regs_count);
    result = modbus_async_decorator;
  }

  return result;
}

std::shared_ptr<ModbusWrapper> ModbusWrapperOneClientFactoryImpl1::createBaseModbusWrapper(const std::string &ip,
                                                                                           int port) {
  if (_impl->base_modbus_wrapper_factory != nullptr) {
    auto new_base_modbus_wrapper = _impl->base_modbus_wrapper_factory->createBaseModbusWrapper(ip, port);
    if (new_base_modbus_wrapper != nullptr) {
      _impl->base_modbus_wrapper = new_base_modbus_wrapper;
    }
  }

  return _impl->base_modbus_wrapper;
}
