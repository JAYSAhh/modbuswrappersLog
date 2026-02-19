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
#include "singlewritemodbuswrapper.h"

void cpsScan(std::shared_ptr<ModbusWrapper> modbus_wrapper) {
//  modbus_wrapper->connect();
  while (true) {
    modbus_wrapper->process();
    uint16_t reg_value;
    modbus_wrapper->readHoldingRegister(28, reg_value, 1);

    auto to_enable = reg_value != 0;
    for (int i = 2; i < 20; i++) {
      modbus_wrapper->setEnabled(to_enable, i);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

void writeConstanlty(std::shared_ptr<ModbusWrapper> modbus_wrapper, int modbus_id) {
//  modbus_wrapper->connect();
  while (true) {
    modbus_wrapper->writeHoldingRegisters(0, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, modbus_id);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main(int argc, char *argv[]) {

  std::string ip = "127.0.0.1";
  int port = 4001;
  int modbus_id = 1;
  int timeout_in_ms = 3000;


//  auto modbus_wrapper_factory = std::make_unique<ModbusClientWrapperFactory>(timeout_in_ms);
  bool is_modbus_mock = false;

  auto modbus_wrapper_storage = std::make_shared<ModbusWrapperStorageImpl1>();
  auto base_mock_modbus_wrapper_factory = std::make_shared<ModbusWrapperMockFactory>();
  auto main_mock_modbus_wrapper_factory = std::make_shared<ModbusClientModbusIDDecoratorFactoryImpl2>(base_mock_modbus_wrapper_factory, modbus_wrapper_storage);
//  auto dividable_mock_modbus_wrapper_factory = std::make_shared<ModbusClientDividableWrapperFactory>(main_mock_modbus_wrapper_factory,
//                                                                                                     0, 120,
//                                                                                                     0, 120);
  auto async_mock_modbus_wrapper_factory = std::make_shared<ModbusAsyncClientWrapperFactory>(main_mock_modbus_wrapper_factory);

  auto base_modbus_wrapper_factory = std::make_shared<ModbusClientWrapperFactory>(nullptr, nullptr, 3, std::chrono::milliseconds(100),
                                                                                  modbus::ModbusClient::RTU,
                                                                                  timeout_in_ms);
//  auto priority_modbus_wrapper_factory = std::make_shared<ModbusPriorityWrapperFactory>(base_modbus_wrapper_factory);
  auto main_modbus_wrapper_factory = std::make_shared<ModbusClientModbusIDDecoratorFactoryImpl2>(base_modbus_wrapper_factory, modbus_wrapper_storage);
  auto dividable_modbus_wrapper_factory = std::make_shared<ModbusClientDividableWrapperFactory>(main_modbus_wrapper_factory,
                                                                                                0, 120,
                                                                                                0, 120);
  auto async_modbus_wrapper_factory = std::make_shared<ModbusAsyncClientWrapperFactory>(dividable_modbus_wrapper_factory);

  auto base_modbus_wrapper = base_modbus_wrapper_factory->createBaseModbusWrapper(ip, port);

  auto modbus_wrapper = async_modbus_wrapper_factory->createModbusWrapper(ip, port,  modbus_id,
                                                                          0, 58,
                                                                          0, 5);

  auto modbus_wrapper_2 = async_modbus_wrapper_factory->createModbusWrapper(ip, port,  2,
                                                                          0, 58,
                                                                          0, 5);
  auto sw_modbus_wrapper_2 = std::make_shared<SingleWriteModbusWrapper>(modbus_wrapper_2);


  sw_modbus_wrapper_2->connect();
  modbus_wrapper->connect();

//  modbus_wrapper_2->setEnabled(false, 1);
//  for (int i = 2; i < 20; i++) {
//    modbus_wrapper_2->setEnabled(true, i);
//  }

  uint16_t value;

  auto thread_1 = std::thread(&writeConstanlty, sw_modbus_wrapper_2, 2);
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  auto cps_thread = std::thread(&cpsScan, modbus_wrapper);
//
//  for (int i = 2; i < 4; i++) {
//    std::thread(&writeConstanlty, modbus_wrapper, i);
//  }


  cps_thread.join();
  thread_1.join();

//  modbus_wrapper->setEnabled(false, 2);
//  modbus_wrapper_2->process();
//  modbus_wrapper->setEnabled(true, 2);
//  modbus_wrapper_2->process();
//  modbus_wrapper_2->process();
//  modbus_wrapper_2->process();
//  modbus_wrapper_2->process();
//  modbus_wrapper->process();
//  modbus_wrapper_2->process();
//  modbus_wrapper->disconnect();
//  modbus_wrapper_2->disconnect();
//  modbus_wrapper_2->disconnect();
//  modbus_wrapper->setEnabled(true, 2);
//  modbus_wrapper->setEnabled(true, 2);
//  modbus_wrapper_2->process();
//  modbus_wrapper->disconnect();
//  for (int i = 1; i < 20; i++) {
//    modbus_wrapper_2->setEnabled(false, i);
//  }

//  modbus_wrapper_2->disconnect();
//  while (true) {
//    modbus_wrapper->process();
//    std::this_thread::sleep_for(std::chrono::milliseconds(500));
//  }
  return EXIT_SUCCESS;
}