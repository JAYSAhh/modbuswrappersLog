#include <iostream>
#include <chrono>

#include "modbuswrappers/logaddition/modbuslogger.h"
#include "modbuswrappers/logaddition/modbuslogwrapperfactory.h"
#include "modbus/modbus.h"
#include "modbuswrappers/modbuswrapper.h"
#include <memory>

#include "modbuswrappers/modbusclientwrapper/modbustcpclientwrapperfactory.h"

#ifdef FOR_WIN
#pragma comment(lib, "ws2_32")
#include <Windows.h>

#endif


int main() {

    std::shared_ptr<ModbusWrapperFactory> factory = std::make_shared<ModbusTcpClientWrapperFactory>(nullptr,nullptr,1);
    auto loggerFactory = ModbusLogWrapperFactory(factory);
    std::string ip = "127.0.0.1";
    auto client = loggerFactory.createModbusWrapper(ip, 5520, "../../../test.log");


    client->connect();
    client->writeHoldingRegister(0,1,1);

    return 0;
}