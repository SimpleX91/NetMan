#include "InternalConnection.h"
#include <mutex>
#include <assert.h>

namespace netman
{

ByteArrayUnion InternalConnection::readAllBuffer()
{
  std::lock_guard<std::mutex> lock(buf_mutex);
  ByteArrayUnion retBuf;

  while (!buffer.empty()) {
    retBuf.push_back(buffer.front());
    buffer.pop();
  }

  return retBuf;
}

}// NetworkManager
