#ifndef NETWORKMANAGER_INT
#define NETWORKMANAGER_INT
#include "InternalAbstract.h"
#include "BSDWinDiff.h"
#include "Reactor.h"
#include <map>

/** TODO: Need to delete InternalConnection
 * from map when destructor is called **/

namespace netman
{

typedef int timerFd;
class InternalAbstract;

struct fdStruct {
  InternalAbstract* conn;
  timerFd timer = 0;
};

/**
 * @class NetworkManagerInt
 * @brief Является внутренней реализацией основного класса сетевого модуля.
 * @details Содержит в себе реактор и выполняет операции на уровне дескрипторов
 * сокета.
 */

class NetworkManagerInt : ReactorInterface
{
  /**
   * @brief Указатель на реактор
   */
  std::unique_ptr<Reactor> nm_reactor;
  /**
   * @brief Карта сокетов и соответствующих им соединений и таймеров
   */
  std::map<SocketDescr, fdStruct> fdMap;
  /**
   * @brief Карта таймеров и соответствующих им соединений
   */
  std::map<timerFd, InternalAbstract*> timerMap;
public:
  NetworkManagerInt();
  virtual ~NetworkManagerInt() {}
  /**
   * @brief Установить новое событие на чтение
   * @param this_conn Внутреняя реализация соединения, которое участвует в
   * событии.
  */
  void setRead(InternalAbstract* this_conn);
  /**
   * @brief Установка тайм-аута на соединение
   * @param this_conn Внутреняя реализация соединения, которое участвует в
   * событии.
   * @param ms_timeout Размер тайм-аута в миллисекундах
  */
  void setTimeout(InternalAbstract* this_conn, long ms_timeout);
  /**
   * @brief Сброс тайм-аута
   * @param this_conn Внутреняя реализация соединения, которое участвует в
   * событии.
  */
  void resetTimeout(InternalAbstract* this_conn);
  /**
   * @brief Функция обратной связи по событию чтения
   * @param fd Дескриптор сокета на котором произошло событие.
  */
  void onSocketEvent(int fd);
  /**
   * @brief Функция обратной связи по событию тайм-аута
   * @param timer_id идентификатор таймера.
  */
  void onTimerElapsed(int timer_id);
  /**
   * @brief Функция обратной связи по событию ошибки на сокете
   * @param fd Дескриптор сокета на котором произошла ошибка.
  */
  void onError(int fd);
  /**
   * @brief Функция обратной связи по разрыву соединения
   * @param fd Дескриптор сокета на котором произошёл разрыв соединения.
  */
  void onPeerConnectionClosed(int fd);

};

}
#endif
