#include "modbuswrappers/modbuswrapper.h"

#include "modbuswrappers/connectable.h"


//void ModbusWrapper::addConnectable(const std::shared_ptr<Connectable> &connectable) {
//  std::cout << "Connectable adding... " << _connectables.size() << " " << this << " " << __func__ << std::endl;
//  _connectables.push_back(connectable);
//  std::cout << "Connectable added: " << _connectables.size() << " " << this << " " << __func__ << std::endl;
//}
//
//void ModbusWrapper::sendConnectionStatus(bool connection_status) {
//  std::cout << "Sending to " << _connectables.size() << " connectables " << this << " " << __func__ << std::endl;
//  std::for_each(std::begin(_connectables), std::end(_connectables), [&](const std::shared_ptr<Connectable> &connectable) {
//    connectable->setConnectionStatus(connection_status);
//  });
//}

//ErrorCode ModbusWrapper::changeAddress(const std::string &ip, int port, bool reconnect) {
//  return OPERATION_INTERRUPTED;
//}

struct ModbusWrapper::ModbusWrapperPrivate{
  std::shared_mutex disabled_modbus_ids_mutex;
  std::set<int> disabled_modbus_ids;
};

ModbusWrapper::ModbusWrapper(): _impl1(new ModbusWrapperPrivate()) {

}

ModbusWrapper::~ModbusWrapper() {
  delete _impl1;
}

void ModbusWrapper::process() {

}

void ModbusWrapper::setEnabled(bool enabled, int modbus_id) {

}

bool ModbusWrapper::isEnabled(int modbus_id) {
  return true;
}

void ModbusWrapper::setIsOpened(bool is_opened, int modbus_id) {

}

bool ModbusWrapper::isOpened(int modbus_id) {
  return true;
}

