#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>

class HostAddress
{
public:
    HostAddress(int port = 0);
    HostAddress(const char* ip, int port);
    HostAddress(const sockaddr_in& addr);
    ~HostAddress(void);

    const char* ip(void) const;
    int port(void) const;

    sockaddr_in* _sockaddr(void) const
    { return (sockaddr_in*)&m_addr; }

private:
    mutable char m_ipBuff[32];
    sockaddr_in m_addr;
};


class Socket
{
public:
    enum {
        IOAgain = -1,
        IOError = -2
    };

    Socket(socket_t sock = -1);
    Socket(void);

    static Socket createSocket(void);

    socket_t getSocket(void) const { return m_socket; }

    bool bind(const HostAddress& addr);
    bool listen(int backlog);

    int option(int level, int name, char* val, socketlen_t* vallen);
    int setOption(int level, int name, char* val, socketlen_t vallen);

    bool setNonBlocking(void);
    bool setReuseaddr(void);
    bool setNoDelay(void);
    bool setKeepAlive(void);
    bool setSendBufferSize(int size);
    bool setRecvBufferSize(int size);
    bool setSendTimeout(int msec);
    bool setRecvTimeout(int msec);

    int sendBufferSize(void);
    int recvBufferSize(void);

    bool connect(const HostAddress& addr);

    //Blocking
    int send(const char *buf, int len, int flags = 0) {
        return ::send(m_socket, buf, len, flags);
    }
    int recv(char *buf, int len, int flags = 0) {
        return ::recv(m_socket, buf, len, flags);
    }

    //NonBlocking
    int nonblocking_send(const char* buff, int size, int flag = 0);
    int nonblocking_recv(char* buff, int size, int flag = 0);

    void close(void);
    bool isNull(void) const;

    static void close(socket_t sock);

private:
    socket_t m_socket;
};


#endif