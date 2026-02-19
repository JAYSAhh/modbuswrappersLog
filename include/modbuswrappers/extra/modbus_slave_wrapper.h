#ifndef MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_EXTRA_MODBUS_SLAVE_WRAPPER_H_
#define MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_EXTRA_MODBUS_SLAVE_WRAPPER_H_

#include "modbuswrappers/modbusasyncclientwrapper/abstractmodbusasyncclientwrapper.h"


/**
 * Абстракция для Modbus слейв обертки. В этой абстракции добавляется регулирование
 * параметра, отвечающего за минимальный период обновления.
 *
 * Благодаря этому параметру логика метода process бадового modbus_wrapper не будет вызываться чаще,
 * чем параметр _period_in_ms
 */
class MODBUSWRAPPERS_EXPORT ModbusSlaveWrapper: public AbstractModbusAsyncClientWrapper {
 public:
  virtual int getMinUpdatePeriodInMs() = 0;
  virtual ErrorCode setMinUpdatePeriodInMs(int period_in_ms) = 0;
};

#endif //MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_EXTRA_MODBUS_SLAVE_WRAPPER_H_
