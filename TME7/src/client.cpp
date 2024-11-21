#include <iostream>
#include <sys/mman.h>
#include <wait.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <string.h>
#include "chat_common.h"

int finished = 0;

void handler(int sig) {
    finished = 1;
}

struct argRead {
    struct message *recep;
    sem_t *mtx;
};

struct argSend {
    struct message*send;
    std::string &nameRecep;
};

void *createMessage(const std::string &name) {
    int fd = shm_open(name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
    ftruncate(fd, sizeof(struct message));
    void *res =  mmap(0, sizeof(struct message), PROT_READ | PROT_WRITE, O_CREAT | O_EXCL | O_RDWR, fd, 0);
    close(fd);
    return res;
}

void readMessage(struct message *recep, sem_t *mtx) {
    while(!finished) {
        sem_wait(mtx);
        if (recep->type != -1) {
            sem_post(mtx);
            sleep(1);
            continue;
        }
        std::cout << "Reçu : " << recep->content << std::endl;
        recep->type = 0;
        sem_post(mtx);
    }
}

int getInput() {
    std::string input;
    std::cout << "entrez : 1 (connection) | 2 (message) | 3 (deconnection)" << std::endl;
    std::cin >> input;
    return atoi(input.c_str());
}

void sendMessage(struct message *send) {
    size_t sz = sizeof(struct myshm);
    void *serv_ = openMemory("/serv", sz);
    struct myshm *serv = (struct myshm *)serv_;
    sem_wait(&(serv->semFree));
    sem_wait(&(serv->mtx));
    struct message* dst = serv->messages + (serv->write++);
    dst->type = send->type;
    memcpy(dst->content, send->content, 10*sizeof(10));
    sem_post(&(serv->mtx));
    sem_post(&(serv->semBusy));
    memset(send->content, 0, 10 * sizeof(char));
    munmap(serv_, sz);
}

void treatInput(struct message *send, std::string &nameRecep) {
    while(!finished) {
        int input = getInput();
        std::string content;
        switch(input) {
            case 1:
                send->type = 1;
                memcpy(send->content, nameRecep.data(), 10*sizeof(char));
                sendMessage(send);
                break;
            case 2:
                std::cout << "get your mess (10 max) : ";
                std::getline(std::cin, content);
                send->type = 2;
                memcpy(send->content, content.c_str(), 10*sizeof(char));
                sendMessage(send);
                break;
            case 3:
                send->type = 3;
                memcpy(send->content, nameRecep.data(), 10*sizeof(char));
                sendMessage(send);
                break;
            default :
                break;
        }
    }
}

void *thread_sender(void *arg_) {
    struct argSend *arg = (struct argSend *) arg_;
    treatInput(arg->send, arg->nameRecep);
    return NULL;
}

void *thread_read(void *arg_) {
    struct argRead *arg = (struct argRead *) arg_;
    readMessage(arg->recep, arg->mtx);
    return NULL;
}


int main(int argc, char const *argv[]){
    struct sigaction act = {0};
    act.sa_handler = handler;
    sigaction(SIGINT, &act, NULL);
    std::string nameRecep(argv[1]);
    std::string nameSend(argv[2]);
    std::string semName = nameRecep + "sem";
    void *recep_ = createMessage(nameRecep);
    void *send_ = createMessage(nameSend);
    sem_t *mtx = sem_open(semName.c_str(), O_RDWR | O_CREAT | O_EXCL, 0666, 1);
    struct argSend argS = {(struct message *)send_, nameRecep};
    struct argRead argR = {(struct message*)recep_, mtx};
    pthread_t sender;
    pthread_t receiver;
    pthread_create(&sender, NULL, thread_sender, &argS);
    pthread_create(&receiver, NULL, thread_read, &argR);    

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    sem_unlink(semName.c_str());
    munmap(recep_, sizeof(struct message));
    munmap(send_, sizeof(struct message));

    return 0;
}
