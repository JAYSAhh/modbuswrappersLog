#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MOCKS_MODBUSWRAPPERMOCKFACTORY_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MOCKS_MODBUSWRAPPERMOCKFACTORY_H_

#include "modbuswrappers/modbuswrapperfactory.h"
#include <modbuswrappers/export.h>


class MODBUSWRAPPERS_EXPORT ModbusWrapperMockFactory: public ModbusWrapperFactory {
 public:
  std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                     int port,
                                                     int modbus_id,
                                                     int start_holding_reg_num,
                                                     int holding_regs_count,
                                                     int start_input_reg_num,
                                                     int input_regs_count) override;
  std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MOCKS_MODBUSWRAPPERMOCKFACTORY_H_
