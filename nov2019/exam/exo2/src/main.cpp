#include "Semaphore.h"
#include <iostream>
#include <thread>

void pinger(int n, pr::Semaphore &doPing, pr::Semaphore &doPong) {
	for (int i=0; i < n ; i++) {
		doPing.acquire(1);
		std::cout << "ping ";
		doPong.release(1);
	}
}

void ponger(int n, pr::Semaphore &doPing, pr::Semaphore &doPong) {
	for (int i=0; i < n ; i++) {
		doPong.acquire(1);
		std::cout << "pong ";
		doPing.release(1);
	}
}

int main () {
	// a faire varier si on le souhaite
	const int NBITER = 5;
	pr::Semaphore doPing(1);
	pr::Semaphore doPong(0);

	// TODO : instancier un thread pinger et un thread ponger avec n=NBITER
	std::thread ping(pinger, NBITER, std::ref(doPing), std::ref(doPong));
	std::thread pong(ponger, NBITER, std::ref(doPing), std::ref(doPong));

	// TODO : terminaison et sortie propre
	ping.join();
	pong.join();

	return 0;
}

