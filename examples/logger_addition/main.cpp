#include <iostream>
#include <chrono>

#include "modbuswrappers/logaddition/modbuslogger.h"
#include "modbuswrappers/logaddition/modbuslogwrapperfactory.h"
#include "modbus/modbus.h"
#include "modbuswrappers/modbuswrapper.h"
#include <memory>

#ifdef FOR_WIN
#pragma comment(lib, "ws2_32")
#include <Windows.h>

#endif


int main() {
    std::string path = "../../../test.log";
    auto logger = ModbusLogger(path);
    auto base_factory = std::make_shared<ModbusWrapperFactory>();
    auto factory = ModbusLogWrapperFactory(base_factory);
    auto client = factory.createBaseModbusWrapper("10.10.110.202", 4001, logger);





    return 0;
}