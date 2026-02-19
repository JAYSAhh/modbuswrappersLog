#include <cstdlib>
#include <iostream>

#include <utils/services/uidgeneratorimpls/boost/uidgeneratorboostimpl.h>

#include "modbuswrappers/extra/modbus_slave_factory_impls/modbus_slave_factory_impl1.h"
#include "modbuswrappers/extra/modbus_slave_wrapper_factory_impls/modbus_slave_wrapper_factory_impl1.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientwrapperfactory.h"
#include "modbuswrappers/service/modbus_wrapper_model.h"
#include "modbuswrappers/service/modbus_wrapper_service_impls/modbus_wrapper_service_impl1.h"
#include "modbuswrappers/mocks/modbuswrappermockfactory.h"
#include "modbuswrappers/modbusclientwrapper/modbustcpclientwrapperfactory.h"
#include "modbuswrappers/connectable.h"


void readHoldingRegister(const std::shared_ptr<ModbusWrapperService> &modbus_wrapper_service,
                         int reg_num, int modbus_id) {
  uint16_t value = 0;
  auto error_code = modbus_wrapper_service->readHoldingRegister(reg_num, value, modbus_id);
  std::cout << "Error code and value after read holding registers: " << error_code << " " << value << std::endl;
}

class TestObserver: public Connectable {
 public:
  void setConnectionStatus(bool connection_status) override {
    std::cout << "CONNECTION STATUS CHANGED " << connection_status << std::endl;
  }

  bool getConnectionStatus() override {
    return false;
  }
  ErrorCode connect() override {
    return DEVICE_NOT_RESPONDING_LAN;
  }
  ErrorCode disconnect() override {
    return DEVICE_NOT_RESPONDING_LAN;
  }
};

int main(int argc, char *argv[]) {
  const std::string ip = "127.0.0.1";
  const int port = 4001;

  const int initial_delay = 200;
  const int max_delay = 20000;
  const int factor = 2;
  const int jitter = 6000;

  auto uid_generator = std::make_shared<UIDGeneratorBoostImpl>();
  auto modbus_slave_factory = std::make_shared<ModbusSlaveFactoryImpl1>(initial_delay,
                                                                        max_delay,
                                                                        factor,
                                                                        jitter);
  auto modbus_slave_wrapper_factory = std::make_shared<ModbusSlaveWrapperFactoryImpl1>();

  auto base_modbus_wrapper_factory = std::make_shared<ModbusClientWrapperFactory>();
//  auto base_modbus_wrapper_factory = std::make_shared<ModbusTcpClientWrapperFactory>();
//  auto base_modbus_wrapper_factory = std::make_shared<ModbusWrapperMockFactory>();
  auto base_modbus_wrapper = base_modbus_wrapper_factory->createBaseModbusWrapper(ip, port);


  auto model = std::make_shared<ModbusWrapperModel>(uid_generator,
                                                    base_modbus_wrapper,
                                                    modbus_slave_factory,
                                                    modbus_slave_wrapper_factory);

  auto modbus_wrapper_service = std::make_shared<ModbusWrapperServiceImpl1>(model);

  auto test_observer = new TestObserver();
//  modbus_wrapper_service->addModbusSlave(1);

//  modbus_wrapper_service->setTCPConnectionData("10.10.110.202", 4001);

//  modbus_wrapper_service->addModbusSlaveWrapper(255, 0, 2, 32768, 32768, 60000);
//  modbus_wrapper_service->addModbusSlaveWrapper(255, 0, 1, 0, 15, 1000);
//  modbus_wrapper_service->addModbusSlaveWrapper(1, 0, 30, 0, 5, 500);
//  modbus_wrapper_service->addModbusSlaveWrapper(1, 0, 0, 30001, 500, 500);
  modbus_wrapper_service->addModbusSlaveWrapper(2, 0, 58, 0, 5, 500);
//  modbus_wrapper_service->addModbusSlaveWrapper(3, 0, 58, 0, 5, 500);
//  modbus_wrapper_service->addModbusSlaveWrapper(4, 0, 83, 0, 5, 500);



//  for (int i = 2; i < 250; i++) {
//    modbus_wrapper_service->addModbusSlaveWrapper(i, 0, 30, 0, 5, 1000);
//  }
//  modbus_wrapper_service->addModbusSlaveWrapper(2, 0, 30, 0, 5, 1000);
//  modbus_wrapper_service->addModbusSlaveWrapper(3, 0, 30, 0, 5, 1000);
//  modbus_wrapper_service->addModbusSlaveWrapper(4, 0, 30, 0, 5, 1000);
//  modbus_wrapper_service->addModbusSlaveWrapper(5, 0, 30, 0, 5, 1000);
//  modbus_wrapper_service->addModbusSlaveWrapper(6, 0, 30, 0, 5, 1000);
//  modbus_wrapper_service->addModbusSlaveWrapper(7, 0, 30, 0, 5, 1000);
//  modbus_wrapper_service->addModbusSlaveWrapper(2, 10, 30, 0, 5, 5000);
//  modbus_wrapper_service->addModbusSlaveWrapper(3, 10, 30, 0, 5, 5000);
//  modbus_wrapper_service->addModbusSlaveWrapper(4, 10, 30, 0, 5, 5000);
//  modbus_wrapper_service->addModbusSlaveWrapper(5, 10, 30, 0, 5, 5000);
//  modbus_wrapper_service->addModbusSlave(2);
//  modbus_wrapper_service->addModbusSlaveWrapper(1, 10000, 30000, 0, 0, 60000);
//  modbus_wrapper_service->addModbusSlaveWrapper(2, 10000, 30000, 0, 0, 60000);
//  modbus_wrapper_service->addModbusSlaveWrapper(3, 0, 2, 0, 0, 1000);

  modbus_wrapper_service->addConnectable(1, test_observer);

  modbus_wrapper_service->connectTCP();
  modbus_wrapper_service->connect(1);
  modbus_wrapper_service->connect(2);

//  modbus_wrapper_service->connect(2);
//  modbus_wrapper_service->connect(3);
//  modbus_wrapper_service->connect(4);
//  modbus_wrapper_service->connect(1);
//  for (int i = 2; i < 250; i++) {
//    modbus_wrapper_service->connect(i);
//  }
//  modbus_wrapper_service->connect(4);
//  modbus_wrapper_service->connect(5);

  modbus_wrapper_service->process();

//  readHoldingRegister(modbus_wrapper_service, 3, 1);
//  readHoldingRegister(modbus_wrapper_service, 30, 1);
//  readHoldingRegister(modbus_wrapper_service, 2, 1);
//  readHoldingRegister(modbus_wrapper_service, 70, 1);
//  readHoldingRegister(modbus_wrapper_service, 20000, 2);

//  auto modbus_slave_ids = modbus_wrapper_service->getModbusSlaves();
//  for (auto modbus_slave_id: modbus_slave_ids) {
//    std::cout << "Found MODBUS SLAVE with ID: " << modbus_slave_id << std::endl;
//    auto modbus_slave_wrapper_ids = modbus_wrapper_service->getModbusSlaveWrappers(modbus_slave_id);
//    for (const auto &modbus_slave_wrapper_id: modbus_slave_wrapper_ids) {
//      std::cout << "\t\t" << "Found MODBUS SLAVE WRAPPER with UID: " << modbus_slave_wrapper_id << std::endl;
//    }
//  }

  std::cout << modbus_wrapper_service->getInfo() << std::endl;

//  modbus_wrapper_service->setTCPConnectionData("10.10.110.22", 4001);

  std::cout << modbus_wrapper_service->getInfo() << std::endl;


  for (int i = 0; i < 1000; i++) {
    modbus_wrapper_service->process();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (i == 50) {
      modbus_wrapper_service->disconnect(1);
//      for (int i = 2; i < 250; i++) {
//        modbus_wrapper_service->disconnect(i);
//      }

      std::cout << "\n\n\n\n\t\t\t\tALL DISCONNECTED \n\n\n\n\n\n";
    } else if (i == 60) {
      modbus_wrapper_service->connect(1);
    }
  }

  return EXIT_SUCCESS;
}