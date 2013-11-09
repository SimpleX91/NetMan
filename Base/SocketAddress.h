#ifndef SOCKETADDRESS_H
#define SOCKETADDRESS_H

#include "NetworkManager/BSDWinDiff.h"
#include <string>

namespace netman
{

class HostAddress;

class SocketAddress
{
public:
  enum AddrType {
    IPv4,
    IPv6,
    UNKNOWN
  };

  /**
   * @brief Конструктор по умолчанию
   */
  SocketAddress();

  SocketAddress(const sockaddr_storage& storage);

  virtual ~SocketAddress();

  /**
   * @function getStorage
   * @brief Предназначена для получения контейнера адреса напрямую
   * @return Константная ссылка на контейнер адреса
   */
  const sockaddr_storage& getStorage() const;

  /**
   * @brief Получение прямого указателя на адрес
   * @notes Использовать без приведения типов (т.е. без (SOCKADDR*))
   * @return Указатель на структуру адреса
   */
  const sockaddr* getAddressPointer() const;

  /**
   * @brief Возвращает размер структуры адреса
   * @return Размер структуры адреса
   */
  unsigned int getSize() const;

  /**
   * @brief Нужна для определения версии протокола, к которой принадлежит адрес
   * @return Версию протокола (IPv4 или IPv6)
   */
  AddrType getType() const;

  /**
   * @brief
   * @return Значение семейства адреса (AF_INET || AF_INET6)
   */
  SocketFamily getFamily() const;

  uint16_t getPort();

  bool operator==(const SocketAddress& addr) const;

protected:
private:
  sockaddr_storage addr;
};

} //NetworkManager

#endif // SOCKETADDRESS_H
