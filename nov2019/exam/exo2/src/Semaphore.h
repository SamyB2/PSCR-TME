#pragma once
#include <mutex>
#include <condition_variable>

namespace pr {

class Semaphore {
	int compteur;
	std::mutex mtx;
	std::condition_variable dispo;

public :
	Semaphore(int initial):compteur(initial) {}
	void acquire(int qte) {
		std::unique_lock<std::mutex> l(mtx);
		while (compteur - qte < 0)
			dispo.wait(l);
		compteur-=qte;
	}
	void release(int qte) {
		std::unique_lock<std::mutex> l(mtx);
		compteur += qte;
		dispo.notify_all();
	}
};

}
