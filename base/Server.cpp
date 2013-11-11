#include "Server.h"

#include "ByteArray.h"
#include "Connection.h"

#include "InternalConnection.h"
#include "TCPInternalConnection.h"
#include "UDPInternalConnection.h"
#include "InternalServer.h"
#include "NetworkManagerInt.h"


namespace netman
{

Server::Server()
{
}

Connection Server::accept(unsigned long ms_timeout, bool oneshot)
{
  unsigned long new_timeout = (ms_timeout != 0) ? ms_timeout : get_timeout();

  if (new_timeout)
    server_handler->getNMHandler()->setTimeout(server_handler.get(), new_timeout);

  Connection new_conn;

  new_conn.conn_handler =
    std::unique_ptr<TCPInternalConnection>(
      new TCPInternalConnection((TCPSocket*)server_handler->acceptImpl())
    );
  server_handler->getNMHandler()->setRead(new_conn.conn_handler.get());

  if (new_timeout && getState() != TimedOut)
    server_handler->getNMHandler()->resetTimeout(server_handler.get());

  return new_conn;

}

int Server::getState()
{
  return server_handler->getState();
}

}
