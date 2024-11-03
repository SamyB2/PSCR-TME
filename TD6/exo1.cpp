#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>

const int N = 5;

void f1 (int id) {
    std::cout << "mon pid : " << getpid() << std::endl;
    std::cout << "pid pere : " << getppid() << std::endl;
    std::cout << "mon id : " << id << std::endl << std::endl;
}

int main() {
    std::vector<pid_t> childs;
    for (int i = 0; i<N ; ++i) {
        pid_t pid = fork();
        if (!pid) {
            f1(i);
        }
        if (pid < 0) return EXIT_FAILURE;
        childs.push_back(pid);
        wait(nullptr);
        break;
    }

    /* Q1 : */
    // for (int i = 0; i<N; ++i) 
    //     wait(NULL);

    /* Q2 : */
    // for (int i = 0; i<N; ++i) {
    //     int ret_val = 0;
    //     waitpid(-1, &ret_val,0);
    //     std::cout << "Mon fils de id : " << i
    //         << " a terminé avec : " << WEXITSTATUS(ret_val) << std::endl; 
    // }

    /* Q3 */
    // for (int i = 0; i<N; ++i) {
    //     int ret_val = 0;
    //     waitpid(-1, &ret_val,0);
    //     std::cout << "Mon fils de id : " << childs[i]
    //         << " a terminé avec : " << WEXITSTATUS(ret_val) << std::endl; 
    // }

    return EXIT_SUCCESS;
}