#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <semaphore.h>

/*
    Semaphore {
        int permits : >= 0

        sem_wait() => P => prend un permis bloque if none
        sem_post() => V => libere un permis + reveille
    }

    -> Construction : 
        -- sem nommée : sem_open
                        sem_close
                        sem_unlink
                nom de sem : /name tq len(name) < 512

*/

void pinger(sem_t *semPing, sem_t *semPong) {
    sem_wait(semPing);
    std::cout << "ping" << std::endl;
    sem_post(semPong);
}

void ponger(sem_t *semPing, sem_t *semPong) {
    sem_wait(semPong);
    std::cout << "pong" << std::endl;
    sem_post(semPing);
}

int main(int argc, char const *argv[]){
    sem_t *semPing = sem_open("ping", O_CREAT | O_RDWR | O_EXCL, 0666, 1);
    sem_t *semPong = sem_open("pong", O_CREAT | O_RDWR | O_EXCL, 0666, 0);
    pid_t pid1 = fork();

    if (!pid1) {
        while(true) ponger(semPing, semPong);
    }else{
        while(true) pinger(semPing, semPong);
    }
    return 0;
}
