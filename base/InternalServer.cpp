#include "InternalServer.h"
#include "Exception.h"
#include "SocketAddressBuilder.h"
#include "SocketAddress.h"
#include "TCPSocket.h"


namespace netman
{

TCPInternalServer::TCPInternalServer(const HostAddress& address, bool isIPv6)
  : server(isIPv6)
{
  SocketAddressBuilder builder;
  server.bindTo(builder.getAddress(address));
  server.startListen();
}

TCPInternalServer::TCPInternalServer(int port, bool isIPv6) : server(isIPv6)
{
  SocketAddressBuilder builder;
  server.bindTo(builder.getAddressAny(port, isIPv6));
  server.startListen();
}


BaseSocket* TCPInternalServer::acceptImpl()
{
  if (!isPending())
    setPending();
  else throw std::string("Already pending");

  std::unique_lock<std::mutex> lock(syncMutex);
  waitVar.wait(lock, [&] {return !isPending();});

  if (isTimedOut()) {
    setIdle();
    throw ServerAcceptTimeout("TCPINTERNAL_SERVER_ACCEPT_TIMEOUT");
  } else {
    setIdle();
    return incomeSocket;
  }

}

SocketDescr TCPInternalServer::getFd()
{
  return server.getSocket();
}

void TCPInternalServer::notifyReceive()
{
  if (isPending()) {
    setIdle();
    incomeSocket = server.acceptConnection().release();
    waitVar.notify_one();
  }
}

void TCPInternalServer::notifyTimeout()
{
  setTimedOut();
  waitVar.notify_one();
}

} // namespace netman
