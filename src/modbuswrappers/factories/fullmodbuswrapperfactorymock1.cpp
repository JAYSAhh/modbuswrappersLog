#include "modbuswrappers/factories/fullmodbuswrapperfactorymock1.h"

#include "modbuswrappers/storages/modbuswrapperstorageimpl1.h"
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecorator.h"
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecoratorfactory.h"
#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapperfactory.h"
#include "modbuswrappers/mocks/modbuswrappermockfactory.h"


struct FullModbusWrapperFactoryMock1::FullModbusWrapperFactoryMock1Private{
  std::shared_ptr<ModbusWrapperFactory> base_modbus_wrapper_factory = nullptr;
  std::shared_ptr<ModbusWrapperFactory> modbus_wrapper_factory = nullptr;
  std::shared_ptr<ModbusWrapperStorage> modbus_wrapper_storage = nullptr;
};

FullModbusWrapperFactoryMock1::FullModbusWrapperFactoryMock1(): _impl(new FullModbusWrapperFactoryMock1Private()) {
  auto modbus_wrapper_storage = std::make_shared<ModbusWrapperStorageImpl1>();
  _impl->base_modbus_wrapper_factory = std::make_shared<ModbusWrapperMockFactory>();
  auto main_modbus_wrapper_factory = std::make_shared<ModbusClientModbusIDDecoratorFactory>(_impl->base_modbus_wrapper_factory, modbus_wrapper_storage);
  _impl->modbus_wrapper_factory = std::make_shared<ModbusAsyncClientWrapperFactory>(main_modbus_wrapper_factory);
}

FullModbusWrapperFactoryMock1::~FullModbusWrapperFactoryMock1() {
  delete _impl;
}

std::shared_ptr<ModbusWrapper> FullModbusWrapperFactoryMock1::createModbusWrapper(const std::string &ip,
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


std::shared_ptr<ModbusWrapper> FullModbusWrapperFactoryMock1::createBaseModbusWrapper(const std::string &ip, int port) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  if (_impl->base_modbus_wrapper_factory != nullptr) {
    result = _impl->base_modbus_wrapper_factory->createBaseModbusWrapper(ip, port);
  }

  return result;
}
