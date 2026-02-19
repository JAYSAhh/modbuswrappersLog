#include "modbuswrappers/service/modbus_wrapper_service.h"

#include <iostream>
#include <sstream>

std::string ModbusWrapperService::getInfo() {
  std::string result = "";

  std::stringstream ss;


  auto tcp_connection_data = getTCPConnectionData();
  auto ip = std::get<1>(tcp_connection_data);
  auto port = std::get<2>(tcp_connection_data);
  ss << "IP: " << ip << "; " << "Port: " << port << std::endl;
  auto modbus_slave_ids = getModbusSlaves();
  for (auto modbus_slave_id: modbus_slave_ids) {
    ss << "Found MODBUS SLAVE with ID: " << modbus_slave_id << std::endl;
    auto modbus_slave_wrapper_ids = getModbusSlaveWrappers(modbus_slave_id);
    for (const auto &modbus_slave_wrapper_uid: modbus_slave_wrapper_ids) {
      ss << "\t\t" << "Found MODBUS SLAVE WRAPPER with UID: " << modbus_slave_wrapper_uid << std::endl;

      auto modbus_slave_wrapper_info_prefix = "\t\t\t\t";

      auto modbus_slave_wrapper_holding_regs_range =
          getModbusSlaveWrapperHoldingRegsRange(modbus_slave_wrapper_uid);
      auto modbus_slave_wrapper_input_regs_range =
          getModbusSlaveWrapperInputRegsRange(modbus_slave_wrapper_uid);
      auto modbus_slave_wrapper_min_update_period_in_ms =
          getModbusSlaveWrapperMinUpdatePeriodInMs(modbus_slave_wrapper_uid);

      ss << modbus_slave_wrapper_info_prefix
         << "Holding Regs Range: ["
         << std::get<0>(modbus_slave_wrapper_holding_regs_range) << "; "
         << std::get<1>(modbus_slave_wrapper_holding_regs_range) << "]" << std::endl;
      ss << modbus_slave_wrapper_info_prefix
         << "Input Regs Range: ["
         << std::get<0>(modbus_slave_wrapper_input_regs_range) << "; "
         << std::get<1>(modbus_slave_wrapper_input_regs_range) << "]" << std::endl;
      ss << modbus_slave_wrapper_info_prefix
         << "Min Update Period in MS:"
         << modbus_slave_wrapper_min_update_period_in_ms << std::endl;
    }
  }

  result = ss.str();

  return result;
}
