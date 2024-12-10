#ifndef FTPSERVER_H_
#define FTPSERVER_H_

#include "ConnectionHandler.h"
#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>

#define LIST 1
#define UPLOAD 2
#define DOWNLOAD 3 
#define QUIT 4

namespace ftp {

    class FTP_Handler : public pr::ConnectionHandler {
        const char *dirname;
        public:
        FTP_Handler(const char *dirname):dirname(dirname) {}
        void handleConnection(pr::Socket s) override;
        ConnectionHandler * clone() const override;

        private:
        void send(const char *content, size_t sz);
        char *read(size_t sz);
        char *list();
        char *upload(size_t sz);
        void download(const char *file_name);
        void send_error(uint8_t error);
    };

}

#endif