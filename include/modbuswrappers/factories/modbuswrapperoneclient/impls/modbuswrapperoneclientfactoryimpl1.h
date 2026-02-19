#ifndef MODBUSWRAPPERDEVICE_SERVICE_FACTORIES_IMPLS_MODBUSWRAPPERONECLIENTFACTORYIMPL1_H_
#define MODBUSWRAPPERDEVICE_SERVICE_FACTORIES_IMPLS_MODBUSWRAPPERONECLIENTFACTORYIMPL1_H_

#include "modbuswrappers/modbuswrapperfactory.h"

#include <modbuswrappers/export.h>


class MODBUSWRAPPERS_EXPORT ModbusWrapperOneClientFactoryImpl1: public ModbusWrapperFactory {
 public:
  explicit ModbusWrapperOneClientFactoryImpl1(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory);
  ~ModbusWrapperOneClientFactoryImpl1();

  std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                     int port,
                                                     int modbus_id,
                                                     int start_holding_reg_num,
                                                     int holding_regs_count,
                                                     int start_input_reg_num,
                                                     int input_regs_count) override;

  std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;

 private:
  struct ModbusWrapperOneClientFactoryImpl1Private;
  ModbusWrapperOneClientFactoryImpl1Private *_impl = nullptr;
};

#endif //MODBUSWRAPPERDEVICE_SERVICE_FACTORIES_IMPLS_MODBUSWRAPPERONECLIENTFACTORYIMPL1_H_
