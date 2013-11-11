#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "BaseSocket.h"
#include "SocketAddress.h"

namespace netman
{

/**
 * @class TCPSocket
 * @brief Класс-обертка над TCP-сокетом
 */
class TCPSocket : public BaseSocket
{
public:
  /**
   * @brief Конструктор по-умолчанию
   * @param isIPv6 Задает режим работы сокета (IPv4 | IPv6)
   */
  TCPSocket(bool isIPv6 = false);

  /**
   * @brief Конструктор инициализации по дескриптору и адресу хоста.
   *        Вызывается TCPServer'ом при приеме соединения с хостом
   * @param descr Дескриптор сокета
   * @param addr Адрес хоста, с которым соединен сокет
   */
  TCPSocket(SocketDescr descr, const SocketAddress& addr);

  ~TCPSocket();

  /**
   * @brief Подключение к хосту
   * @param host Адрес хоста для подключения
   */
  void connectToHost(const SocketAddress& host);
  /**
   * @brief Привязка сокета к порту
   * @param addr Порт, заданный адресом (any:port)
   * @return true|false удалось ли привязать сокет к порту
   */
  bool bindTo(const SocketAddress& addr);

  /**
   * @brief Отправляет данные на сокет
   * @param data Данные в виде байтового массива
   */
  virtual void sendData(const ByteArray& data) const;

  /**
   * @brief Получение адреса хоста, с которым соединен сокет
   * @return Адрес соединенного с сокетом хоста
   */
  const SocketAddress& getConnectedHost() const;

private:
  /// Адрес соединеннного с сокетом хоста
  SocketAddress host;
};

}// NetworkManager

#endif // TCPSOCKET_H
