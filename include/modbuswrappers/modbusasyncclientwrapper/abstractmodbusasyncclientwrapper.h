#ifndef MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSASYNCCLIENTWRAPPER_ABSTRACTMODBUSASYNCCLIENTWRAPPER_H_
#define MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSASYNCCLIENTWRAPPER_ABSTRACTMODBUSASYNCCLIENTWRAPPER_H_

#include "modbuswrappers/modbuswrapper.h"


class MODBUSWRAPPERS_EXPORT AbstractModbusAsyncClientWrapper: public ModbusWrapper {
 public:
  virtual const std::vector<uint16_t> &getHoldingRegs() = 0;
  virtual const std::vector<uint16_t> &getInputRegs() = 0;

  virtual int getStartHoldingRegsNum() = 0;
  virtual int getStartInputRegsNum() = 0;

  virtual int getHoldingRegsCount() = 0;
  virtual int getInputRegsCount() = 0;

  virtual ErrorCode setStartHoldingRegsNum(int reg_num) = 0;
  virtual ErrorCode setStartInputRegsNum(int reg_num) = 0;

  virtual ErrorCode setHoldingRegsCount(int reg_count) = 0;
  virtual ErrorCode setInputRegsCount(int reg_count) = 0;
};

#endif //MODBUSWRAPPERS_LIBMODBUSWRAPPER_INCLUDE_MODBUSWRAPPERS_MODBUSASYNCCLIENTWRAPPER_ABSTRACTMODBUSASYNCCLIENTWRAPPER_H_
