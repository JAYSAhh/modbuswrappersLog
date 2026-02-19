#include "modbuswrappers/mocks/modbuswrappermockfactory.h"
//#include "modbuswrappers/mocks/modbuswrappermock1.h"
#include "modbuswrappers/mocks/modbuswrappermock2.h"

std::shared_ptr<ModbusWrapper> ModbusWrapperMockFactory::createModbusWrapper(const std::string &ip,
                                                                             int port,
                                                                             int modbus_id,
                                                                             int start_holding_reg_num,
                                                                             int holding_regs_count,
                                                                             int start_input_reg_num,
                                                                             int input_regs_count) {
  std::shared_ptr<ModbusWrapper> result = nullptr;

  result = std::make_shared<ModbusWrapperMock2>(ip, port, modbus_id);
  return result;
}
std::shared_ptr<ModbusWrapper> ModbusWrapperMockFactory::createBaseModbusWrapper(const std::string &ip, int port) {
  std::shared_ptr<ModbusWrapper> result = nullptr;

  result = std::make_shared<ModbusWrapperMock2>(ip, port, 1);
  return result;
}
