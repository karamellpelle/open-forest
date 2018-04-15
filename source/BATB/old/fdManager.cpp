#include <algorithm>

#ifdef WIN32
#  include <windows.h>
#  include <time.h> 
#  include <io.h>
#else
#  include <sys/time.h> 
#  include <netinet/in.h> 
#  include <sys/socket.h> 
#  include <sys/wait.h> 
#endif

#include "fdManager.h"

#if 0
#define FD_DEBUG
#endif

using std::max;


/* Set all the file descriptors we are managing
 * into the fd_set
 */
int FdManager::setFDs(fd_set* fds)
{
    int biggest = 0; // biggest sockfd seen so far

    FD_ZERO(fds);
    Handlers::iterator i;
    for (i = handlers.begin(); i != handlers.end(); ++i) 
    {
        int sockfd = i->first;
        FD_SET(sockfd, fds);
        biggest = max(biggest, sockfd);
    }

    return biggest;
}

/* Add a file descriptor to be managed */
void FdManager::add(int sockfd, SocketHandler* sh)
{
    handlers[sockfd] = sh;
}

void FdManager::remove(int sockfd)
{
    handlers.erase(sockfd);
}

void FdManager::print(fd_set* fds)
{
    if (handlers.size() > 0)
    {
	std::cout << "Current sockets:\n";
	Handlers::iterator i;
	for (i = handlers.begin(); i != handlers.end(); ++i) 
	{
	    int sockfd = i->first;
	    std::cout << sockfd;
	    if (FD_ISSET(sockfd, fds))
	    {
		std::cout << " *";
	    }
	    std::cout << "\n";
	}
    }
}

/* loop through all fds in fdManager
 * if fd is set, call action procedure
 */
void FdManager::notify(fd_set* fds)
{
#ifdef FD_DEBUG
    print(fds);
#endif
    Handlers::iterator i;
    for (i = handlers.begin(); i != handlers.end(); )
    {
	int sockfd = i->first;
	if (FD_ISSET(sockfd, fds))
	{
	    SocketHandler* sl = i->second;
	    bool delHandler = sl->handle(sockfd);
	    if (delHandler)
	    {
		handlers.erase(i++);
	    }
	    else
	    {
		++i;
	    }
	}
	else
	{
	    ++i;
	}
    }
}
