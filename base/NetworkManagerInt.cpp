#include "NetworkManagerInt.h"

namespace netman
{

NetworkManagerInt::NetworkManagerInt()
{
  nm_reactor = std::unique_ptr<Reactor>(new Reactor(this));
}

void NetworkManagerInt::setRead(InternalAbstract* this_conn)
{
  fdMap[this_conn->getFd()].conn = this_conn;
  nm_reactor->setReadEvent(this_conn->getFd());

  if (!nm_reactor->isStarted())
    nm_reactor->Start();
}

void NetworkManagerInt::setTimeout(InternalAbstract* this_conn, long ms_timeout)
{
  int timerfd;

  if ((timerfd = fdMap[this_conn->getFd()].timer) == 0) {
    int timerfd = nm_reactor->setTimer(ms_timeout, 1);
    timerMap[timerfd] = this_conn;
    fdMap[this_conn->getFd()].timer = timerfd;
  } else {
    nm_reactor->restartTimer(timerfd, ms_timeout, 1);
  }
}

void NetworkManagerInt::resetTimeout(InternalAbstract* this_conn)
{
  if (fdMap[this_conn->getFd()].timer == 0)
    fprintf(stderr, "Timer not exist");

  nm_reactor->stopTimer(fdMap[this_conn->getFd()].timer);
}

void NetworkManagerInt::onSocketEvent(int fd)
{
  if (fdMap.find(fd) == fdMap.end())
    fprintf(stderr, "Connection does not exists");
  else {
    fdMap[fd].conn->notifyReceive();
  }
}

void NetworkManagerInt::onTimerElapsed(int timer_id)
{
  if (timerMap.find(timer_id) == timerMap.end())
    fprintf(stderr, "Timer does not exists");
  else {
    timerMap[timer_id]->notifyTimeout();
  }
}

void NetworkManagerInt::onError(int fd)
{
}

void NetworkManagerInt::onPeerConnectionClosed(int fd)
{
  if (fdMap.find(fd) == fdMap.end())
    fprintf(stderr, "Peer : Connection does not exists");
  else {
    nm_reactor->resetReadEvent(fd);
    fdMap[fd].conn->notifyDisconnect();
  }
}






}
