#ifndef MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_WRAPPER_FACTORY_IMPLS_MODBUS_SLAVE_WRAPPER_FACTORY_IMPL1_H_
#define MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_WRAPPER_FACTORY_IMPLS_MODBUS_SLAVE_WRAPPER_FACTORY_IMPL1_H_

#include <memory>

#include "modbuswrappers/extra/modbus_slave_wrapper_factory.h"


class MODBUSWRAPPERS_EXPORT ModbusSlaveWrapperFactoryImpl1: public ModbusSlaveWrapperFactory {
 public:
  std::shared_ptr<ModbusSlaveWrapper> createModbusSlaveWrapper(const std::shared_ptr<ModbusWrapper> &modbus_slave,
                                                               int start_holding_reg_num,
                                                               int holding_regs_count,
                                                               int start_input_reg_num,
                                                               int input_regs_count,
                                                               int period_in_ms) override;

};

#endif //MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_WRAPPER_FACTORY_IMPLS_MODBUS_SLAVE_WRAPPER_FACTORY_IMPL1_H_
