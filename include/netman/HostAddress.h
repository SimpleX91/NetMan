#ifndef HOSTADDRESS_H
#define HOSTADDRESS_H

#include <string>
#ifndef WIN32
#include <inttypes.h>
#endif

namespace netman
{

/**
 * @brief Адрес хоста в виде address:port, где адрес может быть доменным именем
 */
class HostAddress
{
public:
  HostAddress();
  HostAddress(const std::string& address, uint16_t port);

  /**
   * @brief Получение строки адреса
   * @return Строка адреса
   */
  std::string getAddress() const;

  /**
   * @brief Получение порта
   * @return Номер порта
   */
  uint16_t getPort() const;

  /**
   * @brief Изменение строки адреса
   * @param address Новая строка адреса
   */
  void setAddress(const std::string& address);

  /**
   * @brief Изменение номера порта
   * @param port Новый номер порта
   */
  void setPort(uint16_t port);

  /**
   * @brief Проверка на наличие данных
   * @return true, если в объекте заполнены поля
   */
  bool isEmpty();
private:
  /// Строка адреса хоста
  std::string host_;

  /// Номер порта
  uint16_t port_;
};

}

#endif // HOSTADDRESS_H
