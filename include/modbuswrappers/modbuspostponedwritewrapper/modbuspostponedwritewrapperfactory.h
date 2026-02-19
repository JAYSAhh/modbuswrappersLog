#ifndef MODBUSWRAPPERS_LIBMODBUSWRAPPER_SRC_MODBUSWRAPPERS_MODBUSPOSTPONEDWRITEWRAPPER_MODBUSPOSTPONEDWRITEWRAPPERFACTORY_H_
#define MODBUSWRAPPERS_LIBMODBUSWRAPPER_SRC_MODBUSWRAPPERS_MODBUSPOSTPONEDWRITEWRAPPER_MODBUSPOSTPONEDWRITEWRAPPERFACTORY_H_

#include <memory>

#include "modbuswrappers/modbuswrapperfactory.h"
#include <modbuswrappers/export.h>

class MODBUSWRAPPERS_EXPORT ModbusPostponedWriteWrapperFactory: public ModbusWrapperFactory {
 public:
  explicit ModbusPostponedWriteWrapperFactory(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory);
  ~ModbusPostponedWriteWrapperFactory();

  std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                     int port,
                                                     int modbus_id,
                                                     int start_holding_reg_num,
                                                     int holding_regs_count,
                                                     int start_input_reg_num,
                                                     int input_regs_count) override;
  std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;

 private:
  struct ModbusPostponedWriteWrapperFactoryPrivate;
  ModbusPostponedWriteWrapperFactoryPrivate *_impl = nullptr;
};

#endif //MODBUSWRAPPERS_LIBMODBUSWRAPPER_SRC_MODBUSWRAPPERS_MODBUSPOSTPONEDWRITEWRAPPER_MODBUSPOSTPONEDWRITEWRAPPERFACTORY_H_
