#ifndef MODBUSWRAPPERS_MODBUSLOGGER_H
#define MODBUSWRAPPERS_MODBUSLOGGER_H
#include <fstream>
#include "utils/baseutils.h"
#include "modbuswrappers/export.h"

enum Operation {
    GET_IP,
    GET_PORT,
    GET_MODBUS_ID,
    CONNECT,
    DISCONNECT,
    IS_CONNECTED,
    ADD_SLAVE,
    REMOVE_SLAVE,
    SET_ENABLED,
    IS_ENABLED,
    SET_IS_OPENED,
    IS_OPENED,
    READ_HOLDING_REGISTERS,
    WRITE_HOLDING_REGISTERS,
    CHECK_CONNECTION_STATUS_BY_RESPONSE,
    CHANGE_STATE,
    CHANGE_ADDRESS,
    ADD_STATE,
    SAVE_TO_EEPROM,
    ADD_CONNECTABLE,
    SEND_CONNECTION_STATUS,
    PROCESS
};








class MODBUSWRAPPERS_EXPORT ModbusLogger {
    public:
        explicit ModbusLogger(std::string fileName);
        ~ModbusLogger();
        void Log(DeviceState &deviceState, Operation operation,ErrorCode errorCode, std::string message);
        void Log(DeviceState &deviceState, Operation operation,ErrorCode errorCode);
        void WriteToStream(std::string &logline);
        void CloseStream();
    private:
        struct ModbusLoggerPrivate;
        ModbusLoggerPrivate *_impl;

};




#endif //MODBUSWRAPPERS_MODBUSLOGGER_H