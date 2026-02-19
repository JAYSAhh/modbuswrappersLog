#include "modbuswrappers/modbuswrappermodbusiddecorator/modbus_client_modbus_id_decorator_factory_impl2.h"

#include <iostream>

#include <utils/services/retryers/exponential_backoff_retryer_impl1.h>
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbus_client_modbus_id_decorator_impl2.h"
#include "modbuswrappers/modbuswrapperstorage.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapper.h"


struct ModbusClientModbusIDDecoratorFactoryImpl2::ModbusClientModbusIDDecoratorFactoryImpl2Private{
  std::shared_ptr<ModbusWrapperStorage> modbus_wrapper_storage = nullptr;
  std::shared_ptr<ModbusWrapperFactory> base_modbus_wrapper_factory = nullptr;

  int initial_delay = 200;
  int max_delay = 60000;
  double factor = 3;
  int jitter = 300;
};

ModbusClientModbusIDDecoratorFactoryImpl2::ModbusClientModbusIDDecoratorFactoryImpl2(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory,
                                                                                     const std::shared_ptr<ModbusWrapperStorage> &modbus_wrapper_storage):
                                                                                     _impl(new ModbusClientModbusIDDecoratorFactoryImpl2Private()){
  _impl->modbus_wrapper_storage = modbus_wrapper_storage;
  _impl->base_modbus_wrapper_factory = base_modbus_wrapper_factory;
}

ModbusClientModbusIDDecoratorFactoryImpl2::ModbusClientModbusIDDecoratorFactoryImpl2(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory,
                                                                                     const std::shared_ptr<ModbusWrapperStorage> &modbus_wrapper_storage,
                                                                                     int initial_delay,
                                                                                     int max_delay,
                                                                                     int factor,
                                                                                     int jitter):
                                                                                     _impl(new ModbusClientModbusIDDecoratorFactoryImpl2Private()){
  _impl->modbus_wrapper_storage = modbus_wrapper_storage;
  _impl->base_modbus_wrapper_factory = base_modbus_wrapper_factory;
  _impl->initial_delay = initial_delay;
  _impl->max_delay = max_delay;
  _impl->factor = factor;
  _impl->jitter = jitter;
}

ModbusClientModbusIDDecoratorFactoryImpl2::~ModbusClientModbusIDDecoratorFactoryImpl2() {
  delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusClientModbusIDDecoratorFactoryImpl2::createModbusWrapper(const std::string &ip,
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
      auto exponential_backoff_retryer = std::make_shared<ExponentialBackoffRetryerImpl1>(_impl->initial_delay,
                                                                                          _impl->max_delay,
                                                                                          _impl->factor,
                                                                                          _impl->jitter);

      auto base_modbus_client_wrapper = std::dynamic_pointer_cast<ModbusClientWrapper>(base_modbus_wrapper);
      if (base_modbus_client_wrapper != nullptr) {
        result = std::make_shared<ModbusClientModbusIDDecoratorImpl2>(base_modbus_client_wrapper, modbus_id, exponential_backoff_retryer);
        _impl->modbus_wrapper_storage->addBaseModbusWrapper(base_modbus_wrapper, ip, port);
      }
    }
  }

  return result;
}

std::shared_ptr<ModbusWrapper> ModbusClientModbusIDDecoratorFactoryImpl2::createBaseModbusWrapper(const std::string &ip,
                                                                                             int port) {
  std::cerr << "MODBUS WRAPPER SHOULD HAS MODBUS ID TO BE CREATED VIA THIS FACTORY " << __func__ << std::endl;
  return nullptr;
}
