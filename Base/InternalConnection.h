#ifndef INTERNALCONNECTION_H
#define INTERNALCONNECTION_H

#include "NetworkManager/ByteArray.h"
#include "InternalAbstract.h"

namespace netman
{
/**
 * @class InternalConnection
 * @brief Является базовым классом внутренней реализации соединения.
 * @details Содержит в себе общие поля и методы внутренней реализации для разных
 * протоколов.
 */
class InternalConnection : public InternalAbstract
{
protected:
  ByteArrayQueue buffer;
  HostAddress host;
  bool IPv6 = false;
  mutable std::mutex buf_mutex;
public:
  InternalConnection() {}
  virtual ~InternalConnection() {}
  /**
   * @brief Отправка данных.
   * @param data Буффер на отправку.
  */
  virtual void sendImpl(const ByteArray& data) = 0;
  /**
   * @brief Приём данных.
   * @param size Количество данных для успешного завершения операции.
   * @return Возвращает буффер ByteArray с принятыми данными.
  */
  virtual ByteArray receiveImpl(unsigned long size = 0) = 0;
  virtual void notifyReceive() = 0;
  virtual void notifyTimeout() = 0;
  virtual void notifyDisconnect() = 0;
  virtual SocketDescr getFd() = 0;
  /**
   * @brief Дополнить данные во внутренний буффер.
   * @param data Буффер который будет дополнен в конец.
  */
  virtual void appendBuffer(const ByteArray& data) = 0;
  /**
   * @brief Cчитать полностью весь буффер. В случае работы в датаграммном
   * режиме считываются все пришедшие сообщения в виде массива буфферов.
   * В потоковом режиме массив из одного буффера.
   * @return Массив буфферов ByteArrayUnion.
  */
  virtual ByteArrayUnion readAllBuffer();
  /**
   * @brief Cчитать некоторое количество байт из буффера.
   * @return буффер ByteArray.
  */
  virtual ByteArray readBuffer(unsigned long size) = 0;
  /**
   * @brief Проверка на наличие данных в буфере.
   * @return Возвращает true если есть данные.
  */
  virtual bool hasData() {
    std::lock_guard<std::mutex> lock(buf_mutex);
    return (!buffer.empty());
  }
  /**
   * @brief Проверка данных в буффере сокета готовых на считывание.
   * @return Возвращает количество байт.
  */
  virtual unsigned long readyForRead() = 0;

  virtual void reconnect() = 0;

  void setHostAddress(const HostAddress& addr) {
    host = addr;
  }
  const HostAddress& getHostAddress() const {
    return host;
  }
  bool isIPv6() {
    return IPv6;
  }
  bool setIPv6(bool value) {
    IPv6 = value;
  }
};

}
#endif // INTERNALCONNECTION_H

