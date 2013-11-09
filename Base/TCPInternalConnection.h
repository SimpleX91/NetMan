#ifndef TCPINTERNALCONNECTION_H
#define TCPINTERNALCONNECTION_H

#include <memory>
#include "InternalConnection.h"
#include "TCPSocket.h"
#include <mutex>

namespace netman
{

class ByteArray;
class HostAddress;

/**
 * @class TCPInternalConnection
 * @brief Реализация TCP соединения.
 */
class TCPInternalConnection : public InternalConnection
{
  std::unique_ptr<TCPSocket> socket;
  long transaction_size;
  std::mutex state_mutex;

public:

  TCPInternalConnection(const HostAddress& address, bool isIPv6 = false);
  TCPInternalConnection(TCPSocket* socket);
  virtual ~TCPInternalConnection();

  virtual SocketDescr getFd();

  virtual void sendImpl(const ByteArray& data);
  virtual ByteArray receiveImpl(unsigned long size = 0);
  virtual void notifyReceive();
  virtual void notifyTimeout();
  virtual void notifyDisconnect();
  virtual void appendBuffer(const ByteArray& data);
  virtual ByteArray readBuffer(unsigned long size);
  unsigned long readyForRead();
  virtual void reconnect();

};

}// NetworkManager
#endif // TCPINTERNALCONNECTION_H

