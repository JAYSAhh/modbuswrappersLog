#include "modbuswrappers/modbusclientwrapper/modbusclientdividablewrapperfactory.h"

#include <memory>
#include <mutex>
#include <utils/services/dividers/impls/reg_read_divider_impl1.h>

#include "modbuswrappers/modbusclientwrapper/modbusclientwrapper.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperconnectedstate.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperdisconnectedstate.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientdividablewrapper.h"

using modbus::ModbusClient;


static const int DEFAULT_HOLDING_START_REG = 0;
static const int DEFAULT_HOLDING_REGS_PER_REQUEST = 30;

static const int DEFAULT_INPUT_START_REG = 0;
static const int DEFAULT_INPUT_REGS_PER_REQUEST = 30;

struct ModbusClientDividableWrapperFactory::ModbusClientDividableWrapperFactoryPrivate{
  int holding_start_reg = 0;
  int holding_regs_per_request = 30;
  int input_start_reg = 0;
  int input_regs_per_request = 30;
  std::shared_ptr<ModbusWrapperFactory> modbus_wrapper_factory = nullptr;
};

ModbusClientDividableWrapperFactory::ModbusClientDividableWrapperFactory(const std::shared_ptr<ModbusWrapperFactory> &modbus_wrapper_factory):
                                                                         _impl(new ModbusClientDividableWrapperFactoryPrivate()){
  _impl->modbus_wrapper_factory = modbus_wrapper_factory;
  _impl->holding_start_reg = DEFAULT_HOLDING_START_REG;
  _impl->holding_regs_per_request = DEFAULT_HOLDING_REGS_PER_REQUEST;
  _impl->input_start_reg = DEFAULT_INPUT_START_REG;
  _impl->input_regs_per_request = DEFAULT_INPUT_REGS_PER_REQUEST;
}

ModbusClientDividableWrapperFactory::ModbusClientDividableWrapperFactory(const std::shared_ptr<ModbusWrapperFactory> &modbus_wrapper_factory,
                                                                         int holding_start_reg,
                                                                         int holding_regs_per_request,
                                                                         int input_start_reg,
                                                                         int input_regs_per_request):
                                                                         _impl(new ModbusClientDividableWrapperFactoryPrivate()){
  _impl->modbus_wrapper_factory = modbus_wrapper_factory;
  _impl->holding_start_reg = holding_start_reg;
  _impl->holding_regs_per_request = holding_regs_per_request;
  _impl->input_start_reg = input_start_reg;
  _impl->input_regs_per_request = input_regs_per_request;
}

ModbusClientDividableWrapperFactory::~ModbusClientDividableWrapperFactory() {
  delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusClientDividableWrapperFactory::createModbusWrapper(const std::string &ip,
                                                                                        int port,
                                                                                        int modbus_id,
                                                                                        int start_holding_reg_num,
                                                                                        int holding_regs_count,
                                                                                        int start_input_reg_num,
                                                                                        int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = nullptr;

  auto holding_reg_req_divider = std::make_shared<RegReadDividerImpl1>(_impl->holding_regs_per_request);
  auto input_reg_req_divider = std::make_shared<RegReadDividerImpl1>(_impl->input_regs_per_request);

  if (_impl->modbus_wrapper_factory != nullptr) {
    auto base_modbus_wrapper = _impl->modbus_wrapper_factory->createModbusWrapper(ip,
                                                                            port,
                                                                            modbus_id,
                                                                            start_holding_reg_num,
                                                                            holding_regs_count,
                                                                            start_input_reg_num,
                                                                            input_regs_count);
    if (base_modbus_wrapper != nullptr) {
      result = std::make_shared<ModbusClientDividableWrapper>(base_modbus_wrapper,
                                                              holding_reg_req_divider,
                                                              input_reg_req_divider);
    }
  }

  return result;
}

std::shared_ptr<ModbusWrapper> ModbusClientDividableWrapperFactory::createBaseModbusWrapper(const std::string &ip, int port) {
  std::cerr << "MODBUS WRAPPER BASE BE CREATED VIA THIS FACTORY " << __func__ << std::endl;
  return nullptr;
}
