#ifndef _CLIENTOBSERVER_H
#define _CLIENTOBSERVER_H

class ClientObserver
{
 public:
    virtual ~ClientObserver() {}
    virtual int update(char** clientsList) = 0;

    struct clientsUpdate : public std::binary_function<ClientObserver*, void*, int>
    {
	int operator()(ClientObserver* observer, char** clientsList)
	{
	    return observer->update(clientsList);
	}
    };
};

#endif // _CLIENTOBSERVER_H
