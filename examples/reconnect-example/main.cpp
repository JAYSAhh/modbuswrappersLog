#include <cstdlib>
#include <iostream>

#include <utils/services/uidgeneratorimpls/boost/uidgeneratorboostimpl.h>
#include <modbus/factories/impls/modbus_rtu_over_tcp_factory_libmodbus_impl1.h>
#include <modbus/factories/impls/modbus_tcp_factory_libmodbus_impl1.h>

#include "modbuswrappers/extra/modbus_slave_factory_impls/modbus_slave_factory_impl1.h"
#include "modbuswrappers/extra/modbus_slave_wrapper_factory_impls/modbus_slave_wrapper_factory_impl1.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperfactory.h"
#include "modbuswrappers/service/modbus_wrapper_model.h"
#include "modbuswrappers/service/modbus_wrapper_service_impls/modbus_wrapper_service_impl1.h"
#include "modbuswrappers/mocks/modbuswrappermockfactory.h"
#include "modbuswrappers/modbusclientwrapper/modbustcpclientwrapperfactory.h"
#include "modbuswrappers/connectable.h"
#include "modbuswrappers/storages/modbuswrapperstorageimpl1.h"
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbus_client_modbus_id_decorator_factory_impl2.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientdividablewrapperfactory.h"
#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapperfactory.h"

int main(int argc, char *argv[]) {

  std::string ip = "127.0.0.1";
  int port = 4001;
  int modbus_id = 2;
  int timeout_in_ms = 3000;


//  auto modbus_wrapper_factory = std::make_unique<ModbusClientWrapperFactory>(timeout_in_ms);
  bool is_modbus_mock = false;

  auto modbus_wrapper_storage = std::make_shared<ModbusWrapperStorageImpl1>();
  auto base_mock_modbus_wrapper_factory = std::make_shared<ModbusWrapperMockFactory>();
  auto main_mock_modbus_wrapper_factory = std::make_shared<ModbusClientModbusIDDecoratorFactoryImpl2>(base_mock_modbus_wrapper_factory, modbus_wrapper_storage);
  auto dividable_mock_modbus_wrapper_factory = std::make_shared<ModbusClientDividableWrapperFactory>(main_mock_modbus_wrapper_factory,
                                                                                                     0, 120,
                                                                                                     0, 120);
  auto async_mock_modbus_wrapper_factory = std::make_shared<ModbusAsyncClientWrapperFactory>(dividable_mock_modbus_wrapper_factory);

  auto base_modbus_wrapper_factory = std::make_shared<ModbusClientWrapperFactory>(timeout_in_ms);
//  auto priority_modbus_wrapper_factory = std::make_shared<ModbusPriorityWrapperFactory>(base_modbus_wrapper_factory);
  auto main_modbus_wrapper_factory = std::make_shared<ModbusClientModbusIDDecoratorFactoryImpl2>(base_modbus_wrapper_factory, modbus_wrapper_storage);
  auto dividable_modbus_wrapper_factory = std::make_shared<ModbusClientDividableWrapperFactory>(main_modbus_wrapper_factory,
                                                                                                0, 120,
                                                                                                0, 120);
  auto async_modbus_wrapper_factory = std::make_shared<ModbusAsyncClientWrapperFactory>(dividable_modbus_wrapper_factory);


  auto modbus_wrapper = async_modbus_wrapper_factory->createModbusWrapper(ip, port,  modbus_id,
                                                                          0, 58,
                                                                          0, 5);


  modbus_wrapper->connect();
  modbus_wrapper->process();
  while (true) {
    modbus_wrapper->process();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  modbus_wrapper->disconnect();
  return EXIT_SUCCESS;
}