#ifndef MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPER_TANGO_SERVICE_IMPLS_MODBUS_WRAPPER_TANGO_SERVICE_IMPL1_H_
#define MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPER_TANGO_SERVICE_IMPLS_MODBUS_WRAPPER_TANGO_SERVICE_IMPL1_H_

#include <memory>

#include "modbuswrappers/service/modbus_wrapper_service.h"

class ModbusWrapperModel;

class MODBUSWRAPPERS_EXPORT ModbusWrapperServiceImpl1: public ModbusWrapperService {
 public:
  explicit ModbusWrapperServiceImpl1(const std::shared_ptr<ModbusWrapperModel> &model);
  ~ModbusWrapperServiceImpl1();

  std::tuple<ErrorCode, std::string, int> getTCPConnectionData() override;
  ErrorCode setTCPConnectionData(const std::string &ip, int port) override;
  std::tuple<ErrorCode, ConnectionStatus> isConnectedTCP() override;
  ErrorCode connectTCP() override;
  ErrorCode disconnectTCP() override;
  std::tuple<ErrorCode, ConnectionStatus> isConnected(int modbus_id) override;
  ErrorCode connect(int modbus_id) override;
  ErrorCode disconnect(int modbus_id) override;
  ErrorCode addModbusSlave(int modbus_slave) override;

  std::vector<int> getModbusSlaves() override;
  bool hasModbusSlave(int modbus_id) override;
  std::shared_ptr<ModbusWrapper> getModbusSlave(int modbus_id) override;
  std::vector<UID> getModbusSlaveWrappers(int modbus_id) override;
  std::shared_ptr<ModbusSlaveWrapper> getModbusSlaveWrapper(const UID &uid) override;

  std::tuple<ErrorCode, std::string> addModbusSlaveWrapper(int modbus_id,
                                                           int start_holding_reg_num,
                                                           int holding_regs_count,
                                                           int start_input_reg_num,
                                                           int input_regs_count,
                                                           int period_in_ms) override;

  std::tuple<int, int> getModbusSlaveWrapperHoldingRegsRange(const UID &uid) override;
  std::tuple<int, int> getModbusSlaveWrapperInputRegsRange(const UID &uid) override;
  int getModbusSlaveWrapperMinUpdatePeriodInMs(const UID &uid) override;
  ErrorCode removeModbusSlave(int modbus_id) override;
  ErrorCode removeModbusSlaveWrapper(const UID &uid) override;
  void addConnectable(int modbus_id, Connectable *connectable) override;

  void process() override;
  ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id, Priority priority = {}) override;
  ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id, Priority priority = {}) override;

  ErrorCode readInputRegisters(int reg_num,
                               int reg_count,
                               std::vector<uint16_t> &values,
                               int modbus_id,
                               Priority priority) override;

 private:
  struct ModbusWrapperServiceImpl1Private;
  ModbusWrapperServiceImpl1Private *_impl = nullptr;

  std::shared_ptr<ModbusSlaveWrapper> getModbusSlaveWrapperByHoldingRegsRange(int modbus_id,
                                                                              int first_holding_reg_num,
                                                                              int holding_regs_count);
  std::shared_ptr<ModbusSlaveWrapper> getModbusSlaveWrapperByInputRegsRange(int modbus_id,
                                                                            int first_input_reg_num,
                                                                            int input_regs_count);
};

#endif //MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPER_TANGO_SERVICE_IMPLS_MODBUS_WRAPPER_TANGO_SERVICE_IMPL1_H_
