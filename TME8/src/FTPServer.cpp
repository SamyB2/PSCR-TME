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
                upload(fd);
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

            case QUIT : {
                return;
            }

        }
    }
}

pr::ConnectionHandler * ftp::FTP_Handler::clone() const{
    return new FTP_Handler(dirname, client);
}

void ftp::FTP_Handler::send(const void *content, size_t sz) {
    size_t snd = 0;
    int fd = client.getFD();
    do {
        ssize_t snd_ = ::send(fd, content + snd, sz - snd, 0);
        if (snd_ < 0) {
            perror("panic sending data");
            return;
        }
        snd += (size_t) snd_;
    }while(snd < sz);
}

char *ftp::FTP_Handler::read(size_t sz) {
    char *content = (char *) malloc(sz * sizeof(char));
    int fd = client.getFD();
    size_t rd = 0;
    do {
        ssize_t rd_ = recv(fd, content + rd, sz - rd, 0);
        if (rd_ < 0) {
            perror("error reading data");
            return;
        }
        rd += (size_t) rd_;
    }while(rd < sz);
    return content;
}

char *ftp::FTP_Handler::list() {
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("error opening dir");
        return NULL;
    }

    struct dirent* entry;
    std::string file_list;
    while((entry = readdir(dir))) {
        file_list += entry->d_name;
        file_list += "\n";
    }

    closedir(dir);
    size_t len = file_list.length();
    char *file_list_ = (char *) malloc(len * sizeof(char));
    strcpy(file_list_, file_list.c_str());
    return file_list_;
}

void ftp::FTP_Handler::upload(int fd) {
    size_t sz;
    ssize_t rd = recv(fd, &sz, sizeof(size_t), 0);
    if (rd < 0 || rd < sizeof(size_t)) {
        perror("error reading size of file");
        return;
    }
    char *file = read(sz);
    sz = 0;
    ssize_t rd = recv(fd, &sz, sizeof(size_t), 0);
    if (rd < 0 || rd < sizeof(size_t)) {
        perror("error reading size of file name");
        return;
    }

    char *name = read(sz);
    int file_fd = open(name, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (file_fd < 0) {
        perror("error opening file ");
        return;
    }
    write(file_fd, file, sz);
    free(file);
    free(name);
    close(file_fd);
}

void ftp::FTP_Handler::download(const char *file_name) {
    int fd = open(file_name, O_RDONLY);
    if (fd < 0) {
        perror("error opening file");
        exit_error:
            send_error(ERROR_DOWNLOAD);
            return;
    }
    size_t sz = (size_t) lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char *content = (char *) malloc(sz * sizeof(char));
    size_t rd = 0;
    do {
        ssize_t rd_ = ::read(fd, content + rd, sz - rd);
        if (rd_ < 0) {
            perror("panic reading file");
            goto exit_error;
        }
        rd += (size_t) rd_;
    }while(rd < sz);

    uint32_t len = sz;
    send(&len, sizeof(uint32_t));
    send(content, sz);
}