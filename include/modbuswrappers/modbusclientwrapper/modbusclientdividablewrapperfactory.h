#ifndef MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTDIVIDABLEWRAPPERFACTORY_H_
#define MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTDIVIDABLEWRAPPERFACTORY_H_
#include <memory>
#include "modbuswrappers/modbuswrapperfactory.h"
#include <modbuswrappers/export.h>

class MODBUSWRAPPERS_EXPORT ModbusClientDividableWrapperFactory: public ModbusWrapperFactory {
 public:
  explicit ModbusClientDividableWrapperFactory(const std::shared_ptr<ModbusWrapperFactory> &modbus_wrapper_factory);
  explicit ModbusClientDividableWrapperFactory(const std::shared_ptr<ModbusWrapperFactory> &modbus_wrapper_factory,
                                               int holding_start_reg,
                                               int holding_regs_per_request,
                                               int input_start_reg,
                                               int input_regs_per_request);
  ~ModbusClientDividableWrapperFactory();

  std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                     int port,
                                                     int modbus_id,
                                                     int start_holding_reg_num,
                                                     int holding_regs_count,
                                                     int start_input_reg_num,
                                                     int input_regs_count) override;

  std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;

 private:
  struct ModbusClientDividableWrapperFactoryPrivate;
  ModbusClientDividableWrapperFactoryPrivate *_impl = nullptr;
};


#endif //MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTDIVIDABLEWRAPPERFACTORY_H_
