#include "TCPSocket.h"
#include "SocketAddress.h"
#include "BSDWinDiff.h"
#include "Exception.h"
#include "ByteArray.h"

namespace netman
{

TCPSocket::TCPSocket(bool isIPv6)
{
  socket_ = createSocket(SOCK_STREAM, IPPROTO_TCP, isIPv6);
}

TCPSocket::TCPSocket(SocketDescr descr
                     , const SocketAddress& addr)
  : BaseSocket(descr), host(addr)
{
}

TCPSocket::~TCPSocket()
{

}

void TCPSocket::connectToHost(const SocketAddress& host)
{
  int err = connect(socket_, host.getAddressPointer(), host.getSize());

  if (err)
    throw SockConnectionError("TCP_SOCKET_CONNECT_FAILED");
}

bool TCPSocket::bindTo(const SocketAddress& addr)
{
  return bindTo_(addr);
}

void TCPSocket::sendData(const ByteArray& data) const
{
  unsigned int total_sent = 0;

  while (total_sent < data.size()) {
    int sent = send(this->socket_
                    , &data[total_sent]
                    , data.size() - total_sent, 0);

    if (sent == SOCKET_ERROR)
      throw SockSendError("TCP_SOCKET_SEND_FAILED"
                          , "TCP_SOCKET_NOT_ALL_DATA_SENT");

    total_sent += sent;
  }
}

const SocketAddress& TCPSocket::getConnectedHost() const
{
  return this->host;
}

}
