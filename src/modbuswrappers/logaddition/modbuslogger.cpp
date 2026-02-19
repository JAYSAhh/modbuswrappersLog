#include "modbuswrappers/logaddition/modbuslogger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <map>

struct ModbusLogger::ModbusLoggerPrivate{
    std::ofstream logstream;

    std::string getErrorCodeLabel(ErrorCode errorCode);
    std::string getOperationLabel(Operation operation);
    std::string getDeviceStateLabel(DeviceState deviceState);

    std::string getCurrentTime();


};

ModbusLogger::ModbusLogger(std::string fileName): _impl(new ModbusLoggerPrivate()){
    _impl->logstream.open(fileName, std::ofstream::out | std::ofstream::app);
    _impl->logstream << "\n\n";
}
ModbusLogger::~ModbusLogger(){
    if (_impl->logstream.is_open()) {
        CloseStream();
    }
}

void ModbusLogger::Log(DeviceState &deviceState, Operation operation,ErrorCode errorCode, std::string message) {
    std::string command = "[" + _impl->getCurrentTime() + "] [" + _impl->getDeviceStateLabel(deviceState) + "] [" + _impl->getOperationLabel(operation) + "] [" + _impl->getErrorCodeLabel(errorCode) + "] " +  message;
    std::cout <<command<< std::endl;
    WriteToStream(command);
}
void ModbusLogger::Log(DeviceState &deviceState, Operation operation,ErrorCode errorCode) {
    std::string command =  "["+ _impl->getCurrentTime() + "] [" + _impl->getDeviceStateLabel(deviceState) + "] [" + _impl->getOperationLabel(operation) + "] [" + _impl->getErrorCodeLabel(errorCode) + "]";
    std::cout <<command<< std::endl;
    WriteToStream(command);
}

void ModbusLogger::WriteToStream(std::string &logline) {
    if (_impl->logstream.is_open()) {
        _impl->logstream << logline<<std::endl;
        _impl->logstream.flush();
    }
}

void ModbusLogger::CloseStream() {
    _impl->logstream.close();
}


std::string ModbusLogger::ModbusLoggerPrivate::getOperationLabel(Operation operation) {
    switch (operation) {
        case GET_IP: return "GET_IP";
        case GET_PORT: return "GET_PORT";
        case GET_MODBUS_ID: return "GET_MODBUS_ID";
        case CONNECT: return "CONNECT";
        case DISCONNECT: return "DISCONNECT";
        case IS_CONNECTED: return "IS_CONNECTED";
        case ADD_SLAVE: return "ADD_SLAVE";
        case REMOVE_SLAVE: return "REMOVE_SLAVE";
        case SET_ENABLED: return "SET_ENABLED";
        case IS_ENABLED: return "IS_ENABLED";
        case SET_IS_OPENED: return "SET_IS_OPENED";
        case IS_OPENED: return "IS_OPENED";
        case READ_HOLDING_REGISTERS: return "READ_HOLDING_REGISTERS";
        case WRITE_HOLDING_REGISTERS: return "WRITE_HOLDING_REGISTERS";
        case CHECK_CONNECTION_STATUS_BY_RESPONSE: return "CHECK_CONNECTION_STATUS_BY_RESPONSE";
        case CHANGE_STATE: return "CHANGE_STATE";
        case CHANGE_ADDRESS: return "CHANGE_ADDRESS";
        case ADD_STATE: return "ADD_STATE";
        case SAVE_TO_EEPROM: return "SAVE_TO_EEPROM";
        case ADD_CONNECTABLE: return "ADD_CONNECTABLE";
        case SEND_CONNECTION_STATUS: return "SEND_CONNECTION_STATUS";
        case PROCESS: return "PROCESS";
        default: return "UNKNOWN_OPERATION";
    }
}

std::string ModbusLogger::ModbusLoggerPrivate::getErrorCodeLabel(ErrorCode errorCode) {
    switch (errorCode) {
        case SUCCESS: return "SUCCESS";
        case MODBUS_ILLEGAL_FUNCTION: return "MODBUS_ILLEGAL_FUNCTION";
        case MODBUS_ILLEGAL_DATA_ADDRESS: return "MODBUS_ILLEGAL_DATA_ADDRESS";
        case MODBUS_ILLEGAL_DATA_VALUE: return "MODBUS_ILLEGAL_DATA_VALUE";
        case MODBUS_SERVER_DEVICE_FAILURE: return "MODBUS_SERVER_DEVICE_FAILURE";
        case MODBUS_ACKNOWLEDGE: return "MODBUS_ACKNOWLEDGE";
        case MODBUS_SERVER_DEVICE_BUSY: return "MODBUS_SERVER_DEVICE_BUSY";
        case MODBUS_NEGATIVE_ACKNOWLEDGE: return "MODBUS_NEGATIVE_ACKNOWLEDGE";
        case MODBUS_MEMORY_PARITY_ERROR: return "MODBUS_MEMORY_PARITY_ERROR";
        case MODBUS_GATEWAY_PATH_UNAVAILABLE: return "MODBUS_GATEWAY_PATH_UNAVAILABLE";
        case MODBUS_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND: return "MODBUS_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND";
        case DEVICE_DOESNT_EXIST: return "DEVICE_DOESNT_EXIST";
        case DEVICE_NOT_RESPONDING_TANGO: return "DEVICE_NOT_RESPONDING_TANGO";
        case DEVICE_TYPE_ERROR: return "DEVICE_TYPE_ERROR";
        case DEVICE_NOT_RESPONDING_LAN: return "DEVICE_NOT_RESPONDING_LAN";
        case DEVICE_NOT_CONNECTED: return "DEVICE_NOT_CONNECTED";
        case INVALID_RESPONSE: return "INVALID_RESPONSE";
        case INVALID_REQUEST: return "INVALID_REQUEST";
        case CRC_ERROR: return "CRC_ERROR";
        case OPERATION_INTERRUPTED: return "OPERATION_INTERRUPTED";
        case DEVICE_DOESNT_HAVE_ATTRIBUTE: return "DEVICE_DOESNT_HAVE_ATTRIBUTE";
        case DEVICE_ALREADY_EXISTS: return "DEVICE_ALREADY_EXISTS";
        case PROPERTY_OR_COMMAND_ALREADY_EXISTS: return "PROPERTY_OR_COMMAND_ALREADY_EXISTS";
        case UNABLE_TO_START_SERVER: return "UNABLE_TO_START_SERVER";
        case UNABLE_TO_STOP_SERVER: return "UNABLE_TO_STOP_SERVER";
        case DEVICE_NOT_INITIALIZED: return "DEVICE_NOT_INITIALIZED";
        case DEVICE_IS_READONLY_MODE: return "DEVICE_IS_READONLY_MODE";
        default: return "UNKNOWN_ERROR_CODE";
        }
}

std::string ModbusLogger::ModbusLoggerPrivate::getDeviceStateLabel(DeviceState deviceState) {
    switch (deviceState) {
        case CONNECTED: return "CONNECTED";
        case NOT_CONNECTED: return "NOT_CONNECTED";
        case DISCONNECTED: return "DISCONNECTED";
        case READ_ONLY: return "READ_ONLY";
        case OPEN: return "OPEN";
        case CLOSE: return "CLOSE";
        case INVALID: return "INVALID";
        case EMPTY: return "EMPTY";
        default: return "UNKNOWN_DEVICE_STATE";
    }

}

std::string ModbusLogger::ModbusLoggerPrivate::getCurrentTime() {
    std::time_t time;
    std::time(&time);
    std::tm* local_time = std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(local_time,"%H:%M:%S");
    return oss.str();
}


