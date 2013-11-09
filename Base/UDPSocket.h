#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "BaseSocket.h"
#include "SocketAddress.h"
#include "NetworkManager/ByteArray.h"

namespace netman
{

/**
 * @brief Класс-обертка над UDP сокетом
 */
class UDPSocket : public BaseSocket
{
  typedef std::pair<ByteArray, SocketAddress> HostData;

  size_t max_packet_size;
public:
  /**
   * @brief Конструктор по-умолчанию
   * @param isIPv6 Задает режим работы сокета (IPv4 | IPv6)
   */
  UDPSocket(bool isIPv6 = false);

  ~UDPSocket();

  /**
   * @brief Привязка сокета к порту
   * @param addr Порт, заданный адресом (any:port)
   * @return true|false удалось ли привязать сокет к порту
   */
  bool bindTo(const SocketAddress& addr);

  /**
   * @brief sendDataTo Отправить датаграмму хосту
   * @param data данные
   * @param host адрес хоста
   */
  void sendDataTo(const ByteArray& data, const SocketAddress& host) const;

  /**
   * @brief readDataFrom Считать датаграмму
   * @param buffer_size Размер буфера под датаграмму. Указать 0 для
   *                    автоопределения размера.
   * @return Пара ByteArray+SocketAddress
   */
  HostData readDataFrom(size_t buffer_size = 0) const;
};

}

#endif // UDPSOCKET_H
