#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapperfactory.h"
#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapper2.h"

#include <iostream>

struct ModbusAsyncClientWrapperFactory::ModbusAsyncClientWrapperFactoryPrivate{
  /**
 * \warning ДЛЯ ИСПОЛЬЗОВАНИЯ КЛАССА НЕОБХОДИМ ОБЯЗАТЕЛЬНО БАЗОВЫЙ ОБЪЕКТ МОДБАС ПОДКЛЮЧЕНИЯ С MODBUSID декоратором
 */
  std::shared_ptr<ModbusWrapperFactory> base_modbus_wrapper_factory = nullptr;
};

ModbusAsyncClientWrapperFactory::ModbusAsyncClientWrapperFactory(const std::shared_ptr<ModbusWrapperFactory> &modbus_wrapper_factory):
                                                                 _impl(new ModbusAsyncClientWrapperFactoryPrivate()){
  _impl->base_modbus_wrapper_factory = modbus_wrapper_factory;
}

ModbusAsyncClientWrapperFactory::~ModbusAsyncClientWrapperFactory() {
  delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusAsyncClientWrapperFactory::createModbusWrapper(const std::string &ip,
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
                                                                                 start_holding_reg_num,
                                                                                 holding_regs_count,
                                                                                 start_input_reg_num,
                                                                                 input_regs_count);
    if (base_modbus_wrapper != nullptr) {
      result = std::make_shared<ModbusAsyncClientWrapper2>(base_modbus_wrapper,
                                                           start_holding_reg_num,
                                                           holding_regs_count,
                                                           start_input_reg_num,
                                                           input_regs_count);
    }
  }
  return result;
}


std::shared_ptr<ModbusWrapper> ModbusAsyncClientWrapperFactory::createBaseModbusWrapper(const std::string &ip,
                                                                                        int port) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  std::cerr << "Cannot create modbus wrapper of AsyncClientWrapper decorator without MODBUS ID and holding and input regs count " << std::endl;
  return result;
}

