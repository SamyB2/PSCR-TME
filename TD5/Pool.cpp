#include "Job.h"
#include "Queue.h"
#include <vector>

class Pool {
    Queue<pr::Job> queue;
    std::vector<std::thread> workers;

    public:
    Pool(int qsize): queue(qsize) {}

    void start(int nbWorkers) {
        for (int i = 0; i < nbWorkers; i++) {
            workers.emplace_back(poolWorker, std::ref(queue));
        }
    }
    
    void submit(pr::Job * job) {
        queue.push(job);
    }

    void stop() {
        queue.setBlocking(false);
        for (auto & worker : workers) {
            worker.join();
        }
        workers.clear();
    }

};

void poolWorker (Queue<pr::Job> & queue) {
    while (true) {
        pr::Job * job = queue.pop();
        if (job == nullptr) return;
        job->run();
        delete job;
    }
}