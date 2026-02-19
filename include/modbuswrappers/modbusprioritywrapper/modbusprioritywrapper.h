#ifndef MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_MODBUSPRIORITYWRAPPER_MODBUSPRIORITYWRAPPER_H_
#define MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_MODBUSPRIORITYWRAPPER_MODBUSPRIORITYWRAPPER_H_

#include <memory>

#include "modbuswrappers/modbuswrapper.h"


static const int MW_MAX_PRIORITY = 1000;


enum ModbusPriorityWrapperRequestType {
  ModbusPriorityWrapper_READ_HOLDING,
  ModbusPriorityWrapper_READ_HOLDING_MULTIPLE,
  ModbusPriorityWrapper_READ_INPUT,
  ModbusPriorityWrapper_READ_INPUT_MULTIPLE,
  ModbusPriorityWrapper_WRITE_HOLDING,
  ModbusPriorityWrapper_WRITE_HOLDING_MULTIPLE,
  ModbusPriorityWrapper_SAVE_TO_EEPROM_FUNCTION
};

struct ModbusPriorityWrapperRequest {
  ModbusPriorityWrapperRequestType request_type;
  int priority;
  std::string uid;

  int reg_num;
  int reg_count;
  std::vector<uint16_t> reg_values;
  int modbus_id;

  /**
   * Для тех случаев, когда CUSTOM_FUNCTION
   */
  int function_code;

  bool operator<(ModbusPriorityWrapperRequest const& r) const {
    // return "true" if "p1" is ordered
    // before "p2", for example:
    return priority < r.priority;
  }
};

struct ModbusPriorityWrapperResponse {
  std::string uid;

  ErrorCode error_code;

  int reg_num;
  int reg_count;
  std::vector<uint16_t> reg_values;
  int modbus_id;

  bool operator<(ModbusPriorityWrapperResponse const& r) const {
    // return "true" if "p1" is ordered
    // before "p2", for example:
    return uid < r.uid;
  }

  bool operator==(ModbusPriorityWrapperResponse const& r) const {
    // return "true" if "p1" is ordered
    // before "p2", for example:
    return uid == r.uid;
  }
};


class MODBUSWRAPPERS_EXPORT ModbusPriorityWrapper: public ModbusWrapper {
 public:
  explicit ModbusPriorityWrapper(const std::shared_ptr<ModbusWrapper> &modbus_wrapper);
  ~ModbusPriorityWrapper();

  ErrorCode changeAddress(const std::string &ip, int port, int modbus_id = 1, bool reconnect = true) override;

  std::string getIP() override;
  int getPort() override;
  int getModbusID() override;
  ErrorCode connect() override;
  ErrorCode disconnect() override;
  ErrorCode isConnected(bool &is_connected, int modbus_id) override;

  void setEnabled(bool enabled, int modbus_id) override;
  bool isEnabled(int modbus_id) override;

  ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id,
                                Priority priority = {}) override;
  ErrorCode readHoldingRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id,
                                 Priority priority = {}) override;
  ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id,
                                 Priority priority = {}) override;
  ErrorCode writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id,
                                  Priority priority = {}) override;
  ErrorCode readInputRegister(int reg_num, uint16_t &value, int modbus_id,
                              Priority priority = {}) override;
  ErrorCode readInputRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id,
                               Priority priority = {}) override;
  ErrorCode saveToEEPROM(int modbus_id, Priority priority = {}) override;
  void addConnectable(Connectable *connectable) override;
  void sendConnectionStatus(bool connection_status) override;
  void process() override;

 private:
  struct ModbusPriorityWrapperPrivate;
  ModbusPriorityWrapperPrivate *_impl = nullptr;

  void processRequest();

  ModbusPriorityWrapperResponse getResponse(const std::string &uid, bool &is_ok);
};

#endif //MODBUSWRAPPERS_INCLUDE_MODBUSWRAPPERS_MODBUSPRIORITYWRAPPER_MODBUSPRIORITYWRAPPER_H_
