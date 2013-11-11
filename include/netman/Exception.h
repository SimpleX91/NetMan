#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <cstring>
#include <cstdio>
#include <typeinfo>

namespace netman
{

/**
 * @brief Базовый класс исключений модуля NetworkManager
 */
class Exception : public std::exception
{
public:
  Exception() {}
  Exception(const char* brief, const char* details = nullptr) {
    size_t size = 0;

    if (brief != nullptr) {
      size = strlen(brief) + 1;
      brief_ = new char[size];
      strncpy(brief_, brief, size);
    }

    if (details != nullptr) {
      size = strlen(details) + 1;
      details_ = new char[size];
      strncpy(details_, details, size);
    }
  }

  Exception(const Exception& obj) : Exception(obj.brief_, obj.details_) {}

  ~Exception() noexcept {
    delete [] brief_;
    delete [] details_;
    delete [] what_;
  }

  virtual const char* what() const noexcept {
    return typeid(this).name();
  }

  inline const char* brief() const noexcept {
    return (brief_ == nullptr) ? "no_brief" : brief_;
  }

  inline const char* details() const noexcept {
    return (details_ == nullptr) ? "no_details" : details_;
  }
private:
  char* brief_ = nullptr;
  char* details_ = nullptr;

  char* what_ = nullptr;
};

class SockConnectionError : public Exception
{
public:
  SockConnectionError(const char* brief, const char* details = nullptr)
    : Exception(brief, details) {}
};

class SockSendError : public Exception
{
public:
  SockSendError(const char* brief, const char* details = nullptr)
    : Exception(brief, details) {}
};

class SockListenError : public Exception
{
public:
  SockListenError(const char* brief, const char* details = nullptr)
    : Exception(brief, details) {}
};

class SockAcceptFailed : public Exception
{
public:
  SockAcceptFailed(const char* brief, const char* details = nullptr)
    : Exception(brief, details) {}
};

class SockInitError : public Exception
{
public:
  SockInitError(const char* brief, const char* details = nullptr)
    : Exception(brief, details) {}
};

class SockRecieveFailed : public Exception
{
public:
  SockRecieveFailed(const char* brief, const char* details = nullptr)
    : Exception(brief, details) {}
};

class SockConnectionLost : public Exception
{
public:
  SockConnectionLost(const char* brief, const char* details = nullptr)
    : Exception(brief, details) {}
};

class SockGenericError : public Exception
{
public:
  SockGenericError(const char* brief, const char* details = nullptr)
    : Exception(brief, details) {}
};

class ServerAcceptTimeout : public Exception
{
public:
  ServerAcceptTimeout(const char* brief, const char* details = nullptr)
    : Exception(brief, details) {}
};

}//NetworkManager

#endif // EXCEPTION_H
