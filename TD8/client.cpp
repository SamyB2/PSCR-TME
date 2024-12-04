#include "Socket.h"
#include <iostream>
#include <unistd.h>
using namespace pr;

int main(int argc, char const *argv[]){
    const char *host = argv[1];
    const int port = atoi(argv[2]);
    Socket sock;
    sock.connect(host, port);
    if (!sock.isOpen()) {
        std::cout << "error connect" << std::endl;
        return EXIT_FAILURE;
    }

    int value = 42;

    // write(sock.getFD(), &value, sizeof(int));
    send(sock.getFD(), &value, sizeof(int), 0);

    // read(sock.getFD(), &value, sizeof(int));
    recv(sock.getFD(), &value, sizeof(int), 0);

    std::cout << "received => " << value << std::endl;

    return 0;
}
