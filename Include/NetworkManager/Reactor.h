#ifndef REACTOR_H
#define REACTOR_H

#include <iostream>
#include "BSDWinDiff.h"
#include <memory>
#include <set>
#include <map>
#include <thread>
#include <mutex>

#define MAX_EVENTS 128

enum Timer_errors {
  TIMER_ALREADY_IDLE = -1,
  TIMER_NOT_EXISTS = -2,
  TIMER_SETTIME_ERROR = -3,
  TIMER_NOT_IDLE = -4
};


namespace netman
{

typedef int expires;
typedef std::set<int> fdSetTy;
typedef std::map<int, expires> timerMapTy;

/**
 * @class Reactor
 * @brief Класс предоставляющий одновременную работу с множеством сокетов.
 * Также предоставляет таймеры
 * @details Обычный класс.
 */

class ReactorInterface;

class Reactor
{
public:
  Reactor(ReactorInterface* callback_interface);
  ~Reactor();

  /**
   * @brief Устанавливает сокет в ожидание данных
   * @param socketFd дескриптор сокета.
   * @param edgeTriggered Reactor сигнализирует заднему фронту, а не по уровню
  */
  void setReadEvent(SocketDescr socketFd, bool edgeTriggered = false);

  /**
   * @brief Сбрасывает состояния сокета, после чего реактор перестаёт оповещать
   * о событиях на сокете
   * @param socketFd дескриптор сокета.
   * события.
  */
  void resetReadEvent(SocketDescr socketFd);

  /**
   * @brief Устанавливает таймер
   * @param time_ms Время установленное в миллисекундах (интервал).
   * @param count Количество раз срабатываний таймера (0 - бесконечно).
   * @return Возвращает идентификатор таймера если операция успешна, иначе -1
   */
  int setTimer(long time_ms, int count);

  /**
   * @brief Останавливает запущенный таймер
   * @param time_ms Время установленное в миллисекундах (интервал).
   * @return Возвращает 1, если всё поизошло успешно, при ошибке возвращает
   * значения меньше нуля описанные в перечислении stopTimer_errors
  */
  int stopTimer(int timer_id);

  /**
   * @brief Перезапускает запущенный таймер
   * @param timer_id id таймера которого нужно перезапустить.
   * @param time_ms Время установленное в миллисекундах (интервал).
   * @param count Количество раз срабатываний таймера (0 - бесконечно).
   * @return Возвращает 1, если всё поизошло успешно, при ошибке возвращает
   * значения меньше нуля описанные в перечислении Timer_errors
  */
  int restartTimer(int timer_id, long time_ms, int count);

  /// Запускает поток
  void Start();

  /**
   * @brief Останавливает поток с epoll_wait
  */
  void Stop();

  /**
   * @brief Проверяет, запущен ли реактор
  */
  bool isStarted();

protected:
  void Execute();
  void setCycleMsTime(struct itimerspec* value, long time_ms);

private:
  std::thread thread;
  ReactorInterface* callback_interface;
  std::mutex stop_thread_mutex;

  fdSetTy fdSet;
  int epollFd;
  timerMapTy timerMap;

  bool started = false;
};

/**
 * @brief Интерфейс обратной связи реактора
 * @details Реактор вызывает интерфейсные функции по событиям
 */
class ReactorInterface
{
public:
  virtual ~ReactorInterface() {}

  /**
   * @brief Функция обратной связи для оповещения события на сокете
   * @param fd номер дескриптора, который готов к операции чтения.
   */
  virtual void onSocketEvent(int fd) = 0;
  /**
   * @brief Функция обратной связи для оповещения события по таймеру
   * @param timer_id идентификатор сработавшего таймера.
   */
  virtual void onTimerElapsed(int timer_id) = 0;
  /**
   * @brief Функция обратной связи для оповещения об ошибке на сокете
   * @param fd дескриптор сокета, на котором произошла ошибка
   */
  virtual void onError(int fd) = 0;
  /**
   * @brief Функция обратной связи для оповещения об разрыва соединения от
   * удалённого хоста (Работает только с TCP соединениями)
   * @param fd дескриптор сокета, на котором произошёл разрыв TCP соединения
   */
  virtual void onPeerConnectionClosed(int fd) = 0;
};

}

// REACTOR_H
#endif
