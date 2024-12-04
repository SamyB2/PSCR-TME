#include "ServerSocket.h"
#include <vector>
#include <thread>

using namespace pr;

class TCPServer {
    ServerSocket ss;
    bool run;
    std::vector<std::thread> connections;

    public :

    TCPServer(int port):ss(port),run(false) {}

    ~TCPServer() {
        for (std::thread &x : connections) {
            x.join();
        }
    }

    virtual void handleClient(Socket client);

    bool startServer() {
        connections.emplace_back([&] () {
            while(run) {
                Socket client = ss.accept();
                // gestion error
                connections.emplace_back(TCPServer::handleClient, this, client);
            }
        });
        return true;
    }

    bool endServer() {
        run = false;
    }
};

