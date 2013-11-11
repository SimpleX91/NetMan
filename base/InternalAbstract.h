#ifndef INTERNALABSTRACT_H
#define INTERNALABSTRACT_H

#include "NonCopyable.h"
#include "ExternalAbstract.h"
#include "BSDWinDiff.h"
#include <condition_variable>
#include <mutex>


namespace netman
{
/**
 * @class ExternalAbstract
 * @brief Является базовым классом для внутренней реализации сервера и
 * соединения
 * @details Содержит в себе общие поля и методы внутренней реализации классов
 * соединения и сервера
 */
class NetworkManagerInt;

class InternalAbstract : NonCopyable
{
  ConnStates state = Idle;
  bool done_ = false;
  NetworkManagerInt* nm_handler;
protected:
  std::condition_variable waitVar;
  std::mutex syncMutex;
public:

  /**
   * @brief Функция получения дескриптора сокета. Используется
   * в NetworkManager'e для работы с реактором.
   * @return Возвращает дескриптор сокета.
   */
  virtual SocketDescr getFd() = 0;
  /**
   * @brief Метод уведомления данного соединения об успешнм приёме данных.
   */
  virtual void notifyReceive() = 0;
  /**
   * @brief Метод уведомления данного об истечении времени данного на операцию.
   */
  virtual void notifyTimeout() = 0;
  /**
   * @brief Метод уведомления о разрыве соединения удалённым хостом.
   */
  virtual void notifyDisconnect() = 0;
  /**
    * @brief Проверка на занятость данного соединения.
    * @return Возвращает true если соединение занято операцией.
    */
  inline bool isPending() {
    return state == Pending;
  }
  /**
    * @brief Устанавливает состояние соединения - занято.
    */
  inline void setPending() {
    state = Pending;
  }
  /**
    * @brief Проверка не закончилась ли последняя операция соединения тайм-аутом.
    * @return Возвращает true если произошёл тайм-аут.
    */
  inline bool isTimedOut() {
    return state == TimedOut;
  }
  /**
    * @brief Устанавливает состояние соединения - операция завершилась
    * по тайм-ауту.
    */
  inline void setTimedOut() {
    state = TimedOut;
  }
  /**
   * @brief Проверка не прервалась ли последняя операция разрывом соединения.
   * @return Возвращает true если произошёл разрыв соединения.
   */
  inline bool isDisconnected() {
    return state == Disconnected;
  }
  /**
   * @brief Устанавливает состояние соединения - соединение разорвалось.
   */
  inline void setDisconnected() {
    state = Disconnected;
  }
  /**
   * @brief Проверка не занято ли соединение
   * @return Возвращает true соединение не занято и готово к выполнению
   * операций.
   */
  inline bool isIdle() {
    return state == Idle;
  }
  /**
   * @brief Устанавливает состояние соединения - не занято.
   */
  inline void setIdle() {
    state = Idle;
  }
  /**
   * @brief Функция получения состояние
   * @return Возвращает число, соответствующее перечислению connStates.
   */
  inline ConnStates getState() {
    return state;
  }
  /**
   * @brief Устанавливает флаг завершения операции.
   */
  inline void doDone() {
    done_ = true;
  }
  inline void resetDone() {
    done_ = false;
  }
  inline bool isDone() {
    return done_;
  }


  inline NetworkManagerInt* getNMHandler() {
    return nm_handler;
  }
  inline void setNMHandler(NetworkManagerInt* nm_handler_) {
    nm_handler = nm_handler_;
  }
  void setTimeout(unsigned long ms_timeout);
  void resetTimeout();
};


}
#endif
