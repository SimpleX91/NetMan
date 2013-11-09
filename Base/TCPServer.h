#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "BaseSocket.h"
#include <memory>


namespace netman
{

class TCPSocket;

/**
 * @brief Сервер TCP. Работает с протоколами IPv4 & IPv6
 */
class TCPServer : public BaseSocket
{
public:
  /**
   * @brief Конструктор по-умолчанию
   * @param isIPv6 Задает режим работы сокета (IPv4 | IPv6)
   */
  TCPServer(bool isIPv6 = false);

  /**
   * @brief Привязка сокета к порту
   * @param addr Порт, заданный адресом (any:port)
   * @return true|false удалось ли привязать сокет к порту
   */
  bool bindTo(const SocketAddress& addr);

  /**
   * @brief acceptConnection Прием соединения с хостом
   * @return указатель на сокет
   */
  std::unique_ptr<TCPSocket> acceptConnection();

  /**
   * @brief startListen Начать прослушивание сокета
   * @notes вызывать только после bindTo!
   */
  void startListen();
};

}
#endif // TCPSERVER_H
