#ifndef TEST_PROJECT_WITH_MODULE_ARCH_SERVICES_MODELS_IMPLS_STANDALONEIMPL_DEVICES_MODBUSWRAPPERPOLL_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_SERVICES_MODELS_IMPLS_STANDALONEIMPL_DEVICES_MODBUSWRAPPERPOLL_H_

#include <memory>

#include <threadpooling/iprocessable.h>

#include <modbuswrappers/export.h>

class ModbusAsyncClientWrapper;

class MODBUSWRAPPERS_EXPORT ModbusWrapperPoll: public IProcessable {
 public:
  explicit ModbusWrapperPoll(const std::shared_ptr<ModbusAsyncClientWrapper> &modbus_client_wrapper);
  ~ModbusWrapperPoll();

  void process() override;

 private:
  struct ModbusWrapperPollPrivate;
  ModbusWrapperPollPrivate *_impl = nullptr;
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_SERVICES_MODELS_IMPLS_STANDALONEIMPL_DEVICES_MODBUSWRAPPERPOLL_H_
