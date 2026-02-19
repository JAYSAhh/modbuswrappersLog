#ifndef MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_FACTORY_H_
#define MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_FACTORY_H_

#include <memory>

#include "modbuswrappers/modbuswrapper.h"


class MODBUSWRAPPERS_EXPORT ModbusSlaveFactory {
 public:
  virtual std::shared_ptr<ModbusWrapper> createModbusSlave(const std::shared_ptr<ModbusWrapper> &base_modbus_wrapper,
                                                           int modbus_id) = 0;
};

#endif //MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_FACTORY_H_
