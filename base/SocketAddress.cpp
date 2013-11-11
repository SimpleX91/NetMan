#include "SocketAddress.h"

#include "HostAddress.h"
#include <string>

namespace netman
{

SocketAddress::SocketAddress()
{
}

SocketAddress::SocketAddress(const sockaddr_storage& storage)
  : addr(storage)
{
}

SocketAddress::~SocketAddress()
{
}


const sockaddr_storage& SocketAddress::getStorage() const
{
  return addr;
}

const sockaddr* SocketAddress::getAddressPointer() const
{
  return reinterpret_cast<const sockaddr*>(&addr);
}

unsigned int SocketAddress::getSize() const
{
  return sizeof(addr);
}

SocketAddress::AddrType SocketAddress::getType() const
{
  if (addr.ss_family == AF_INET)
    return IPv4;
  else if (addr.ss_family == AF_INET6)
    return IPv6;
  else
    return UNKNOWN;
}

SocketFamily SocketAddress::getFamily() const
{
  return this->addr.ss_family;
}

uint16_t SocketAddress::getPort()
{
  return ntohs(reinterpret_cast<const USockAddr*>(&addr)->port);
}

bool SocketAddress::operator ==(const SocketAddress& addr) const
{
  return false;
}

}// NetworkManager
