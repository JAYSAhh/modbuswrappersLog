#ifndef TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPER_H_
#define TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPER_H_

#include <set>
#include <shared_mutex>
#include <memory>

#include <utils/baseutils.h>
#include <threadpooling/iprocessable.h>

#include <modbuswrappers/export.h>

class Connectable;

struct Priority {
  bool is_custom_priority = false;
  int custom_priority = 1;
};

class MODBUSWRAPPERS_EXPORT ModbusWrapper: public IProcessable {
 public:
  explicit ModbusWrapper();
  virtual ~ModbusWrapper();

  virtual ErrorCode changeAddress(const std::string &ip, int port, int modbus_id = 1, bool reconnect = true) = 0;

  virtual std::string getIP() = 0;
  virtual int getPort() = 0;
  virtual int getModbusID() = 0;

  virtual ErrorCode connect() = 0;
  virtual ErrorCode disconnect() = 0;

  /**
   * @brief Разрешает или запрещает обмен с modbus устройством
   * @param enabled true -- разрешить, false -- запретить
   * @param modbus_id modbus_id устройства
   * @return
   */
  virtual void setEnabled(bool enabled, int modbus_id);
  virtual bool isEnabled(int modbus_id);

  virtual void setIsOpened(bool is_opened, int modbus_id);
  virtual bool isOpened(int modbus_id);

  /**
   * @brief Возвращает результат последней проверки метода isConnected
   *
   * @note  В отличие от метода isConnected не проверяет сокет, а просто возвращает
   * значение последней проверки
   * @return true -- если есть подключени, false -- если его нет
   */

  virtual ErrorCode isConnected(bool &is_connected, int modbus_id = 1) = 0;
  virtual ErrorCode readHoldingRegister(int reg_num, uint16_t &value, int modbus_id = 1,
                                        Priority priority = {}) = 0;
  virtual ErrorCode readHoldingRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id = 1,
                                         Priority priority = {}) = 0;
  virtual ErrorCode writeHoldingRegister(int reg_num, uint16_t value, int modbus_id = 1,
                                         Priority priority = {}) = 0;
  virtual ErrorCode writeHoldingRegisters(int reg_num, std::vector<uint16_t> value, int modbus_id = 1,
                                          Priority priority = {}) = 0;

  virtual ErrorCode readInputRegister(int reg_num, uint16_t &value, int modbus_id = 1,
                                      Priority priority = {}) = 0;
  virtual ErrorCode readInputRegisters(int reg_num, int reg_count, std::vector<uint16_t> &values, int modbus_id = 1,
                                       Priority priority = {}) = 0;

  virtual ErrorCode saveToEEPROM(int modbus_id = 1,
                                 Priority priority = {}) = 0;

  virtual void addConnectable(Connectable *connectable) = 0;
  virtual void sendConnectionStatus(bool connection_status) = 0;

  virtual void process() override;

    private:
        struct ModbusWrapperPrivate;
        ModbusWrapperPrivate *_impl1 = nullptr;
};

#endif //TEST_PROJECT_WITH_MODULE_ARCH_MODBUSWRAPPERS_MODBUSWRAPPER_H_
