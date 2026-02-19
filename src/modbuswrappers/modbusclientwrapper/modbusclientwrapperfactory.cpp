#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperfactory.h"

#include <memory>
#include <mutex>

#include <modbus/factories/impls/modbus_rtu_over_tcp_factory_impl2.h>
#include <modbus/factories/impls/modbus_rtu_over_tcp_factory_libmodbus_impl1.h>
#include <utils/services/retryers/exponential_backoff_retryer_impl1.h>
#include <modbus/factories/impls/modbus_libmodbus_factory_impl1.h>
#include <modbus/factories/impls/modbus_with_retry_factory_impl1.h>
#include <modbus/dynamic_modbus_clients/factories/dynamic_modbus_client_factory_impl1.h>

#include "modbuswrappers/modbusclientwrapper/modbusclientwrapper.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperconnectedstate.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperdisconnectedstate.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrappernotconnectedstate.h"

using modbus::ModbusClient;

struct ModbusClientWrapperFactory::Impl {
  int _retry_count = 3;
  std::chrono::system_clock::duration _min_interval_between_requests = std::chrono::milliseconds(100);
  PackageType _package_type = PackageType::RTU;

  std::shared_ptr<DtLogger> _logger = nullptr;
  std::shared_ptr<Notifier> _notifier = nullptr;

  int _timeout = 1;

  int _initial_delay = 200;
  int _max_delay = 20000;
  double _factor = 2;
  int _jitter = 6000;
};

ModbusClientWrapperFactory::ModbusClientWrapperFactory(const std::shared_ptr<DtLogger> &logger,
                                                       const std::shared_ptr<Notifier> &notifier,
                                                       int retry_count,
                                                       std::chrono::system_clock::duration min_interval_between_requests,
                                                       PackageType package_type,
                                                       int timeout_in_sec):
                                                       _impl(new Impl) {
  _impl->_logger = logger;
  _impl->_notifier = notifier;
  _impl->_retry_count = retry_count;
  _impl->_min_interval_between_requests = min_interval_between_requests;
  _impl->_package_type = package_type;
  _impl->_timeout = timeout_in_sec;
}

ModbusClientWrapperFactory::ModbusClientWrapperFactory(int initial_delay,
                                                       int max_delay,
                                                       int factor,
                                                       int jitter,
                                                       const std::shared_ptr<DtLogger> &logger,
                                                       const std::shared_ptr<Notifier> &notifier,
                                                       int retry_count,
                                                       std::chrono::system_clock::duration min_interval_between_requests,
                                                       PackageType package_type,
                                                       int timeout_in_sec):
                                                       _impl(new Impl) {
  _impl->_logger = logger;
  _impl->_notifier = notifier;

  _impl->_initial_delay = initial_delay;
  _impl->_max_delay = max_delay;
  _impl->_factor = factor;
  _impl->_jitter = jitter;
  _impl->_retry_count = retry_count;
  _impl->_min_interval_between_requests = min_interval_between_requests;
  _impl->_package_type = package_type;
  _impl->_timeout = timeout_in_sec;
}

ModbusClientWrapperFactory::~ModbusClientWrapperFactory() {
  delete _impl;
}


std::shared_ptr<ModbusWrapper> ModbusClientWrapperFactory::createModbusWrapper(const std::string &ip,
                                                                               int port,
                                                                               int modbus_id,
                                                                               int start_holding_reg_num,
                                                                               int holding_regs_count,
                                                                               int start_input_reg_num,
                                                                               int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = createBaseModbusWrapper(ip, port);
  return result;
}

std::shared_ptr<ModbusWrapper> ModbusClientWrapperFactory::createBaseModbusWrapper(const std::string &ip, int port) {
  std::shared_ptr<ModbusWrapper> result = nullptr;

  auto exponential_backoff_retryer = std::make_shared<ExponentialBackoffRetryerImpl1>(_impl->_initial_delay,
                                                                                      _impl->_max_delay,
                                                                                      _impl->_factor,
                                                                                      _impl->_jitter);

  auto base_modbus_client_factory = std::make_shared<ModbusLibmodbusFactoryImpl1>(_impl->_package_type,
                                                                                  _impl->_logger,
                                                                                  _impl->_notifier);
  auto modbus_with_retry_factory = std::make_shared<ModbusWithRetryFactoryImpl1>(base_modbus_client_factory,
                                                                                 _impl->_logger,
                                                                                 _impl->_notifier,
                                                                                 _impl->_retry_count,
                                                                                 _impl->_min_interval_between_requests);
  auto modbus_client_factory = std::make_shared<DynamicModbusClientFactoryImpl1>(modbus_with_retry_factory);
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
