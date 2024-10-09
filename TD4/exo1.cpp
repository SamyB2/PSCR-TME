#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <chrono>

void work(int index) {
    std::cout << "started " << index << std::endl;
    auto r = ::rand() % 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(r));
    std::cout <<"finished " << index << std::endl;
}

int create_and_wait(int N) {
    using namespace std;
    vector<thread> threads;
    threads.reserve(N);
    for (int i = 0; i < N; i++) {
        threads.emplace_back(work, i);
        std::cout << "created " << i << std::endl;
    }
    int i = 0;
    for (auto& t : threads) {
        t.join();
        std::cout << "joined " << i++ << std::endl;
    }
    return 0;
}

/*
    Relations de precedence : 
    -- started i -> finished i -> joined i
    -- created i -> joined i
    -- started i <-/-> started j
    -- started i <-/-> finished j => sont en concurrence
*/

int main(int argc, const char ** argv) {
    int N = 3;
    if (argc > 1) {
        N = ::atoi(argv[1]);
    }
    ::srand(::time(nullptr));
    return create_and_wait(N);
}