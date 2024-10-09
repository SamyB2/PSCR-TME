#include "Banque.h"
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <iostream>

using namespace std;
using namespace pr;

const int NB_THREAD = 10;
const int MONEY = 1000;
const int NB_COMPTES = 5;

void behave_q1(Banque & b) {
	for (int i = 0; i < 10000; ++i) {
		int c1 = rand() % b.size();
		int c2 = rand() % b.size();
		int value = 1 + (rand() % 99);
		b.transfert(c1, c2, value);
		sleep(0.2);
	}
}

void comptable(Banque & b) {
	b.comptabiliser(NB_COMPTES * MONEY);
}

int main () {
	vector<thread> threads;
	Banque b(NB_COMPTES, MONEY);
	
	// thread creation
	threads.reserve(NB_THREAD);

	// solde total
	cout << "Solde total : " << b.size() * MONEY << endl;

	for (int i = 0; i < NB_THREAD; ++i) {
		threads.emplace_back(behave_q1, ref(b));
	}

	// thread comptable
	threads.emplace_back(comptable, ref(b));

	for (auto & t : threads) {
		t.join();
	}

	// bilan comptable
	b.comptabiliser(NB_COMPTES * MONEY);

	return 0;
}
