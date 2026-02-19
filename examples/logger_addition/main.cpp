#include <iostream>
#include <chrono>

#include "modbuswrappers/logaddition/modbuslogger.h"
#include "modbus/modbus.h"
#include "modbuswrappers/modbuswrapper.h"

#ifdef FOR_WIN
#pragma comment(lib, "ws2_32")
#include <Windows.h>

#endif


int main() {
    std::string path = "../../../test.log";
    auto loggar = ModbusLogger(path);


    std::cout << "AMBATAKAm.M\n";




    return 0;
}