#ifndef FTPSERVER_H_
#define FTPSERVER_H_

#include "ConnectionHandler.h"
#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

#define LIST 1
#define UPLOAD 2
#define DOWNLOAD 3 
#define QUIT 4

#define ERROR_DOWNLOAD 0

namespace ftp {

    class FTP_Handler : public pr::ConnectionHandler {
        const char *dirname;
        pr::Socket client;
        public:
        FTP_Handler(const char *dirname, pr::Socket client):dirname(dirname), client(client) {}
        void handleConnection(pr::Socket s) override;
        ConnectionHandler * clone() const override;

        private:
        void send(const void *content, size_t sz);
        char *read(size_t sz);
        char *list();
        void upload(int fd);
        void download(const char *file_name);
        void send_error(uint32_t error);
    };

}

#endif