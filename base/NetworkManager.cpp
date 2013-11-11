#include "NetworkManager.h"
#include "NetworkManagerInt.h"
#include "InternalConnection.h"
#include "UDPInternalConnection.h"
#include "TCPInternalConnection.h"
#include "InternalServer.h"

namespace netman
{

NetworkManager::NetworkManager()
  : nm_impl(new NetworkManagerInt())
{
}

NetworkManager::~NetworkManager()
{
}

Connection NetworkManager::TcpConnect(const std::string address
                                      , uint16_t port, bool ipv6)
{
  HostAddress addr(address, port);
  return TcpConnect(addr, ipv6);
}

Connection NetworkManager::TcpConnect(const HostAddress& address, bool ipv6)
{
  Connection newConn;
  newConn.conn_handler = std::make_shared<TCPInternalConnection>(address, ipv6);
  newConn.conn_handler->setNMHandler(nm_impl.get());
  nm_impl->setRead(newConn.conn_handler.get());
  return newConn;
}

Connection NetworkManager::UdpConnect(const std::string address
                                      , uint16_t port
                                      , uint16_t bind_port
                                      , bool ipv6)
{
  HostAddress addr(address, port);
  return UdpConnect(addr, bind_port, ipv6);
}

Connection NetworkManager::UdpConnect(const HostAddress& address
                                      , uint16_t bind_port
                                      , bool ipv6)
{
  Connection newConn;
  newConn.conn_handler = std::make_shared<UDPInternalConnection>(address
                                                                 , bind_port
                                                                 , ipv6);
  newConn.conn_handler->setNMHandler(nm_impl.get());
  nm_impl->setRead(newConn.conn_handler.get());
  return newConn;
}

Server NetworkManager::TcpServer(const HostAddress& address, bool isIPv6)
{
  Server newServer;
  newServer.server_handler = std::make_shared<TCPInternalServer>(address
                                                                 , isIPv6);
  newServer.server_handler->setNMHandler(nm_impl.get());
  nm_impl->setRead(newServer.server_handler.get());
  return newServer;
}

Server NetworkManager::TcpServerAny(uint16_t port, bool isIPv6)
{
  Server newServer;
  newServer.server_handler = std::make_shared<TCPInternalServer>(port, isIPv6);
  newServer.server_handler->setNMHandler(nm_impl.get());
  nm_impl->setRead(newServer.server_handler.get());
  return newServer;
}



}
