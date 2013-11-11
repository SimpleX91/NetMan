#include "UDPInternalConnection.h"
#include "ByteArray.h"
#include "HostAddress.h"
#include "SocketAddress.h"
#include "SocketAddressBuilder.h"
#include "UDPSocket.h"
#include "assert.h"

namespace netman
{

UDPInternalConnection::UDPInternalConnection(const HostAddress& address
                                             , uint16_t port
                                             , bool isIPv6)
  : socket_(new UDPSocket(isIPv6))
{
  SocketAddressBuilder builder;
  host_address_ = builder.getAddress(address);

  if (port)
    socket_->bindTo(builder.getAddressAny(port, isIPv6));
}

UDPInternalConnection::UDPInternalConnection(UDPSocket* socket)
  : socket_(socket)
{
}

UDPInternalConnection::~UDPInternalConnection()
{
}

SocketDescr UDPInternalConnection::getFd()
{
  socket_->getSocket();
}

void UDPInternalConnection::sendImpl(const ByteArray& data)
{
  socket_->sendDataTo(data, host_address_);
}

ByteArray UDPInternalConnection::receiveImpl(unsigned long size)
{
  if (readyForRead() == 0) {

    if (!isPending())
      setPending();
    else
      throw std::string("Already pending");

    std::unique_lock<std::mutex> lock(syncMutex);
    waitVar.wait(lock, [&] {return !isPending();});

    if (isTimedOut()) {
      setIdle();
      throw std::string("Operation timeout");
    } else
      setIdle();
  }

  return readBuffer();
}

void UDPInternalConnection::notifyReceive()
{
  auto packet = socket_->readDataFrom(); //data:address

  //if (packet.second == this->host_address_) {
  // Если мы получили данные от хоста, с которым работаем - читаем пакет
  // Иначе - отбрасываем как некорректный
  appendBuffer(packet.first);

  if (isPending()) {
    setIdle();
    waitVar.notify_one();
  }

  //}
}

void UDPInternalConnection::notifyTimeout()
{
  setTimedOut();
  waitVar.notify_one();
}

void UDPInternalConnection::notifyDisconnect()
{
  assert(false && "No disconnect events for UDPConnection!");
}

void UDPInternalConnection::appendBuffer(const ByteArray& data)
{
  std::lock_guard<std::mutex> lock(buf_mutex);
  buffer.push(data);
}

ByteArray UDPInternalConnection::readBuffer(unsigned long size)
{
  std::lock_guard<std::mutex> lock(buf_mutex);

  assert(buffer.size() > 0 && "Buffer is empty!");

  ByteArray buf = buffer.front();
  buffer.pop();

  return buf;
}

unsigned long UDPInternalConnection::readyForRead()
{
  std::lock_guard<std::mutex> lock(buf_mutex);
  return buffer.size();
}

void UDPInternalConnection::reconnect()
{
}

}// NetworkManager
