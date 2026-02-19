#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUSCLIENTMODBUSIDDECORATORFACTORY_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUSCLIENTMODBUSIDDECORATORFACTORY_H_

#include <memory>

#include "modbuswrappers/modbuswrapperfactory.h"
#include <modbuswrappers/export.h>

class ModbusWrapperStorage;

class MODBUSWRAPPERS_EXPORT ModbusClientModbusIDDecoratorFactory: public ModbusWrapperFactory {
 public:
  explicit ModbusClientModbusIDDecoratorFactory(const std::shared_ptr<ModbusWrapperFactory> &modbus_wrapper_factory,
                                                const std::shared_ptr<ModbusWrapperStorage> &modbus_wrapper_storage);
  ~ModbusClientModbusIDDecoratorFactory();

  std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                     int port,
                                                     int modbus_id,
                                                     int start_holding_reg_num,
                                                     int holding_regs_count,
                                                     int start_input_reg_num,
                                                     int input_regs_count) override;

  std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;

 private:
  struct ModbusClientModbusIDDecoratorFactoryPrivate;
  ModbusClientModbusIDDecoratorFactoryPrivate *_impl = nullptr;
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUSCLIENTMODBUSIDDECORATORFACTORY_H_
