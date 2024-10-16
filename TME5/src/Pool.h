#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>
#include <semaphore>

namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
	std::counting_semaphore<1000000L> & csmph;

public:
	Pool(size_t qsize, std::counting_semaphore<1000000L> & s): queue(qsize), csmph(s) {}

	void start (int nbthread) {
		for (int i = 0; i<nbthread; ++i) {
			threads.emplace_back(&poolWorker, std::ref(queue), csmph);
		}
	}

	void submit (Job * job) {
		queue.push(job);
	}

	void stop() {
		queue.setBlocking(false);
		for (auto & th : threads) 
			th.join();
		threads.clear();
	}
	~Pool() ;
};

void poolWorker (Queue<pr::Job> & queue, std::counting_semaphore<> &s) {
    while (true) {
        pr::Job * job = queue.pop();
        if (job == nullptr) return;
        job->run();
		s.release();
        delete job;
    }
}

}
