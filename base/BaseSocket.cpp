#include "BaseSocket.h"
#include "SocketAddress.h"
#include "ByteArray.h"
#include "Exception.h"

namespace netman
{

BaseSocket::BaseSocket()
{
}

BaseSocket::BaseSocket(SocketDescr socket)
  : socket_(socket)
{
}

SocketDescr BaseSocket::createSocket(int type, int protocol, bool ipv6)
{
  SocketDescr my_socket;
  my_socket = socket((ipv6) ? AF_INET6 : AF_INET
                     , type
                     , protocol);

  if (my_socket == INVALID_SOCKET)
    throw SockInitError("SOCKET_INIT_FAILED");

  return my_socket;
}

BaseSocket::~BaseSocket()
{
  closesocket(this->socket_);
}

bool BaseSocket::bindTo_(const SocketAddress& addr)
{
  int err = bind(socket_, addr.getAddressPointer(), addr.getSize());

  if (err)
    return false;

  return true;
}

ByteArray BaseSocket::readData(size_t buffer_size) const
{
  char buffer[buffer_size];
  int read_size = recv(this->socket_, buffer, buffer_size, 0);

  if (read_size < 0)
    throw SockRecieveFailed("SOCKET_RECIEVE_ERROR");
  else if (read_size == 0)
    throw SockConnectionLost("SOCKET_CONNECTION_LOST");

  return ByteArray(buffer, read_size);
}

ByteArray BaseSocket::readAllData() const
{
  unsigned int size = getReadAvailable();
  return (size) ? readData(size) : ByteArray();
}

SocketDescr BaseSocket::getSocket() const
{
  return socket_;
}

uint16_t BaseSocket::getPort() const
{
  sockaddr addr;
  socklen_t size = sizeof(addr);

  if (getsockname(socket_, &addr, &size) == 0) {
    return ntohs(reinterpret_cast<const USockAddr*>(&addr)->port);
  }

  return 0;
}

size_t BaseSocket::getReadAvailable() const
{
  u_long size = 0;
  int err = ioctlsocket(socket_, FIONREAD, &size);

  if (err == SOCKET_ERROR)
    throw SockGenericError("SOCKET_READ_AVAILABLE_ERROR"
                           , "SOCKET_IOCTL_COULD_NOT_GET_BUFFER_SIZE");

  return size;
}

} //NetworkManager
