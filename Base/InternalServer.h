#ifndef INTERNALSERVER_H
#define INTERNALSERVER_H
#include "InternalAbstract.h"
#include "BaseSocket.h"
#include "TCPServer.h"
#include "NetworkManager/Server.h"

namespace netman
{
/**
 * @class InternalServer
 * @brief Является базовым классом внутренней реализации сервера для разных
 * протоколов.
 * @details Содержит в себе общие поля и методы для разных реализаций сервера.
 */
class InternalServer : public InternalAbstract
{
  ServerAsyncInterface* cb_interface;
  bool isAsync;
public:
  InternalServer() {}

  virtual ~InternalServer() {}
  /**
    * @brief Принятие входящего соединения
    * @return Возвращает BaseSocket - обёртку над сокетом.
   */
  virtual BaseSocket* acceptImpl() = 0;
  virtual void notifyReceive() = 0;
  virtual void notifyTimeout() = 0;
  virtual void notifyDisconnect() = 0;
  virtual SocketDescr getFd() = 0;
};
/**
 * @class TCPInternalServer
 * @brief Реализация сервера TCP.
 */
class TCPInternalServer : public InternalServer
{
  TCPServer server;
  TCPSocket* incomeSocket;

public:
  TCPInternalServer(const HostAddress& address, bool isIPv6 = false);
  TCPInternalServer(int port, bool isIPv6 = false);
  virtual ~TCPInternalServer() {}
  virtual BaseSocket* acceptImpl();
  virtual SocketDescr getFd();


  virtual void notifyReceive();
  virtual void notifyTimeout();
  virtual void notifyDisconnect() {}

};

}


#endif
