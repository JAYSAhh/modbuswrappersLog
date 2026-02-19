#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_STORAGES_MODBUSWRAPPERSTORAGEIMPL1_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_STORAGES_MODBUSWRAPPERSTORAGEIMPL1_H_

#include <memory>

#include "modbuswrappers/modbuswrapperstorage.h"
#include <modbuswrappers/export.h>

class MODBUSWRAPPERS_EXPORT ModbusWrapperStorageImpl1: public ModbusWrapperStorage {
 public:
  explicit ModbusWrapperStorageImpl1();
  ~ModbusWrapperStorageImpl1();

  std::shared_ptr<ModbusWrapper> getBaseModbusWrapper(const std::string &ip, int port) override;
  void addBaseModbusWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper,
                            const std::string &ip,
                            int port) override;

 private:
  struct ModbusWrapperStorageImpl1Private;
  ModbusWrapperStorageImpl1Private *_impl = nullptr;
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_STORAGES_MODBUSWRAPPERSTORAGEIMPL1_H_
