#include "NetworkManagerInt.h"

namespace netman
{

void InternalAbstract::setTimeout(unsigned long ms_timeout)
{
  nm_handler->setTimeout(this, ms_timeout);

}


void InternalAbstract::resetTimeout()
{
  nm_handler->resetTimeout(this);

}

}
