#ifndef MODBUSWRAPPERS_SRC_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUS_CLIENT_MODBUS_ID_DECORATOR_FACTORY_IMPL2_H_
#define MODBUSWRAPPERS_SRC_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUS_CLIENT_MODBUS_ID_DECORATOR_FACTORY_IMPL2_H_

#include <memory>

#include "modbuswrappers/modbuswrapperfactory.h"
#include <modbuswrappers/export.h>

class ModbusWrapperStorage;

class MODBUSWRAPPERS_EXPORT ModbusClientModbusIDDecoratorFactoryImpl2: public ModbusWrapperFactory {
 public:
  explicit ModbusClientModbusIDDecoratorFactoryImpl2(const std::shared_ptr<ModbusWrapperFactory> &modbus_wrapper_factory,
                                                     const std::shared_ptr<ModbusWrapperStorage> &modbus_wrapper_storage);

  /**
 * @brief Конструктор с указанием всех параметров ретрайера
   * @param
 * @param initial_delay изначальная задержка ретрайера
 * @param max_delay максимальная задержка ретрейра
 * @param factor коэффициент увеличения задержки ретрайера
 * @param jitter рандомный параметр, обеспечивающий колебание задержки ретрайера
 */
  explicit ModbusClientModbusIDDecoratorFactoryImpl2(const std::shared_ptr<ModbusWrapperFactory> &base_modbus_wrapper_factory,
                                                     const std::shared_ptr<ModbusWrapperStorage> &modbus_wrapper_storage,
                                                     int initial_delay,
                                                     int max_delay,
                                                     int factor,
                                                     int jitter);
  ~ModbusClientModbusIDDecoratorFactoryImpl2();

  std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                     int port,
                                                     int modbus_id,
                                                     int start_holding_reg_num,
                                                     int holding_regs_count,
                                                     int start_input_reg_num,
                                                     int input_regs_count) override;

  std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;

 private:
  struct ModbusClientModbusIDDecoratorFactoryImpl2Private;
  ModbusClientModbusIDDecoratorFactoryImpl2Private *_impl = nullptr;
};

#endif //MODBUSWRAPPERS_SRC_MODBUSWRAPPERS_MODBUSWRAPPERMODBUSIDDECORATOR_MODBUS_CLIENT_MODBUS_ID_DECORATOR_FACTORY_IMPL2_H_
