#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#  include <windows.h>
#  include <TIME.H> 
#  include <io.h>
#else
#  include <unistd.h>
#  include <sys/time.h> 
#  include <netinet/in.h> 
#  include <sys/socket.h> 
#  include <sys/wait.h> 
#  include <arpa/inet.h>
#  include <netdb.h> 
#endif

#include "util.h"
#include "md5.h"
#include "paint.h"


#define	INET_ADDRSTRLEN	16 /* "ddd.ddd.ddd.ddd\0"
                               1234567890123456 */

/* this is adapted from 
 * UNIX Network Programming, Volume 1
 * program lsif01
 */

/* This loops through the different interfaces
 * and returns the first AF_INET address that is not
 * the loopback address (127.0.0.1). If you have multiple
 * interfaces, this may not be what you want.
 */
#if 0
char* getMyIP(void)
{
    int sockfd, len;
    char *ptr, buf[2048];
    static char addrstr[INET_ADDRSTRLEN];
    struct ifconf ifc;
    struct ifreq *ifr;
    struct sockaddr_in *sinptr;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_req = (struct ifreq *) buf;
    ioctl(sockfd, SIOCGIFCONF, &ifc);

    for (ptr = buf; ptr < buf + ifc.ifc_len;)
    {
        ifr = (struct ifreq *) ptr;
        len = sizeof(struct sockaddr);
#ifdef HAVE_SOCKADDR_SA_LEN
        if (ifr->ifr_addr.sa_len > len)
            len = ifr->ifr_addr.sa_len;         /* length > 16 */
#endif
        ptr += sizeof(ifr->ifr_name) + len;     /* for next one in buffer */

        if (ifr->ifr_addr.sa_family == AF_INET)
	{
            sinptr = (struct sockaddr_in *) &ifr->ifr_addr;
	    inet_ntop(AF_INET, &sinptr->sin_addr, addrstr, sizeof(addrstr));
	    if (strcmp(addrstr, "127.0.0.1"))
		return addrstr;
	}
    }
    return NULL;
}
#endif

fptype myrandom(fptype min, fptype max)
{
    fptype diff = max-min;
#if 0
    return (rand() / (fptype)RAND_MAX) * diff + min;
#else
    return uni() * diff + min;
#endif
}

void myrandomSeed(unsigned int seed)
{
#if 0
    srand(seed);
#else
    seed_uni(seed);
#endif
}

std::string md5sum(const char* message)
{
    md5_state_t state;
    md5_byte_t digest[16];
    //char* hex_output = (char*)emalloc(16*2 + 1);
    char hex_output[16*2 + 1];
    int di;

    md5_init(&state);
    md5_append(&state, (const md5_byte_t *)message, strlen(message));
    md5_finish(&state, digest);
    for (di = 0; di < 16; ++di)
	sprintf(hex_output + di * 2, "%02x", digest[di]);
    return std::string(hex_output);
}
