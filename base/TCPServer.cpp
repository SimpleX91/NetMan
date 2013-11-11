#include "TCPServer.h"
#include "TCPSocket.h"
#include "Exception.h"

namespace netman
{

TCPServer::TCPServer(bool isIPv6)
{
  socket_ = createSocket(SOCK_STREAM, IPPROTO_TCP, isIPv6);
}

bool TCPServer::bindTo(const SocketAddress& addr)
{
  return bindTo_(addr);
}

std::unique_ptr<TCPSocket> TCPServer::acceptConnection()
{
  sockaddr_storage storage;
  socklen_t size = sizeof(storage);
  int socket = accept(socket_
                      , reinterpret_cast<sockaddr*>(&storage)
                      , &size);

  if (socket == INVALID_SOCKET)
    throw SockAcceptFailed("TCP_SERVER_ACCEPT_FAILED");

  SocketAddress address(storage);

  return std::unique_ptr<TCPSocket>(new TCPSocket(socket, address));
}

void TCPServer::startListen()
{
  int err = listen(socket_, 0);

  if (err)
    throw SockListenError("TCP_SERVER_LISTEN_FAILED"
                          , "TCP_SERVER_COULD_NOT_START_LISTENING");
}

}// NetworkManager
