#include "TCPServer.h"

bool pr::TCPServer::startServer (int port) {
    ss = new ServerSocket(port);

    connections.emplace_back([&] () {
        while(run) {
            pr::Socket client = ss->accept();
            
            connections.emplace_back([&] () {
                handler->handleConnection(client);
                client.close();
            });
        }
        ss->close();
    });
    return true;
}

void pr::TCPServer::stopServer () {
    run = false;
    for (std::thread &x : connections)
        x.join();
}