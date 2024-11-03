#include "Semaphore.h"
#include <thread>
#include <vector>
#include <random>

// TODO : classe à modifier
class Data {
	std::vector<int> values;
	mutable pr::Semaphore readers;
	mutable pr::Semaphore writers;

public :
	Data() : readers(256), writers(1) {}

	int read() const {
		readers.acquire(1);
		int res = -1;
		if (values.empty()) res = 0;
		else
			res = values[rand()%values.size()];
		readers.release(1);
		return res;
	}
	void write() {
		writers.acquire(1);
		values.push_back(rand());
		writers.release(1);
	}
};

// Pas de modifications dans la suite.
void worker(Data & data) {
	for (int i=0; i < 20 ; i++) {
		auto r = ::rand() % 1000 ; // 0 to 1 sec
		std::this_thread::sleep_for (std::chrono::milliseconds(r));
		if (r % 2)
			auto lu = data.read();
		else
			data.write();
	}
}

int main () {
	// a faire varier
	const int NBTHREAD=10;

	// le data partagé
	Data d;

	std::vector<std::thread> threads;
	for (int i=0; i < NBTHREAD; i++)
		threads.emplace_back(worker,std::ref(d));

	for (auto & t: threads)
		t.join();
	return 0;
}

