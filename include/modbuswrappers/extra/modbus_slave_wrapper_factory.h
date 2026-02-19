#ifndef MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_WRAPPER_FACTORY_H_
#define MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_WRAPPER_FACTORY_H_

#include <memory>

#include "modbuswrappers/modbuswrapper.h"
#include "modbus_slave_wrapper.h"

class MODBUSWRAPPERS_EXPORT ModbusSlaveWrapperFactory {
 public:
  virtual std::shared_ptr<ModbusSlaveWrapper> createModbusSlaveWrapper(const std::shared_ptr<ModbusWrapper> &modbus_slave,
                                                                       int start_holding_reg_num,
                                                                       int holding_regs_count,
                                                                       int start_input_reg_num,
                                                                       int input_regs_count,
                                                                       int period_in_ms) = 0;
};

#endif //MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_WRAPPER_FACTORY_H_
