#ifndef BASESOCKET_H
#define BASESOCKET_H

#include "BSDWinDiff.h"
#include "NonCopyable.h"

namespace netman
{

class SocketAddress;
class ByteArray;

/**
 * @brief Базовый класс для кроссплатформенной обертки над сокетами Беркли
 */
class BaseSocket : protected NonCopyable
{
public:
  BaseSocket(SocketDescr socket);
  /**
   * @brief Виртуальный деструктор
   */
  virtual ~BaseSocket();

  //========================================================================//

  /**
   * @brief Читает данные с сокета
   * @param size Размер буфера для данных
   * @return Полученные данные в виде байтового массива
   */
  virtual ByteArray readData(size_t buffer_size) const;

  /**
   * @brief Читает все данные с сокета
   * @return Полученные данные в виде байтового массива
   */
  virtual ByteArray readAllData() const;

  //========================================================================//

  /**
   * @brief Возвращает дескриптор сокета для прослушивания
   * @return Дескриптор сокета
   */
  SocketDescr getSocket() const;

  /**
   * @brief Возвращает номер порта, к которому привязан сокет
   *        или ноль, если сокет не привязан к порту
   * @return Номер порта
   */
  uint16_t getPort() const;

  /**
   * @brief Функция для определения размера полученных на сокет данных
   * @return
   */
  size_t getReadAvailable() const;

protected:
  /**
   * @brief Конструктор по умолчанию
   * @notes Отключен
   */
  BaseSocket();

  /**
   * @brief Создает сокет операционной системы
   * @param type Поле "type" функции socket
   * @param protocol Поле "protocol" функции socket
   * @param ipv6 Создать сокет для работы с протоколом IPv6 или IPv4
   * @return Дескриптор сокета
   */
  SocketDescr createSocket(int type, int protocol, bool ipv6 = false);

  /**
   * @brief bindTo_ Привязывает сокет к указанному в addr номеру порта
   * @param addr ANY адрес с портом, к которому нужно привязать сокет
   * @return true в случае успешного завершения функции, false - в противном
   */
  bool bindTo_(const SocketAddress& addr);

  /// Дескриптор сокета
  SocketDescr socket_;
};

}

#endif // BASESOCKET_H
