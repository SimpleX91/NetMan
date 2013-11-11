#include "HostAddress.h"
#include <string>

namespace netman
{

HostAddress::HostAddress()
  : HostAddress("", 0)
{
}

HostAddress::HostAddress(const std::string& address, uint16_t port)
{
  this->host_ = address;
  this->port_ = port;
}

std::string HostAddress::getAddress() const
{
  return this->host_;
}

uint16_t HostAddress::getPort() const
{
  return this->port_;
}

void HostAddress::setAddress(const std::string& address)
{
  this->host_ = address;
}

void HostAddress::setPort(uint16_t port)
{
  this->port_ = port;
}

bool HostAddress::isEmpty()
{
  return (this->host_.empty() && !this->port_);
}

}// NetworkManager
