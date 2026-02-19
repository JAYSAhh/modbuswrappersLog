#include "modbuswrappers/modbusclientwrapper/modbustcpclientwrapperfactory.h"

#include <memory>
#include <mutex>

#include <utils/services/retryers/exponential_backoff_retryer_impl1.h>
#include <modbus/dynamic_modbus_clients/factories/dynamic_modbus_client_factory_impl1.h>
#include <modbus/factories/impls/modbus_libmodbus_factory_impl1.h>

#include "modbuswrappers/modbusclientwrapper/modbusclientwrapper.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperconnectedstate.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperdisconnectedstate.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrappernotconnectedstate.h"

using modbus::ModbusClient;

struct ModbusTcpClientWrapperFactory::Impl {
  PackageType _package_type = PackageType::TCP;

  std::shared_ptr<DtLogger> _logger = nullptr;
  std::shared_ptr<Notifier> _notifier = nullptr;

  int _timeout = 1;

  int _initial_delay = 200;
  int _max_delay = 20000;
  double _factor = 2;
  int _jitter = 6000;
};

ModbusTcpClientWrapperFactory::ModbusTcpClientWrapperFactory(const std::shared_ptr<DtLogger> &logger,
                                                             const std::shared_ptr<Notifier> &notifier,
                                                             int timeout_in_sec):
    _impl(new Impl) {
  _impl->_logger = logger;
  _impl->_notifier = notifier;

  _impl->_timeout = timeout_in_sec;;
}

ModbusTcpClientWrapperFactory::~ModbusTcpClientWrapperFactory() {
  delete _impl;
}


std::shared_ptr<ModbusWrapper> ModbusTcpClientWrapperFactory::createModbusWrapper(const std::string &ip,
                                                                                  int port,
                                                                                  int modbus_id,
                                                                                  int start_holding_reg_num,
                                                                                  int holding_regs_count,
                                                                                  int start_input_reg_num,
                                                                                  int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = createBaseModbusWrapper(ip, port);
  return result;
}

std::shared_ptr<ModbusWrapper> ModbusTcpClientWrapperFactory::createBaseModbusWrapper(const std::string &ip, int port) {
  std::shared_ptr<ModbusWrapper> result = nullptr;

  auto exponential_backoff_retryer = std::make_shared<ExponentialBackoffRetryerImpl1>(_impl->_initial_delay,
                                                                                      _impl->_max_delay,
                                                                                      _impl->_factor,
                                                                                      _impl->_jitter);

  auto base_modbus_client_factory = std::make_shared<ModbusLibmodbusFactoryImpl1>(_impl->_package_type,
                                                                                  _impl->_logger,
                                                                                  _impl->_notifier);
  auto modbus_client_factory = std::make_shared<DynamicModbusClientFactoryImpl1>(base_modbus_client_factory);
  auto modbus_client = modbus_client_factory->createModbusClient(ip, port, _impl->_timeout);
//
//  if (modbus_client == nullptr) {
//    return result;
//  }

  auto modbus_wrapper = std::make_shared<ModbusClientWrapper>(ip, port, port);
  auto modbus_wrapper_connected_state = std::make_shared<ModbusClientWrapperConnectedState>(modbus_wrapper, modbus_client,
                                                                                            _impl->_timeout);
  auto modbus_wrapper_disconnected_state = std::make_shared<ModbusClientWrapperDisconnectedState>(modbus_wrapper,
                                                                                                  modbus_client,
                                                                                                  exponential_backoff_retryer);
  auto modbus_wrapper_not_connected_state = std::make_shared<ModbusClientWrapperNotConnectedState>(modbus_wrapper, modbus_client);

  modbus_wrapper->addState({CONNECTED, modbus_wrapper_connected_state});
  modbus_wrapper->addState({DISCONNECTED, modbus_wrapper_disconnected_state});
  modbus_wrapper->addState({NOT_CONNECTED, modbus_wrapper_not_connected_state});
  modbus_wrapper->changeState(NOT_CONNECTED);

  result = modbus_wrapper;

//
//  // TODO: УБРАААААААААААААААть !
//  modbus_client->connect();
//  modbus_wrapper->connect();


//  result = std::make_shared<ModbusClientWrapper>(modbus_client);
  return result;
}