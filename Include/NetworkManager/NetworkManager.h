#ifndef NETWORKMANAGER
#define NETWORKMANAGER

#include <memory>
#include "Connection.h"
#include "HostAddress.h"
#include <string>
#include "NonCopyable.h"
#include "Server.h"

namespace netman
{

class NetworkManagerInt;

/**
 * @brief Модуль сетевого взаимодействия
 * @details Главный класс модуля NetworkManager, предоставляет пользовательский
 *          интерфейс для управление модулем и взаимодействия с ним
 */
class NetworkManager : public NonCopyable
{
  std::unique_ptr<NetworkManagerInt> nm_impl;
public:
  NetworkManager();
  ~NetworkManager();

  /**
   * @brief Создание нового TCP соединения
   * @param address Адрес хоста
   * @param port Номер порта для подключения
   * @param ipv6 Режим работы протокола (false - ipv4)
   * @return Установленное соединение
   */
  Connection TcpConnect(const std::string address
                        , uint16_t port, bool ipv6 = false);

  /**
   * @brief Создание нового TCP соединения по HostAddress
   * @param address Класс адреса хоста
   * @param ipv6 Режим работы протокола (false - ipv4)
   * @return Установленное соединение
   */
  Connection TcpConnect(const HostAddress& address, bool ipv6 = false);

  /**
   * @brief Создание нового UDP соединения
   * @param address Адрес хоста
   * @param port Номер порта для подключения
   * @param bind_port Номер локального порта для привязки сокета
   * @param ipv6 Режим работы протокола (false - ipv4)
   * @return Установленное соединение
   */
  Connection UdpConnect(const std::string address
                        , uint16_t port
                        , uint16_t bind_port = 0
                                               , bool ipv6 = false);

  /**
   * @brief Создание нового UDP соединения по HostAddress
   * @param address Класс адреса хоста
   * @param bind_port Номер локального порта для привязки сокета
   * @param ipv6 Режим работы протокола (false - ipv4)
   * @return Установленное соединение
   */
  Connection UdpConnect(const HostAddress& address
                        , uint16_t bind_port = 0
                                               , bool ipv6 = false);
  /**
   * @brief Создание нового TCP сервера для прослушивания определённого
   * интерфейса, заданного в HostAddress
   * @param address Класс адреса хоста
   * @param ipv6 Режим работы протокола (false - ipv4)
   * @return Сервер прослушивающий данный интерфейс
   */
  Server TcpServer(const HostAddress& address, bool isIPv6 = false);
  /**
   * @brief Создание нового TCP сервера для прослушивания всех интерфейсов
   * @param port Порт на котором будет работать сервер
   * @param ipv6 Режим работы протокола (false - ipv4)
   * @return Сервер прослушивающий все интерфейсы
   */
  Server TcpServerAny(uint16_t port, bool isIPv6 = false);

};


/**
 * @brief Класс для создания строго одного экземпляра класса NetworkManager
 * @details Класс предназначен для использования в программах, в которых нужен
 *          только один экземпляр класса NetworkManager (синглетон)
 */
class NMSingleton
{
  NMSingleton() = delete;
  NMSingleton(const NMSingleton&) = delete;
public:
  /**
   * @brief Получение экземпляра объекта класса NetworkManager
   * @return Экземпляр объекта NetworkManager (синглетон)
   */
  static NetworkManager& instance() {
    static NetworkManager instance_;
    return instance_;
  }
};

}

#endif
