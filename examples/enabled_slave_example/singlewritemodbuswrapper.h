#ifndef EPSS13DEVICESTANDALONEPLUGIN_DEVICEPLUGIN_ENTITY_IMPLS_STANDALONEIMPL1_SINGLEWRITEMODBUSWRAPPER_H_
#define EPSS13DEVICESTANDALONEPLUGIN_DEVICEPLUGIN_ENTITY_IMPLS_STANDALONEIMPL1_SINGLEWRITEMODBUSWRAPPER_H_

#include <modbuswrappers/modbuswrapper.h>

class SingleWriteModbusWrapper: public ModbusWrapper {
 public:
  explicit SingleWriteModbusWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper);
  ~SingleWriteModbusWrapper();

  std::string getIP() override;
  int getPort() override;
  int getModbusID() override;

  ErrorCode changeAddress(const std::string &ip, int port, int modbus_id, bool reconnect) override;

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
  void setEnabled(bool enabled, int modbus_id) override;
  bool isEnabled(int modbus_id) override;

 private:
  struct Impl;
  Impl *_impl = nullptr;
};

#endif //EPSS13DEVICESTANDALONEPLUGIN_DEVICEPLUGIN_ENTITY_IMPLS_STANDALONEIMPL1_SINGLEWRITEMODBUSWRAPPER_H_
