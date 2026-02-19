#ifndef MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_MODBUSPRIORITYWRAPPER_MODBUSPRIORITYWRAPPERFACTORY_H_
#define MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_MODBUSPRIORITYWRAPPER_MODBUSPRIORITYWRAPPERFACTORY_H_

#include <memory>

#include "modbuswrappers/modbuswrapperfactory.h"

class MODBUSWRAPPERS_EXPORT ModbusPriorityWrapperFactory: public ModbusWrapperFactory {
 public:
  explicit ModbusPriorityWrapperFactory(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory);
  ~ModbusPriorityWrapperFactory();

  std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                     int port,
                                                     int modbus_id,
                                                     int start_holding_reg_num,
                                                     int holding_regs_count,
                                                     int start_input_reg_num,
                                                     int input_regs_count) override;
  std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;

 private:
  struct ModbusPriorityWrapperFactoryPrivate;
  ModbusPriorityWrapperFactoryPrivate *_impl = nullptr;
};

#endif //MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_MODBUSPRIORITYWRAPPER_MODBUSPRIORITYWRAPPERFACTORY_H_
