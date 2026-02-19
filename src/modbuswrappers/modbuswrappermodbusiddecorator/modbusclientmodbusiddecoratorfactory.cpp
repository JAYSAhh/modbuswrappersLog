#include "modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecoratorfactory.h"
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecorator.h"

#include "modbuswrappers/modbuswrapperstorage.h"

#include <iostream>

struct ModbusClientModbusIDDecoratorFactory::ModbusClientModbusIDDecoratorFactoryPrivate{
  std::shared_ptr<ModbusWrapperStorage> modbus_wrapper_storage = nullptr;
  std::shared_ptr<ModbusWrapperFactory> base_modbus_wrapper_factory = nullptr;
};

ModbusClientModbusIDDecoratorFactory::ModbusClientModbusIDDecoratorFactory(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory,
                                                                           const std::shared_ptr<ModbusWrapperStorage> &modbus_wrapper_storage):
                                                                           _impl(new ModbusClientModbusIDDecoratorFactoryPrivate()){
  _impl->modbus_wrapper_storage = modbus_wrapper_storage;
  _impl->base_modbus_wrapper_factory = base_modbus_wrapper_factory;
}

ModbusClientModbusIDDecoratorFactory::~ModbusClientModbusIDDecoratorFactory() {
 delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusClientModbusIDDecoratorFactory::createModbusWrapper(const std::string &ip,
                                                                                         int port,
                                                                                         int modbus_id,
                                                                                         int start_holding_reg_num,
                                                                                         int holding_regs_count,
                                                                                         int start_input_reg_num,
                                                                                         int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  if (_impl->modbus_wrapper_storage != nullptr) {
    auto base_modbus_wrapper = _impl->modbus_wrapper_storage->getBaseModbusWrapper(ip, port);
    if (base_modbus_wrapper == nullptr) {
      if (_impl->base_modbus_wrapper_factory != nullptr) {
        base_modbus_wrapper = _impl->base_modbus_wrapper_factory->createBaseModbusWrapper(ip, port);
      }
    }

    if (base_modbus_wrapper != nullptr) {
      result = std::make_shared<ModbusClientModbusIDDecorator>(base_modbus_wrapper, modbus_id);
      _impl->modbus_wrapper_storage->addBaseModbusWrapper(base_modbus_wrapper, ip, port);
    }
  }

  return result;
}
std::shared_ptr<ModbusWrapper> ModbusClientModbusIDDecoratorFactory::createBaseModbusWrapper(const std::string &ip,
                                                                                             int port) {
  std::cerr << "MODBUS WRAPPER SHOULD HAS MODBUS ID TO BE CREATED VIA THIS FACTORY " << __func__ << std::endl;
  return nullptr;
}
