#include <iostream>
#include <thread>
#include <chrono>

namespace pr {

    class Barrier {
        int nb;

        public :
        void done() {

        }
        void waitFor() {

        }

    };

    class Job {
        public:
        virtual void run() = 0; // ne cherche pas, pas d'implem
        virtual ~Job() {};
    };

    class SleepJob : public Job {
        int calcul(int v) {
            using namespace std::chrono_literals;
            std::cout << "calcul en cours" << std::endl;
            std::this_thread::sleep_for(1s);
            int ret = v % 255;
            std::cout << "finished" << std::endl;
            return ret;
        }
        int arg;
        int * ret;
        public:
        void run() {
            *ret = calcul(arg);
        }

        ~SleepJob() {};
    };

    class BarrierJob : public Job {
        Barrier * barrier;
        int calcul(int v) {
            using namespace std::chrono_literals;
            std::cout << "calcul en cours" << std::endl;
            std::this_thread::sleep_for(1s);
            int ret = v % 255;
            std::cout << "finished" << std::endl;
            return ret;
        }
        public:
        void run() {
            /*code*/
        }

        ~BarrierJob() {};
    };

};