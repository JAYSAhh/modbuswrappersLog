#include <utils/services/retryers/exponential_backoff_retryer_impl1.h>
#include <utils/services/dividers/impls/reg_read_divider_impl1.h>
#include "modbuswrappers/extra/modbus_slave_factory_impls/modbus_slave_factory_impl1.h"

#include "modbuswrappers/modbuswrappermodbusiddecorator/modbus_client_modbus_id_decorator_impl2.h"
#include "modbuswrappers/modbusclientwrapper/modbusclientdividablewrapper.h"

ModbusSlaveFactoryImpl1::ModbusSlaveFactoryImpl1(int initial_delay,
                                                 int max_delay,
                                                 int factor,
                                                 int jitter):
                                                 _initial_delay(initial_delay),
                                                 _max_delay(max_delay),
                                                 _factor(factor),
                                                 _jitter(jitter) {

}

std::shared_ptr<ModbusWrapper> ModbusSlaveFactoryImpl1::createModbusSlave(const std::shared_ptr<ModbusWrapper> &base_modbus_wrapper,
                                                                          int modbus_id) {

  std::shared_ptr<ModbusWrapper> result = nullptr;

  if (base_modbus_wrapper != nullptr && modbus_id > 0 && modbus_id <= 255) {
    auto exponential_backoff_retryer = std::make_shared<ExponentialBackoffRetryerImpl1>(_initial_delay,
                                                                                        _max_delay,
                                                                                        _factor,
                                                                                        _jitter);


    auto base_modbus_client_wrapper = std::dynamic_pointer_cast<ModbusClientWrapper>(base_modbus_wrapper);
    if (base_modbus_client_wrapper != nullptr) {
      auto modbus_id_wrapper = std::make_shared<ModbusClientModbusIDDecoratorImpl2>(base_modbus_client_wrapper, modbus_id,
                                                                                    exponential_backoff_retryer);

      auto input_regs_divider = std::make_shared<RegReadDividerImpl1>(30);
      auto holding_regs_divider = std::make_shared<RegReadDividerImpl1>(30);

      result = std::make_shared<ModbusClientDividableWrapper>(modbus_id_wrapper, holding_regs_divider, input_regs_divider);
    }
  }

  return result;
}
