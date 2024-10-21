#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>
#include <barrier>

namespace pr {

void poolWorker (Queue<pr::Job> & queue, std::counting_semaphore<> &csmph) {
    while (true) {
        pr::Job * job = queue.pop();
        if (job == nullptr) return;
        job->run();
		csmph.release();
        delete job;
    }
}

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
	std::counting_semaphore<> &csmph_;

public:
	Pool(size_t qsize, std::counting_semaphore<> & csmph): queue(qsize), csmph_(csmph) {}

	void start (int nbthread) {
		for (int i = 0; i<nbthread; ++i) {
			threads.emplace_back(poolWorker, std::ref(queue), std::ref(csmph_));
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
	~Pool(){} ;
};



}
