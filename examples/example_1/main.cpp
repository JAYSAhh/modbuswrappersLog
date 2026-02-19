#include <iostream>
#include <chrono>

#include "modbus/modbus.h"
#include "modbus/factories/impls/modbus_tcp_factory_impl_1.h"
#include "modbus/factories/impls/modbus_rtu_over_tcp_factory_impl2.h"
#include <modbuswrappers/modbusclientwrapper/modbusclientwrapperfactory.h>

#ifdef FOR_WIN
#pragma comment(lib, "ws2_32")
#include <Windows.h>
#endif


using namespace modbus;

int main() {
#ifdef FOR_WIN
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
#endif


  auto modbus_factory = std::make_shared<ModbusClientWrapperFactory>();
  auto client = modbus_factory->createBaseModbusWrapper("10.10.110.202", 4001);
  
  if (client == nullptr) {
    std::cerr << "Error while creating modbus client TCP " << std::endl;
    return EXIT_FAILURE;
  }
  
//
////  ModbusClient client("10.10.110.34", 4001);
  auto error_code = client->connect();
  bool is_connected = false;
  client->isConnected(is_connected);
  if (!is_connected) {
    std::cout << "Connection failure" << std::endl;
    return EXIT_FAILURE;
  }
//
//  std::this_thread::sleep_for(std::chrono::seconds(2));
//  client->disconnect();
//  std::this_thread::sleep_for(std::chrono::seconds(2));
//  is_connected = client->connect();
//  if (!is_connected) {
//    std::cerr << "Connection failuer" << std::endl;
//    return EXIT_FAILURE;
//  }
//
//  std::this_thread::sleep_for(std::chrono::seconds(2));
//
//  is_connected = client->connect();
//  if (!is_connected) {
//    std::cerr << "Connection failuer" << std::endl;
//    return EXIT_FAILURE;
//  }
//
//
//  std::this_thread::sleep_for(std::chrono::seconds(2));
//
//  is_connected = client->connect();
//  if (!is_connected) {
//    std::cerr << "Connection failuer" << std::endl;
//    return EXIT_FAILURE;
//  }
//
//  std::this_thread::sleep_for(std::chrono::seconds(2));

//  sleep(7);

  uint16_t value;
  auto result = client->readHoldingRegister(0, value);
  std::vector<uint16_t> values(4);
  for (int i = 0; i < 10; i++) {
    std::cout << "\n";
    result = client->readHoldingRegister(1, value);
    std::cout << i << " " << 0 << result << std::endl;
    result = client->readHoldingRegister(5, value);
    std::cout << i << " " << 1 << std::endl;
    result = client->readInputRegister(5, value);
    std::cout << i << " " << 2 << std::endl;
    result = client->readInputRegister(1, value);
    std::cout << i << " " << 3 << std::endl;

    // Возможно падает на множественном чтении
    result = client->readInputRegisters(1, 4, values);
    std::cout << i << " " << 4 << std::endl;
//    result = client->readHoldingRegisters(1, 5, values);
//    std::cout << i << " " << 5 << std::endl;
  }

  std::cout << "RESULT CODE IS " << result << std::endl;

  return EXIT_SUCCESS;
}
