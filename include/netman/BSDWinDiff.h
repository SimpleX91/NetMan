#ifndef BSDWINDIFF_H
#define BSDWINDIFF_H

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#endif

namespace netman
{
#ifdef WIN32
typedef short SocketFamily;
typedef SOCKET SocketDescr;
typedef socklen_t int
#define SocketLastError() WSAGetLastError()
#else
typedef sa_family_t SocketFamily;
typedef int SocketDescr;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define closesocket(X) close(X)
#define ioctlsocket(X, Y, Z) ioctl(X, Y, Z)
#define strcpy_s(X, Y, Z) strncpy(X, Z, Y)
#define SocketLastError() errno
#endif

typedef struct USOCKADDR {
  SocketFamily family;
  unsigned short port;
} USockAddr;

}

#endif // BSDWINDIFF_H
