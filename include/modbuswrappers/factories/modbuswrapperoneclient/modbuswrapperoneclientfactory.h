#ifndef MODBUSWRAPPERDEVICE_SERVICE_STORAGES_MODBUSWRAPPERONECLIENTFACTORY_H_
#define MODBUSWRAPPERDEVICE_SERVICE_STORAGES_MODBUSWRAPPERONECLIENTFACTORY_H_

#include <utils/baseutils.h>
#include "modbuswrappers/modbuswrapper.h"

#include <modbuswrappers/export.h>


class MODBUSWRAPPERS_EXPORT ModbusWrapperOneClientFactory {
 public:
  virtual ErrorCode setBaseModbusWrapper(const std::string &ip, int port) = 0;

  virtual ErrorCode addSlave(int modbus_id, int holding_regs_count, int input_regs_count) = 0;
  virtual ErrorCode removeSlave(int modbus_id) = 0;

  virtual
};

#endif //MODBUSWRAPPERDEVICE_SERVICE_STORAGES_MODBUSWRAPPERONECLIENTFACTORY_H_
