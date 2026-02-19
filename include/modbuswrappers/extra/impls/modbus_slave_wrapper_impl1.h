#ifndef MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPERS_IMPLS_MODBUS_SLAVE_WRAPPER_IMPL1_H_
#define MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPERS_IMPLS_MODBUS_SLAVE_WRAPPER_IMPL1_H_

// TODO: refactor порядок методов

#include <memory>

#include "modbuswrappers/extra/modbus_slave_wrapper.h"

/**
 * ModbusWrapper, который является оберткой для слейва.
 * В данной реализации присутствует вся логика AsyncModbusClientWrapper,
 * но метод process может выполняться лишь по истечении определенного периода времени
 *
 * Логика работы этой обертки в том, что она не вызывает метод process сама.
 * Метод process вызывается извне, и может вызываться так часто, как это возможно. Но логика метода process
 * будет исполняться только тогда, когда будет проходить период времени с последнего исполнения этой логики.
 * Если же необходимый период был выдержан, то метод будет отрабатывать вхолостую.
 */
class MODBUSWRAPPERS_EXPORT ModbusSlaveWrapperImpl1: public ModbusSlaveWrapper {
 public:
  /**
   *
   * @param period_in_ms - период, с которым должен срабатывать process (метод может срабатывать реже, но не чаще)
   */
  ModbusSlaveWrapperImpl1(const std::shared_ptr<AbstractModbusAsyncClientWrapper> &base_modbus_wrapper, int period_in_ms);
  ~ModbusSlaveWrapperImpl1() override;

  ErrorCode changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) override;
//  ErrorCode changeAddress(const std::string &ip, int port, bool reconnect) override;
  std::string getIP() override;
  int getPort() override;
  int getModbusID() override;
  ErrorCode connect() override;
  ErrorCode disconnect() override;
  ErrorCode isConnected(bool &is_connected, int modbus_id) override;
  ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id, Priority priority) override;
  ErrorCode readHoldingRegisters(int reg_num,
                                 int reg_count,
                                 std::vector<uint16_t> &values,
                                 int modbus_id,
                                 Priority priority) override;
  ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id, Priority priority) override;
  ErrorCode writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id, Priority priority) override;
  ErrorCode readInputRegister(int reg_num, uint16_t &value, int modbus_id, Priority priority) override;
  ErrorCode readInputRegisters(int reg_num,
                               int reg_count,
                               std::vector<uint16_t> &values,
                               int modbus_id,
                               Priority priority) override;
  ErrorCode saveToEEPROM(int modbus_id, Priority priority) override;
  void addConnectable(Connectable *connectable) override;
  void sendConnectionStatus(bool connection_status) override;
  void process() override;

  const std::vector<uint16_t> &getHoldingRegs() override;
  const std::vector<uint16_t> &getInputRegs() override;
  int getHoldingRegsCount() override;
  int getInputRegsCount() override;
  ErrorCode setHoldingRegsCount(int reg_count) override;
  ErrorCode setInputRegsCount(int reg_count) override;

  int getStartHoldingRegsNum() override;
  int getStartInputRegsNum() override;
  ErrorCode setStartHoldingRegsNum(int reg_num) override;
  ErrorCode setStartInputRegsNum(int reg_num) override;

  int getMinUpdatePeriodInMs() override;
  ErrorCode setMinUpdatePeriodInMs(int period_in_ms) override;

 private:
  struct ModbusSlaveWrapperImpl1Private;
  ModbusSlaveWrapperImpl1Private *_impl = nullptr;
};

#endif //MODBUSWRAPPERS_SERVICE_MODBUS_WRAPPERS_IMPLS_MODBUS_SLAVE_WRAPPER_IMPL1_H_
