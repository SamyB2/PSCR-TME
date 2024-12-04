#ifndef SRC_SERVERSOCKET_H_
#define SRC_SERVERSOCKET_H_

#include "Socket.h"

namespace pr {

class ServerSocket {
	int socketfd;

public :
	// Demarre l'ecoute sur le port donne
	ServerSocket(int port){
		socketfd = -1;
    	struct sockaddr_in addr;
    	addr.sin_family = AF_INET;
    	addr.sin_port = htons(port);
    	addr.sin_addr.s_addr = INADDR_ANY;

    	socketfd = socket(AF_INET, SOCK_STREAM, 0);

    	if (socketfd == -1) {
    	    perror("panic");
    	    return;
    	}

    	if (bind(socketfd, (struct sockaddr *) &addr, sizeof(addr))) {
    	    perror("panic bind");
    	    exit_:
    	    ::close(socketfd);
    	    socketfd = -1;
    	    return;
    	}

    	if (listen(socketfd, 10) < 0) {
    	    perror("panic listen");
    	    goto exit_;
    	}
	}

	int getFD() { return socketfd;}
	bool isOpen() const {return socketfd != -1;}

	Socket accept() {
		if (!isOpen()) return Socket();
    	struct sockaddr_in client;
    	socklen_t len = sizeof(client);
    	int fd_c = ::accept(socketfd, (struct sockaddr *)&client, &len);
    	if (fd_c < 0) {
    	    perror("panic accept");
    	    return Socket();
    	}
    	std::cout << "Connection : " << &client << std::endl;
    	return Socket(fd_c);
	}

	void close() {
		if (socketfd != -1) {
			shutdown(socketfd, 2);
			::close(socketfd);
			socketfd = -1;
		}
	}
};

} // ns pr
#endif /* SRC_SERVERSOCKET_H_ */
