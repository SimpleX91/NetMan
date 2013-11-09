#ifndef UDPINTERNALCONNECTION_H
#define UDPINTERNALCONNECTION_H

#include <memory>
#include "InternalConnection.h"
#include "SocketAddress.h"

namespace netman
{

class ByteArray;
class UDPSocket;

/**
 * @class UDPInternalConnection
 * @brief Реализация UDP соединения.
 */
class UDPInternalConnection : public InternalConnection
{
  std::unique_ptr<UDPSocket> socket_;
  SocketAddress host_address_;
public:

  UDPInternalConnection(const HostAddress& address
                        , uint16_t port = 0
                                          , bool isIPv6 = false);
  UDPInternalConnection(UDPSocket* socket);
  virtual ~UDPInternalConnection();

  virtual SocketDescr getFd();

  virtual void sendImpl(const ByteArray& data);
  virtual ByteArray receiveImpl(unsigned long size = 0);
  virtual void notifyReceive();
  virtual void notifyTimeout();
  virtual void notifyDisconnect();
  virtual void appendBuffer(const ByteArray& data);
  virtual ByteArray readBuffer(unsigned long size = 0);
  unsigned long readyForRead();
  virtual void reconnect();
};

}
#endif //UDPINTERNALCONNECTION_H

