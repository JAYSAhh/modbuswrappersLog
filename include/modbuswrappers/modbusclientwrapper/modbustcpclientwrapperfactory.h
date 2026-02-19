#ifndef MODBUSWRAPPERS_SRC_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSTCPCLIENTWRAPPERFACTORY_H_
#define MODBUSWRAPPERS_SRC_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSTCPCLIENTWRAPPERFACTORY_H_

#include <memory>

#include "modbuswrappers/modbuswrapperfactory.h"
#include <modbuswrappers/export.h>

class DtLogger;
class Notifier;

/**
 * @deprecated
 * Изпользуйте ModbusClientWrapperFactory
 */
class MODBUSWRAPPERS_EXPORT ModbusTcpClientWrapperFactory: public ModbusWrapperFactory {
 public:
  explicit ModbusTcpClientWrapperFactory(const std::shared_ptr<DtLogger> &logger,
                                         const std::shared_ptr<Notifier> &notifier,
                                         int timeout_in_sec = 1);
  virtual ~ModbusTcpClientWrapperFactory();

  std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                     int port,
                                                     int modbus_id,
                                                     int start_holding_reg_num,
                                                     int holding_regs_count,
                                                     int start_input_reg_num,
                                                     int input_regs_count) override;

  std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;

 private:
  struct Impl;
  Impl *_impl = nullptr;

  int _timeout = 1;

  int _initial_delay = 200;
  int _max_delay = 60000;
  double _factor = 3;
  int _jitter = 300;
};
#endif //MODBUSWRAPPERS_SRC_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSTCPCLIENTWRAPPERFACTORY_H_
