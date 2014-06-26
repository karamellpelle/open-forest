#ifndef _FDMANAGER_H
#define _FDMANAGER_H

#include <map>
#include <algorithm>
#include <functional>

#include "util.h"

class SocketHandler
{
 public:
    virtual bool handle(int sockfd) { return false; }
    virtual ~SocketHandler() {}
};

class FdManager
{
 public:
    typedef std::map<int, SocketHandler*> Handlers;

    int setFDs(fd_set* fds);
    void add(int sockfd, SocketHandler* sl);
    void remove(int sockfd);
    void notify(fd_set* fds);

 private:
    Handlers handlers;
    void print(fd_set* fds);
};

#endif // _FDMANAGER_H
