#include "Reactor.h"
#include <sys/epoll.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include "assert.h"
#include "sys/timerfd.h"
#include "time.h"
#include <fcntl.h>

namespace netman
{

void Reactor::setCycleMsTime(itimerspec* value, long time_ms)
{
  struct timespec interval_value;
  interval_value.tv_sec = time_ms / 1000;
  interval_value.tv_nsec = (time_ms % 1000) * 1000000;
  value->it_interval = interval_value;
  value->it_value = interval_value;
}

Reactor::Reactor(ReactorInterface* callbackInterface)
  : callback_interface(callbackInterface)
{
  struct epoll_event ev;
  epollFd = epoll_create1(0);

  // Установка pipe для деструктора
  pipe(pipeFds);
  int flags = fcntl(pipeFds[Read], F_GETFL, 0);
  fcntl(pipeFds[Write], F_SETFL, flags | O_NONBLOCK);
  ev.events = EPOLLIN;
  ev.data.fd = pipeFds[Read];
  epoll_ctl(epollFd, EPOLL_CTL_ADD, pipeFds[Read], &ev);
}

Reactor::~Reactor()
{
  if (isStarted())
    Stop();

  close(epollFd);
}

bool Reactor::isStarted()
{
  return started;
}

void Reactor::Stop()
{
  stop_thread_mutex.unlock();
  char ch = '0';
  write(pipeFds[Write], &ch, 1);
  if (thread.joinable())
    thread.join();
}

void Reactor::Start()
{
  if (isStarted())
    Stop();

  stop_thread_mutex.lock();
  thread = std::thread(&Reactor::Execute, this);
}

void Reactor::setReadEvent(int fd, bool edgeTriggered)
{
  int action;
  struct epoll_event ev;

  if (fdSet.find(fd) == fdSet.end()) {
    action = EPOLL_CTL_ADD;
    fdSet.insert(fd);
  } else {
    action = EPOLL_CTL_MOD;
  }

  ev.events = EPOLLIN | EPOLLRDHUP;

  if (edgeTriggered)
    ev.events |= EPOLLET;

  ev.data.fd = fd;

  if (epoll_ctl(epollFd, action, fd, &ev) == -1) {
    std::cerr << "epoll_ctl error:" << strerror(errno) << std::endl;
  }

}

void Reactor::resetReadEvent(SocketDescr socketFd)
{
  struct epoll_event ev;

  if (fdSet.find(socketFd) == fdSet.end())
    return;

  ev.data.fd = socketFd;
  ev.events = 0;

  if (epoll_ctl(epollFd, EPOLL_CTL_DEL, socketFd, &ev) == -1) {
    std::cerr << "epoll_ctl error:" << strerror(errno) << std::endl;
  }

  fdSet.erase(socketFd);

}

int Reactor::setTimer(long time_ms, int count)
{
  struct itimerspec new_value;
  int timerFd = timerfd_create(CLOCK_MONOTONIC, 0);
  timerMap[timerFd] = count;

  setCycleMsTime(&new_value, time_ms);

  if (timerfd_settime(timerFd, 0, &new_value, NULL) == -1) {
    std::cerr << "set_time error:" << strerror(errno) << std::endl;
    return TIMER_SETTIME_ERROR;
  }

  setReadEvent(timerFd);
  return timerFd;
}

int Reactor::stopTimer(int timer_id)
{
  struct itimerspec cur_value;
  struct itimerspec new_value;

  if (timerMap.find(timer_id) == timerMap.end())
    return TIMER_NOT_EXISTS;

  timerfd_gettime(timer_id, &cur_value);

  if (cur_value.it_value.tv_sec == 0 && cur_value.it_value.tv_nsec == 0)
    return TIMER_ALREADY_IDLE;

  memset(&new_value, 0, sizeof(itimerspec));

  if (timerfd_settime(timer_id, 0, &new_value, NULL) == -1) {
    std::cerr << "epoll_ctl error:" << strerror(errno) << std::endl;
    return TIMER_SETTIME_ERROR;
  }

  resetReadEvent(timer_id);
  return 1;
}

int Reactor::restartTimer(int timer_id, long time_ms, int count)
{
  struct itimerspec cur_value;
  struct itimerspec new_value;

  if (timerMap.find(timer_id) == timerMap.end())
    return TIMER_NOT_EXISTS;

  timerfd_gettime(timer_id, &cur_value);

  if (cur_value.it_value.tv_sec != 0 || cur_value.it_value.tv_nsec != 0)
    return TIMER_NOT_IDLE;

  setCycleMsTime(&new_value, time_ms);

  if (timerfd_settime(timer_id, 0, &new_value, NULL) == -1) {
    std::cerr << "set_time error:" << strerror(errno) << std::endl;
    return TIMER_SETTIME_ERROR;
  }

  timerMap[timer_id] = count;
  setReadEvent(timer_id, true);

}

void Reactor::Execute()
{
  int nfds, fd, n;
  struct epoll_event ev;
  struct epoll_event* evlist;
  uint64_t timerValue;
  evlist = new struct epoll_event[MAX_EVENTS];
  started = true;

  while (stop_thread_mutex.try_lock() == false) {
    nfds = epoll_wait(epollFd, evlist, MAX_EVENTS, -1);

    if (nfds == -1) {
      std::cerr << "epoll_wait error: " << strerror(errno) << std::endl;
    }

    for (n = 0; n < nfds; n++) {
      fd = evlist[n].data.fd;

      if (evlist[n].events & EPOLLIN) {
        if (timerMap.find(fd) != timerMap.end()) {
          if (timerMap[fd]) {
            timerMap[fd]--;
            read(fd, &timerValue, 8);

            if (timerMap[fd] == 0)
              stopTimer(fd);
          }

          //std::cerr << "Timer event: " << timerValue << std::endl;
          callback_interface->onTimerElapsed(fd);
        } else if (fd == pipeFds[Read]) {
          char ch;
          int result = 1;
          while (result > 0)
            result = read(fd, &ch, 1);
        } else {
          assert(fdSet.find(fd) != fdSet.end() && "fdSet has not that fd");
          //std::cerr << "Read event!\n" << std::endl;
          int available;
          ioctl(fd, FIONREAD, &available);

          if (available)
            callback_interface->onSocketEvent(fd);
          else
            callback_interface->onPeerConnectionClosed(fd);
        }
      } else if (evlist[n].events & EPOLLRDHUP) {
        callback_interface->onPeerConnectionClosed(fd);
      } else if (evlist[n].events & EPOLLERR)
        callback_interface->onError(fd);

    }
  }

  started = false;
}

} // namespace netman
