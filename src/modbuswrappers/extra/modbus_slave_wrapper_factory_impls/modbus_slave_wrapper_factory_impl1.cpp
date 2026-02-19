#include "modbuswrappers/extra/modbus_slave_wrapper_factory_impls/modbus_slave_wrapper_factory_impl1.h"

#include "modbuswrappers/modbusasyncclientwrapper/modbusasyncclientwrapper2.h"
#include "modbuswrappers/extra/impls/modbus_slave_wrapper_impl1.h"


std::shared_ptr<ModbusSlaveWrapper> ModbusSlaveWrapperFactoryImpl1::createModbusSlaveWrapper(const std::shared_ptr<
    ModbusWrapper> &modbus_slave,
                                                                                             int start_holding_reg_num,
                                                                                             int holding_regs_count,
                                                                                             int start_input_reg_num,
                                                                                             int input_regs_count,
                                                                                             int period_in_ms) {
  std::shared_ptr<ModbusSlaveWrapper> result = nullptr;

  if (modbus_slave != nullptr) {
    auto async_modbus_wrapper = std::make_shared<ModbusAsyncClientWrapper2>(modbus_slave,
                                                                            start_holding_reg_num,
                                                                            holding_regs_count,
                                                                            start_input_reg_num,
                                                                            input_regs_count);
    result = std::make_shared<ModbusSlaveWrapperImpl1>(async_modbus_wrapper, period_in_ms);
  }

  return result;
}
