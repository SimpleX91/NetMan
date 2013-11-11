#include "Connection.h"
#include "InternalConnection.h"
#include "NetworkManagerInt.h"

namespace netman
{
Connection::Connection()
{
}

void Connection::send(const ByteArray& data, unsigned long ms_timeout)
{
  conn_handler->sendImpl(data);
}

ByteArray Connection::receive(unsigned long size, unsigned long ms_timeout)
{
  unsigned long new_timeout = (ms_timeout != 0) ? ms_timeout : get_timeout();

  if (new_timeout)
    conn_handler->setTimeout(new_timeout);

  ByteArray result = conn_handler->receiveImpl(size);

  if (new_timeout && getState() != TimedOut)
    conn_handler->resetTimeout();

  return result;
}

ByteArrayUnion Connection::receiveAll()
{

}

void Connection::sendBurst(const ByteArrayUnion& data, unsigned long ms_timeout)
{
  for (const auto & packet : data)
    conn_handler->sendImpl(packet);
}

unsigned long Connection::readyForRead()
{
  conn_handler->readyForRead();
}

void Connection::reconnect()
{
  conn_handler->reconnect();
}

int Connection::getState()
{
  return conn_handler->getState();
}

}
