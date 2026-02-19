#ifndef MODBUSWRAPPERS_MODBUSLOGWRAPPERFACTORY_H
#define MODBUSWRAPPERS_MODBUSLOGWRAPPERFACTORY_H
#include "modbuswrappers/modbuswrapperfactory.h"
#include "modbuswrappers/logaddition/modbuslogger.h"
#include "modbuswrappers/logaddition/modbuslogwrapper.h"
#include  "modbuswrappers/export.h"

class MODBUSWRAPPERS_EXPORT ModbusLogWrapperFactory: public ModbusWrapperFactory {
  public:
    explicit ModbusLogWrapperFactory(std::shared_ptr<ModbusWrapperFactory> &base_wrapper_factory);
    ~ModbusLogWrapperFactory();

    std::shared_ptr<ModbusWrapper> createModbusWrapper(const std::string &ip,
                                                 int port,
                                                 int modbus_id,
                                                 int start_holding_reg_num,
                                                 int holding_regs_count,
                                                 int start_input_reg_num,
                                                 int input_regs_count) override;

    std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip, int port) override;
    std::shared_ptr<ModbusWrapper> createBaseModbusWrapper(const std::string &ip,int port,ModbusLogger &logger);

  private:
    struct ModbusLogWrapperFactoryPrivate;
    ModbusLogWrapperFactoryPrivate *_impl = nullptr;
};



#endif //MODBUSWRAPPERS_MODBUSLOGWRAPPERFACTORY_H