#include "modbuswrappers/factories/fullmodbuswrapperfactoryimpl1.h"

#include "modbuswrappers/storages/modbuswrapperstorageimpl1.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperfactory.h"
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecorator.h"
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecoratorfactory.h"
#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapperfactory.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientdividablewrapperfactory.h"

using PackageType = modbus::ModbusClient::PackageType;

struct FullModbusWrapperFactoryImpl1::FullModbusWrapperFactoryImpl1Private{
  std::shared_ptr<ModbusWrapperFactory> base_modbus_wrapper_factory = nullptr;
  std::shared_ptr<ModbusWrapperFactory> modbus_wrapper_factory = nullptr;
  std::shared_ptr<ModbusWrapperStorage> modbus_wrapper_storage = nullptr;
};

FullModbusWrapperFactoryImpl1::FullModbusWrapperFactoryImpl1(): _impl(new FullModbusWrapperFactoryImpl1Private()) {
  auto modbus_wrapper_storage = std::make_shared<ModbusWrapperStorageImpl1>();
  _impl->base_modbus_wrapper_factory = std::make_shared<ModbusClientWrapperFactory>(nullptr, nullptr, 3, std::chrono::milliseconds(100), PackageType::RTU);
  auto main_modbus_wrapper_factory = std::make_shared<ModbusClientModbusIDDecoratorFactory>(_impl->base_modbus_wrapper_factory, modbus_wrapper_storage);
  auto modbus_dividable_wrapper_factory = std::make_shared<ModbusClientDividableWrapperFactory>(main_modbus_wrapper_factory);
  _impl->modbus_wrapper_factory = std::make_shared<ModbusAsyncClientWrapperFactory>(modbus_dividable_wrapper_factory);
}

FullModbusWrapperFactoryImpl1::~FullModbusWrapperFactoryImpl1() {
 delete _impl;
}

std::shared_ptr<ModbusWrapper> FullModbusWrapperFactoryImpl1::createModbusWrapper(const std::string &ip,
                                                                                  int port,
                                                                                  int modbus_id,
                                                                                  int start_holding_reg_num,
                                                                                  int holding_regs_count,
                                                                                  int start_input_reg_num,
                                                                                  int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  if (_impl->modbus_wrapper_factory != nullptr) {
    result = _impl->modbus_wrapper_factory->createModbusWrapper(ip,
                                                          port,
                                                          modbus_id,
                                                          0,
                                                          holding_regs_count,
                                                          0,
                                                          input_regs_count);
  }
  return result;
}


std::shared_ptr<ModbusWrapper> FullModbusWrapperFactoryImpl1::createBaseModbusWrapper(const std::string &ip, int port) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  if (_impl->base_modbus_wrapper_factory != nullptr) {
    result = _impl->base_modbus_wrapper_factory->createBaseModbusWrapper(ip, port);
  }

  return result;
}
