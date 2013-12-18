#include "SocketAddressBuilder.h"
#include <string>
#include <BSDWinDiff.h>
#include "SocketAddress.h"
#include "HostAddress.h"
#include "Exception.h"

namespace netman
{

SocketAddressBuilder::SocketAddressBuilder()
{
}

SocketAddressBuilder::~SocketAddressBuilder()
{
}

SocketAddress SocketAddressBuilder::getAddress(const HostAddress& host,
                                               bool prefer_ipv6) const
{
  return getAddress_(host.getAddress(), host.getPort(), prefer_ipv6);
}

SocketAddress SocketAddressBuilder
::getAddressByHostname(const std::string& hostname
                       , uint16_t port
                       , bool prefer_ipv6) const
{
  return getAddress_(hostname, port, prefer_ipv6);
}

SocketAddress SocketAddressBuilder::getAddressAny(uint16_t port
                                                  , bool ipv6) const
{
  return getAddress_((ipv6) ? "::" : "0.0.0.0", port, ipv6);
}

SocketAddress SocketAddressBuilder::getAddressLocalhost(uint16_t port
                                                        , bool ipv6) const
{
  return getAddress_("localhost", port, ipv6);
}

SocketAddress SocketAddressBuilder::getAddress_(const std::string& uri
                                                , uint16_t port
                                                , bool ipv6) const
{
  sockaddr_storage storage;
  std::string port_; port_ = std::to_string(port);
  addrinfo* result = nullptr;

  int error = getaddrinfo(uri.data(), port_.data(), nullptr, &result);

  if (error != 0) {
    freeaddrinfo(result);
    throw netman::IpNotResolved("Couldn't resolve address");
  }

  // Лямбда-функция для поиска IP адреса нужной версии
  auto findAddress = [&](bool ipv6_addr) -> sockaddr* {
    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
      if ((ipv6_addr && ptr->ai_family == AF_INET6)
      || (!ipv6_addr && ptr->ai_family == AF_INET))
        return ptr->ai_addr;
    }

    return nullptr;
  };

  // Ищем подходящий адрес
  sockaddr* buf = nullptr;

  if ((buf = findAddress(ipv6)) != nullptr
      || (buf = findAddress(!ipv6)) != nullptr) {

    storage = *reinterpret_cast<sockaddr_storage*>(buf);
    reinterpret_cast<USockAddr*>(&storage)->port = htons(port);
  } else {
    freeaddrinfo(result);
    throw netman::IpNotResolved("IP Address Not Found");
  }

  freeaddrinfo(result);
  return SocketAddress(storage);
}


HostAddress SocketAddressBuilder
::getHostAddress(const SocketAddress& address) const
{
  // Получаем IP адрес в виде строки
  char buffer[INET6_ADDRSTRLEN];
  int err = getnameinfo(address.getAddressPointer()
                        , address.getSize()
                        , buffer
                        , sizeof(buffer)
                        , 0, 0
                        , NI_NUMERICHOST);

  // Получаем порт
  uint16_t port = ntohs(
                    reinterpret_cast<const USockAddr*>(address.getAddressPointer())->port
                  );

  return HostAddress(buffer, port);
}

}// NetworkManager
