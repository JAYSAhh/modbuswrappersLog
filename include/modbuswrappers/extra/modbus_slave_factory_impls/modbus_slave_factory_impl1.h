#ifndef MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_FACTORY_IMPLS_MODBUS_SLAVE_FACTORY_IMPL1_H_
#define MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_FACTORY_IMPLS_MODBUS_SLAVE_FACTORY_IMPL1_H_

#include "modbuswrappers/extra/modbus_slave_factory.h"

class MODBUSWRAPPERS_EXPORT ModbusSlaveFactoryImpl1: public ModbusSlaveFactory {
 public:
  /**
* @brief Конструктор с указанием всех параметров ретрайера
* @param initial_delay изначальная задержка ретрайера
* @param max_delay максимальная задержка ретрейра
* @param factor коэффициент увеличения задержки ретрайера
* @param jitter рандомный параметр, обеспечивающий колебание задержки ретрайера
*/
  explicit ModbusSlaveFactoryImpl1(int initial_delay,
                                   int max_delay,
                                   int factor,
                                   int jitter);

  std::shared_ptr<ModbusWrapper> createModbusSlave(const std::shared_ptr<ModbusWrapper> &base_modbus_wrapper,
                                                   int modbus_id) override;

 private:
  int _initial_delay = 200;
  int _max_delay = 60000;
  double _factor = 3;
  int _jitter = 300;
};

#endif //MODBUSWRAPPERS_SERVICE_FACOTRIES_MODBUS_SLAVE_FACTORY_IMPLS_MODBUS_SLAVE_FACTORY_IMPL1_H_
