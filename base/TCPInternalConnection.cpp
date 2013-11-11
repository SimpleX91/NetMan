#include "TCPInternalConnection.h"
#include "HostAddress.h"
#include "ByteArray.h"
#include "SocketAddress.h"
#include "SocketAddressBuilder.h"
#include "assert.h"

namespace netman
{

TCPInternalConnection::TCPInternalConnection(const HostAddress& address,
                                             bool isIPv6)
  : socket(new TCPSocket(isIPv6))
{
  SocketAddressBuilder builder;
  setHostAddress(address);
  setIPv6(isIPv6);
  socket->connectToHost(builder.getAddress(address, isIPv6));
}

TCPInternalConnection::TCPInternalConnection(TCPSocket* socket)
  : socket(socket)
{
}

SocketDescr TCPInternalConnection::getFd()
{
  return socket->getSocket();
}

TCPInternalConnection::~TCPInternalConnection()
{
}

void TCPInternalConnection::sendImpl(const ByteArray& data)
{
  socket->sendData(data);
}

ByteArray TCPInternalConnection::receiveImpl(unsigned long size)
{
  if (readyForRead() == 0 || size > readyForRead()) {
    // Проверка, не запущена ли другая операция
    resetDone();

    if (isDisconnected())
      throw std::string("Disconnected");
    else if (isPending())
      throw std::string("Already pending");
    else
      setPending();

    transaction_size = size;

    std::unique_lock<std::mutex> lock(syncMutex);
    waitVar.wait(lock, [&] {return !isPending();});
    std::lock_guard<std::mutex> state_lock(state_mutex);

    if (isDone()) {
      if (isPending())
        setIdle();
    } else if (isTimedOut()) {
      setIdle();
      throw std::string("Operation timeout");
    } else if (isDisconnected()) {
      throw std::string("Disconnected");
    }
  }

  size = (size == 0) ? readyForRead() : size;
  return readBuffer(size);
}

void TCPInternalConnection::notifyReceive()
{
  std::lock_guard<std::mutex> state_lock(state_mutex);
  appendBuffer(socket->readAllData());

  if (isPending() && (readyForRead() >= transaction_size)) {
    setIdle();
    doDone();
    waitVar.notify_one();
  }
}

void TCPInternalConnection::notifyTimeout()
{
  std::lock_guard<std::mutex> state_lock(state_mutex);
  setTimedOut();
  waitVar.notify_one();
}

void TCPInternalConnection::notifyDisconnect()
{
  std::lock_guard<std::mutex> state_lock(state_mutex);
  setDisconnected();
  waitVar.notify_one();
}

ByteArray TCPInternalConnection::readBuffer(unsigned long size)
{
  std::lock_guard<std::mutex> lock(buf_mutex);
  assert(buffer.front().size() >= size && "Buffer hasn't enough data");
  ByteArray retBuf;
  retBuf = buffer.front().subtract(0, size);
  buffer.front().erase(0, size);

  if (buffer.front().size() == 0)
    buffer.pop();

  return retBuf;
}

unsigned long TCPInternalConnection::readyForRead()
{
  std::lock_guard<std::mutex> lock(buf_mutex);
  return (!buffer.empty()) ? buffer.front().size() : 0;
}

void TCPInternalConnection::reconnect()
{
  SocketAddressBuilder builder;
  socket->connectToHost(builder.getAddress(getHostAddress(), isIPv6()));
}

void TCPInternalConnection::appendBuffer(const ByteArray& data)
{
  std::lock_guard<std::mutex> lock(buf_mutex);

  if (buffer.empty())
    buffer.push(data);
  else
    buffer.front().append(data);
}

}// NetworkManager
