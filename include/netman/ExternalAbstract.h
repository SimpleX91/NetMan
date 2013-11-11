#ifndef EXTERNALABSTRACT
#define EXTERNALABSTRACT
#include "HostAddress.h"

namespace netman
{

enum ConnStates {
  Idle = 0,
  Pending,
  TimedOut,
  Disconnected
};



/**
 * @class ExternalAbstract
 * @brief Является базовым классом для интерфейса пользователя сетевого модуля
 * как для соединения, так и для сервера.
 * @details Содержит в себе общие поля и методы для пользвательского класса
 * соединения и сервера
 */
class ExternalAbstract
{

  bool IPv6 = false;
  unsigned long timeout = 0;
protected:

public:

  void setTimeout(unsigned long ms_timeout) {
    timeout = ms_timeout;
  }
  unsigned long get_timeout() const {
    return timeout;
  }

};

}



#endif
