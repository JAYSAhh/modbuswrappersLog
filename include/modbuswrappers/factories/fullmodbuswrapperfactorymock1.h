#ifndef MODBUSWRAPPERS_SRC_MODBUSWRAPPERS_FULLMODBUSWRAPPERFACTORYMOCK1_H_
#define MODBUSWRAPPERS_SRC_MODBUSWRAPPERS_FULLMODBUSWRAPPERFACTORYMOCK1_H_

#include "modbuswrappers/modbuswrapperfactory.h"

#include <modbuswrappers/export.h>

class MODBUSWRAPPERS_EXPORT FullModbusWrapperFactoryMock1: public ModbusWrapperFactory {
 public:
  explicit FullModbusWrapperFactoryMock1();
  ~FullModbusWrapperFactoryMock1();

  std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                     int port,
                                                     int modbus_id,
                                                     int start_holding_reg_num,
                                                     int holding_regs_count,
                                                     int start_input_reg_num,
                                                     int input_regs_count) override;
  std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;

 private:
  struct FullModbusWrapperFactoryMock1Private;
  FullModbusWrapperFactoryMock1Private *_impl = nullptr;
};

#endif //MODBUSWRAPPERS_SRC_MODBUSWRAPPERS_FULLMODBUSWRAPPERFACTORYMOCK1_H_
