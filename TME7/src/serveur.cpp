#include <iostream>
#include <sys/mman.h>
#include <wait.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <string.h>
#include "chat_common.h"

void diffuse_message(const char *mess, std::vector<std::string> &idClients) {
    for (std::string &x : idClients) {
        std::string s("sem");
        int fd = shm_open(x.c_str(), O_RDWR, 0666);
        void *shm = mmap(0, sizeof(message), PROT_WRITE | PROT_READ, O_RDWR, fd, 0);
        close(fd);
        std::string semName = x + s;
        fd = shm_open(semName.c_str(), O_RDWR, 0666);
        void *shm2 = mmap(0, sizeof(message), PROT_WRITE | PROT_READ, O_RDWR, fd, 0);
        struct message *messClient = (struct message *)shm;
        sem_t *mtx = (sem_t *)shm2;
        sem_wait(mtx);
        memcpy(messClient->content, mess, sizeof(char) * 1024);
        messClient->type = -1;
        sem_post(mtx);
        munmap(shm2, sizeof(sem_t));
        munmap(shm, sizeof(struct message *));
    }
}

void treatMessage(struct message *mess, std::vector<std::string> &idClients) {
    switch (mess->type){
    case 1:
        idClients.emplace_back(mess->content);
        break;

    case 2: 
        diffuse_message(mess->content, idClients);
        break;

    case 3:
        for (auto i = idClients.begin(); i!=idClients.end(); i++) {
            if (!strcmp(mess->content, i->c_str())) {
                idClients.erase(i);
                break;
            }
        }
        break;
    default : return;
    }
    mess->type = -1;
}


int main(int argc, char const *argv[]){
    struct myshm* serv = createMyshm("/serv");
    std::vector<std::string> ids;

    while (true) {
        sem_wait(&(serv->semBusy));
        sem_wait(&(serv->mtx));
        int index = serv->read;
        serv->read = (serv->read + 1)%MAX_MESS;
        sem_post(&(serv->mtx));
        treatMessage(serv->messages + index, ids);
        sem_post(&(serv->semFree));
    }


    return 0;
}
