#include "ServerSocket.h"
#include <iostream>
using namespace pr;

int main(int argc, char const *argv[]){
    const int port = atoi(argv[1]);
    ServerSocket serv(port);

    while(true) {
        Socket client = serv.accept();

        int msg;
        recv(client.getFD(), &msg, sizeof(int), 0);

        std::cout << "received server : " << msg << std::endl;

        msg = 1;
        send(client.getFD(), &msg, sizeof(int), 0);
        client.close();
    }
    serv.close();
    return 0;
}
