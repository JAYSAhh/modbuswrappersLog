#include "modbuswrappers/modbusasyncclientwrapper/modbuswrapperpoll.h"

#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapper.h"

struct ModbusWrapperPoll::ModbusWrapperPollPrivate{
  std::shared_ptr<ModbusAsyncClientWrapper> modbus_async_client_wrapper = nullptr;
};

ModbusWrapperPoll::ModbusWrapperPoll(const std::shared_ptr<ModbusAsyncClientWrapper> &modbus_client_wrapper):
                                     _impl(new ModbusWrapperPollPrivate()){
  _impl->modbus_async_client_wrapper = modbus_client_wrapper;
}

ModbusWrapperPoll::~ModbusWrapperPoll() {
  delete _impl;
}

void ModbusWrapperPoll::process() {
  if (_impl->modbus_async_client_wrapper != nullptr) {
    _impl->modbus_async_client_wrapper->process();
  }
}
