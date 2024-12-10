#include "FTPServer.h"

void ftp::FTP_Handler::handleConnection(pr::Socket client) {
    while (true) {
        uint8_t action = -1;
        int fd = client.getFD();

        ssize_t rd = recv(fd, &action, sizeof(uint8_t), 0);

        switch (rd){
            case 0: return;
            
            case -1 :
                perror("error reading action");
                return;

            case LIST : {
                char *list_files = list();
                size_t len = strlen(list_files);
                send(list_files, len);
                free(list_files);
                break;
            }

            case UPLOAD : {
                size_t sz;
                ssize_t rd = recv(fd, &sz, sizeof(size_t), 0);
                if (rd < 0 || rd < sizeof(size_t)) {
                    perror("error reading size of file");
                    return;
                }
                char *file = upload(sz);
                sz = 0;
                ssize_t rd = recv(fd, &sz, sizeof(size_t), 0);
                if (rd < 0 || rd < sizeof(size_t)) {
                    perror("error reading size of file name");
                    return;
                }

                char *name = read(sz);
                int file_fd = open(name, O_CREAT | O_WRONLY | O_APPEND, 0666);
                write(file_fd, file, sz);
                free(file);
                free(name);
                close(file_fd);
                break;
            }
        
            case DOWNLOAD : {
                size_t sz;
                ssize_t rd = recv(fd, &sz, sizeof(size_t), 0);
                if (rd < 0 || rd < sizeof(size_t)) {
                    perror("error reading size of file");
                    return;
                }
                char *file_name = read(sz);

                download(file_name);
                free(file_name);
            }

        }
    }
}