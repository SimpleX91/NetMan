#include "UDPSocket.h"
#include "BSDWinDiff.h"
#include "Exception.h"

namespace netman
{

UDPSocket::UDPSocket(bool isIPv6)
  : max_packet_size(1500)
{
  socket_ = createSocket(SOCK_DGRAM, IPPROTO_UDP, isIPv6);
}

UDPSocket::~UDPSocket()
{
}

bool UDPSocket::bindTo(const SocketAddress& addr)
{
  return bindTo_(addr);
}

void UDPSocket::sendDataTo(const ByteArray& data,
                           const SocketAddress& host) const
{
  int rslt = sendto(socket_, data.data(), data.size(), 0
                    , host.getAddressPointer(), host.getSize());

  if (rslt == SOCKET_ERROR)
    throw SockSendError("UDP_SEND_FAILED", "UDP_SEND_ERROR_OCCURED");
  else if (rslt != data.size())
    throw SockSendError("UDP_SEND_FAILED", "UDP_SEND_NOT_ALL_DATA_SENT");
}

UDPSocket::HostData UDPSocket::readDataFrom(size_t buffer_size) const
{
  if (buffer_size == 0) {
    //TODO: Calculate datagram size
    buffer_size = this->max_packet_size;
  }

  char buf[buffer_size];
  sockaddr_storage addr;
  socklen_t size = sizeof(addr);
  int rslt = 0;

  rslt = recvfrom(socket_, buf, buffer_size, 0
                  , reinterpret_cast<sockaddr*>(&addr)
                  , &size);

  if (rslt == SOCKET_ERROR)
    throw SockRecieveFailed("UDP_SOCKET_READ_ERROR");

  return HostData(ByteArray(buf, rslt), SocketAddress(addr));
}

}//NetworkManager
