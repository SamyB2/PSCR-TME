#include <netinet/ip.h>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unistd.h>



#include "Socket.h"

/*
    man 7 ip
    struct sockaddr_in {
        sa_family_t sin_family;
        in_port_t sin_port;
        struct in_addr_t sin_addr;
    }

    struct in_addr {
        uint32_t s_addr;
    }

    INADDR_ANT -> toute interface de la machine 0.0.0.0
    INADDR_LOOPBACK -> localhost
    INADDR_BROADCAST -> udp brdcst ff.ff.ff.ff

*/

std::ostream &pr::operator<<(std::ostream &os, struct sockaddr_in *addr) {
    char host[2048];
    if (!getnameinfo((sockaddr *) addr, sizeof(struct sockaddr_in), host, 2048, NULL, 0, 0)) {
        os << "host => " << host << std::endl;
        os << "addr => " << inet_ntoa(addr->sin_addr) << std::endl;
        os << "port => " << ntohs(addr->sin_port) << std::endl;
    }
    return os;

}

void pr::Socket::connect(in_addr ipv4, int port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = ipv4;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("panic socket");
        return;
    }

    if (::connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("panic connect");
        close();
        return;
    }
}


void pr::Socket::connect(const std::string & host, int port) {
    struct addrinfo *res;
    if (getaddrinfo(host.c_str(), NULL, NULL, &res)) {
        perror("panic");
        return;
    }
        struct in_addr ip;
    for (auto rp = res; !rp; rp = rp->ai_next) {
        if (rp->ai_family == AF_INET) {
            ip = ((struct sockaddr_in *)(rp->ai_addr))->sin_addr;
            break;
        }
    }
    freeaddrinfo(res);
    connect(ip, port);
}

void pr::Socket::close() {
    if (fd == -1) {
        shutdown(fd, 2);
        ::close(fd);
        fd = -1;
    }
}