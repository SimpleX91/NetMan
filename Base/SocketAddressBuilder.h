#ifndef SOCKETADDRESSBUILDER_H
#define SOCKETADDRESSBUILDER_H

#include <string>

namespace netman
{

class HostAddress;
class SocketAddress;

class SocketAddressBuilder
{
public:
  SocketAddressBuilder();
  ~SocketAddressBuilder();

  SocketAddress getAddress(const HostAddress& host,
                           bool prefer_ipv6 = false) const;
  SocketAddress getAddressByHostname(const std::string& hostname
                                     , uint16_t port
                                     , bool prefer_ipv6 = false) const;
  SocketAddress getAddressAny(uint16_t port, bool ipv6 = false) const;
  SocketAddress getAddressLocalhost(uint16_t port, bool ipv6 = false) const;

  HostAddress getHostAddress(const SocketAddress& address) const;

protected:

  SocketAddress getAddress_(const std::string& uri
                            , uint16_t port
                            , bool ipv6 = false) const;

};

} // NetworkManager
#endif // SOCKETADDRESSBUILDER_H
