#include "modbuswrappers/logaddition/modbuslogger.h"
#include "modbuswrappers/logaddition/modbuslogwrapper.h"
#include "modbuswrappers/logaddition/modbuslogwrapperfactory.h"

#include <iostream>

struct  ModbusLogWrapperFactory::ModbusLogWrapperFactoryPrivate
{
  std::shared_ptr<ModbusWrapperFactory> base_modbus_wrapper_factory = nullptr;
};

ModbusLogWrapperFactory::ModbusLogWrapperFactory(std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory): ModbusWrapperFactory(), _impl(new ModbusLogWrapperFactoryPrivate())
{
  _impl->base_modbus_wrapper_factory = base_modbus_wrapper_factory;
}

ModbusLogWrapperFactory::~ModbusLogWrapperFactory()
{
  delete _impl;
}

std::shared_ptr<ModbusWrapper> ModbusLogWrapperFactory::createModbusWrapper(const std::string &ip,
                                                                                 int port,
                                                                                 int modbus_id,
                                                                                 int start_holding_reg_num,
                                                                                 int holding_regs_count,
                                                                                   int start_input_reg_num,
                                                                                 int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  std::cerr << "Cannot create modbus wrapper of MODBUS_LOG_WRAPPER decorator without MODBUS_LOGGER " << std::endl;
  return result;
}
std::shared_ptr<ModbusWrapper> ModbusLogWrapperFactory::createBaseModbusWrapper(const std::string &ip, int port) {
  std::shared_ptr<ModbusWrapper> result = nullptr;
  std::cerr << "Cannot create modbus wrapper of MODBUS_LOG_WRAPPER decorator without MODBUS ID and MODBUS_LOGGER " << std::endl;
  return result;
}

std::shared_ptr<ModbusWrapper> ModbusLogWrapperFactory::createBaseModbusWrapper(const std::string &ip,int port,ModbusLogger &logger)
{

  auto result = std::make_shared<ModbusLogWrapper>(ip,port,logger);
  return result;
}
