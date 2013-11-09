#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include "ExternalAbstract.h"
#include <mutex>

namespace netman
{

class InternalServer;
class NetworkManagerInt;
class Connection;
class ByteArray;

/**
 * @class Server
 * @brief Базовый класс для пользователя модуля NetworkManager.
 * @details Предоставляет интерфейс взаимодействия с пользовательским
 *  сервером.
 */
class Server : public ExternalAbstract
{
  std::shared_ptr<InternalServer> server_handler;
  Server();
public:
  /**
   * @brief Принимает подключение пришедшее на сокет.
   * @param ms_timeout Значение таймаута.
   * @param oneshot Если установлена, то команда отрабатывает один раз.
   * Иначе отрабатывает, пока не закроется сервер или вызовется команда stopAccept
   * По умолчанию true. В синхронном режиме доступен только режим по умолчанию.
  */
  Connection accept(unsigned long ms_timeout = 0, bool oneshot = true);

  int getState();
  friend class NetworkManager;
};

class ServerAsyncInterface
{
public:
  virtual ~ServerAsyncInterface() {}

  virtual void onNewConnection(Connection new_conn) = 0;
  virtual void onTimerElapsed() = 0;
  virtual void onError() = 0;

};

}
#endif
