#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <semaphore.h>
#include <sys/mman.h>
#include <wait.h>

#include "Stack.h"
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


template <typename T>
pr::Stack<T> *openStack(const char *name) {
    int fd = shm_open(name, O_RDWR);
    void *smh = mmap(0, sizeof(pr::Stack<T>), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return (pr::Stack<T> *) smh;
}

template <typename T>
void closeStack(pr::Stack<T> *stack) {
    munmap(stack, sizeof(pr::Stack<T>));
}

void deleteStack(const char *name) {
    unlink(name);
}


#define N 5

/*
    sem nommé pas besoin sur mem partagee
    sem anonyme => mem partagé
*/

int main(int argc, char const *argv[]){
    void * shm = mmap(0, sizeof(sem_t) * N,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_t *semTab = (sem_t *) shm;
    for(int i = 0; i<N; i++) {
        int semVal = (!i)?1:0;
        sem_init(semTab + i, 1, semVal);
    }
    for(int i = 0; i<N; ++i) {
        if(!fork()) {
            for (int k = 0; i<10; k++) {
                sem_wait(semTab+i);
                std::cout << "ping" << std::endl;
                sem_post(semTab + (i + 1)%N);

            }
            munmap(shm, sizeof(sem_t) * N);
            exit(0);
        }
    }

    for (int i = 0; i<N; i++) wait(NULL);
    for (int i = 0; i<N; i++) 
        sem_destroy(semTab + i);
    munmap(shm, sizeof(sem_t) * N);
    return 0;
}
