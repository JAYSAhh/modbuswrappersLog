#include <iostream>
#include <chrono>
#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapperfactory.h"
#include "modbuswrappers/storages/modbuswrapperstorageimpl1.h"
#include "modbuswrappers/modbusclientwrapper/modbustcpclientwrapperfactory.h"
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecoratorfactory.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientdividablewrapperfactory.h"

int main() {
  auto modbus_wrapper_storage = std::make_shared<ModbusWrapperStorageImpl1>();
  auto base_modbus_wrapper_factory = std::make_shared<ModbusTcpClientWrapperFactory>(1);

  auto main_modbus_wrapper_factory = std::make_shared<ModbusClientModbusIDDecoratorFactory>(base_modbus_wrapper_factory, modbus_wrapper_storage);
  auto dividable_modbus_wrapper_factory = std::make_shared<ModbusClientDividableWrapperFactory>(main_modbus_wrapper_factory,
                                                                                           0, 30,
                                                                                           0, 30);
  auto async_modbus_wrapper_factory = std::make_shared<ModbusAsyncClientWrapperFactory>(dividable_modbus_wrapper_factory);

  auto modbus_wrapper = async_modbus_wrapper_factory->createModbusWrapper("192.168.127.102", 502, 1, 30, 5);
  if (modbus_wrapper == nullptr) {
    std::cerr << "Error while creating modbus wrapper " << std::endl;
    return EXIT_FAILURE;
  }

  auto connect_response = modbus_wrapper->connect();
  std::cout << "Modbus wrapper first connect response is " << connect_response << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  auto change_address_response = modbus_wrapper->changeAddress("192.168.127.101", 502, 1, false);
  std::cout << "Modbus wrapper change address response is " << change_address_response << std::endl;

  std::cout << "Modbus wrapper ip " << modbus_wrapper->getIP() << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  connect_response = modbus_wrapper->connect();
  std::cout << "Modbus wrapper second connect response is " << connect_response << std::endl;

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  return EXIT_SUCCESS;
}
