#include <modbuswrappers/modbusclientwrapper/modbusclientwrapperfactory.h>
#include <utils/modbusutils.h>
#include "modbuswrappers/storages/modbuswrapperstorageimpl1.h"
#include "modbuswrappers/mocks/modbuswrappermockfactory.h"
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbusclientmodbusiddecoratorfactory.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientdividablewrapperfactory.h"
#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapperfactory.h"
#include "modbuswrappers/modbuswrappermodbusiddecorator/modbus_client_modbus_id_decorator_factory_impl2.h"

int main(int argc, char *argv[]) {
  const std::string ip = "127.0.0.1";
  const int port = 4001;
  const int modbus_id = 2;
  const int timeout_in_sec = 3;

  auto modbus_wrapper_factory = std::make_unique<ModbusClientWrapperFactory>(timeout_in_sec);
  bool is_modbus_mock = false;

  auto modbus_wrapper_storage = std::make_shared<ModbusWrapperStorageImpl1>();
  auto base_mock_modbus_wrapper_factory = std::make_shared<ModbusWrapperMockFactory>();
  auto main_mock_modbus_wrapper_factory = std::make_shared<ModbusClientModbusIDDecoratorFactoryImpl2>(base_mock_modbus_wrapper_factory, modbus_wrapper_storage);
  auto dividable_mock_modbus_wrapper_factory = std::make_shared<ModbusClientDividableWrapperFactory>(main_mock_modbus_wrapper_factory,
                                                                                                     0, 30,
                                                                                                     0, 30);
  auto async_mock_modbus_wrapper_factory = std::make_shared<ModbusAsyncClientWrapperFactory>(dividable_mock_modbus_wrapper_factory);

  auto base_modbus_wrapper_factory = std::make_shared<ModbusClientWrapperFactory>(timeout_in_sec);
//  auto priority_modbus_wrapper_factory = std::make_shared<ModbusPriorityWrapperFactory>(base_modbus_wrapper_factory);
  auto main_modbus_wrapper_factory = std::make_shared<ModbusClientModbusIDDecoratorFactoryImpl2>(base_modbus_wrapper_factory, modbus_wrapper_storage);
  auto dividable_modbus_wrapper_factory = std::make_shared<ModbusClientDividableWrapperFactory>(main_modbus_wrapper_factory,
                                                                                                0, 30,
                                                                                                0, 30);
  auto async_modbus_wrapper_factory = std::make_shared<ModbusAsyncClientWrapperFactory>(dividable_modbus_wrapper_factory);



  auto modbus_wrapper = async_modbus_wrapper_factory->createModbusWrapper(ip, port,  modbus_id,
                                                                          0, 58,
                                                                          0, 5);
  if (modbus_wrapper != nullptr) {
    modbus_wrapper->connect();
    modbus_wrapper->process();

    std::vector<uint16_t> reg_values(2);
    modbus::toMsbLsb(0x100, reg_values[0], reg_values[1]);
    modbus_wrapper->writeHoldingRegisters(0, reg_values, modbus_id);


    modbus_wrapper->process();

    modbus_wrapper->readHoldingRegisters(0, 2, reg_values, modbus_id);


    uint32_t value = 0;
    modbus::fromMsbLsb(reg_values[0], reg_values[1], value);

    std::cout << "VALUE: " << value << std::endl;
  }

  return 0;
}