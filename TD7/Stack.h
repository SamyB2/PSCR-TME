#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>
#include <fcntl.h>
#include <atomic>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab[STACKSIZE];
	std::atomic<size_t> sz;
	sem_t mtx;
	sem_t semFree;
	sem_t semBusy;

public :
	Stack () : sz(0) {
		sem_init(&mtx, 1, 1); 
		sem_init(&semFree, 1, STACKSIZE);
		sem_init(&semBusy, 1, 0);
	}

	~Stack () {
		sem_destroy(&mtx)
		sem_destroy(&semFree);
		sem_destroy(&semBusy);
	}

	T pop () {
		// bloquer si vide
		sem_wait(&semBusy);
		sem_wait(&mtx)
		T toret = tab[--sz];
		sem_post(&mtx);
		sem_post(&semFree);
		return toret;
	}

	void push(T elt) {
		//bloquer si plein
		sem_wait(&semFree);
		sem_wait(&mtx);
		tab[sz++] = elt;
		sem_post(&mtx);
		sem_post(&semBusy);
	}
};

}
