#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPERFACTORY_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPERFACTORY_H_

#include <memory>

#include "modbuswrappers/modbuswrapperfactory.h"
#include <modbuswrappers/export.h>

#include <modbus/modbus.h>

using PackageType = modbus::ModbusClient::PackageType;

class DtLogger;
class Notifier;

class MODBUSWRAPPERS_EXPORT ModbusClientWrapperFactory: public ModbusWrapperFactory {
 public:
  explicit ModbusClientWrapperFactory(const std::shared_ptr<DtLogger> &logger,
                                      const std::shared_ptr<Notifier> &notifier,
                                      int retry_count,
                                      std::chrono::system_clock::duration min_interval_between_requests,
                                      PackageType package_type,
                                      int timeout_in_sec = 1);
  /**
   * @brief Конструктор с указанием всех параметров ретрайера
   * @param initial_delay изначальная задержка ретрайера
   * @param max_delay максимальная задержка ретрейра
   * @param factor коэффициент увеличения задержки ретрайера
   * @param jitter рандомный параметр, обеспечивающий колебание задержки ретрайера
   * @param timeout_in_sec таймаут для TCP
   */
  explicit ModbusClientWrapperFactory(int initial_delay,
                                      int max_delay,
                                      int factor,
                                      int jitter,
                                      const std::shared_ptr<DtLogger> &logger,
                                      const std::shared_ptr<Notifier> &notifier,
                                      int retry_count,
                                      std::chrono::system_clock::duration min_interval_between_requests,
                                      PackageType package_type,
                                      int timeout_in_sec = 1);
  virtual ~ModbusClientWrapperFactory();

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
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSCLIENTWRAPPER_MODBUSCLIENTWRAPPERFACTORY_H_
