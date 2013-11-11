#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <mutex>

#include "ByteArray.h"
#include "ExternalAbstract.h"

namespace netman
{

class InternalConnection;


/**
 * @class Connection
 * @brief Базовый класс для пользователя модуля NetworkManager.
 * @details Предоставляет интерфейс для управления и взаимодействия с
 *          соединением
 */
class Connection : public ExternalAbstract
{

  std::shared_ptr<InternalConnection> conn_handler;


  //Connection(Connection &rhs);
public:
  Connection();
  /**
   * @brief Отправка данных.
   * @param data Массив ByteArray на отправку.
   * @param ms_timeout Время ожидания выполнения команды.
   * @return Установленное соединение
   */
  void send(const ByteArray& data, unsigned long ms_timeout = 0);
  /**
   * @brief Отправка данных.
   * @param size Количество байт на приём.
   * @param ms_timeout Время ожидания выполнения команды.
   * @return Массив ByteArray пришедших данных.
   */
  ByteArray receive(unsigned long size = 0, unsigned long ms_timeout = 0);
  /**
   * @brief Получение всех данных которые есть на данный момент в буффере сокета.
   * Если на момент вызова функции данные отсутствуют, то команда ничего не
   * возвращает.
   * @return Массив ByteArray пришедших данных.
   */
  ByteArrayUnion receiveAll();
  /**
   * @brief Отправка всей очереди данных ByteArrayUnion. Актуально для UDP, когда
   * необходимо отправить несколько пакетов одной командой.
   * @param data Очередь ByteArrayUnion на отправку.
   * @param ms_timeout время ожидания выполнения команды.
   */
  void sendBurst(const ByteArrayUnion& data, unsigned long ms_timeout = 0);
  /**
   * @brief Взятие состояния соединения.
   * @return Возвращает состояние соединения.
   * Idle - никаких операция на данный момент не производится.
   * Pending - соединение в данный момент занято.
   * TimedOut - Время ожидания истекло.
   * Disconnected - соединения разрушено удалённым хостом.
   */

  int getState();
  /**
   * @brief Получение количества байт, хранящихся во внутреннем буфере и готовые
   * для чтения
   * @return Количество байт
   */
  unsigned long readyForRead();
  /**
   * @brief Повторное соединие к хосту, который был задан изначально
   */
  void reconnect();
  friend class NetworkManager;
  friend class Server;
};


}

#endif // CONNECTION_H
