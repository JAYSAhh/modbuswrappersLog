#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPERSTORAGE_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPERSTORAGE_H_

#include <memory>

#include "modbuswrapper.h"


class ModbusWrapperStorage {
 public:
  virtual std::shared_ptr<ModbusWrapper> getBaseModbusWrapper(const std::string &ip, int port) = 0;
  virtual void addBaseModbusWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                                    const std::string &ip,
                                    int port) = 0;
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPERSTORAGE_H_
