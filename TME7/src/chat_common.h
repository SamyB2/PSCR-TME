#ifndef H_CHAT_COMMON
#define H_CHAT_COMMON

#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* Pour les constantes des modes */
#include <fcntl.h>           /* Pour les constantes O_* */
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

#include <string.h>

#define MAX_MESS 50
#define MAX_USERS 10
#define TAILLE_MESS 10

struct message {
  long type;
  char content[TAILLE_MESS];
}message;

struct myshm {
  int read; /* nombre de messages retransmis par le serveur */
  int write; /* nombre de messages non encore retransmis par le serveur */
  sem_t mtx;
  sem_t semFree;
  sem_t semBusy;
  struct message messages[MAX_MESS];
};

struct myshm *createMyshm(char *name) {
	int fd = shm_open("/serverID", O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd < 0) {
        perror("error fd");
        exit(1);
    }
    ftruncate(fd, sizeof(struct myshm));
    void *shm = mmap(0, sizeof(message), PROT_READ | PROT_WRITE, O_CREAT | O_EXCL | O_RDWR, fd, 0);
    close(fd);
	struct myshm* shmServ = (struct myshm *) shm;
	sem_init(&(shmServ->mtx), 1, 1);
	sem_init(&(shmServ->semFree), 1, MAX_MESS);
	sem_init(&(shmServ->semBusy), 1, 0);
	return shmServ;
}

char *getName(char *name);

#endif
