#include <iostream>
#include <sys/mman.h>
#include <wait.h>
#include "Stack.h"

#define N 3
#define M 2

void prod(pr::Stack<int> *stack) {
    for (int i = 0; i<10; i++) 
        stack->push(0);
    std::cout << "finished prod" << std::endl;
}

void cons(pr::Stack<int> *stack) {
    for (int i = 0; i<15; i++)
        stack->pop();
    std::cout << "finished cons" << std::endl;
}

template <typename T>
pr::Stack<T> *createStack(const char *name) {
    int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0666);
    if (fd < 0) return NULL;
    ftruncate(fd, sizeof(pr::Stack<T>));
    void *smh = (pr::Stack<T> *) mmap(
                                    0, sizeof(pr::Stack<T>),
                                    PROT_READ | PROT_WRITE,
                                    MAP_SHARED, fd, 0);
    close(fd);
    pr::Stack<T> *stack = new(smh) pr::Stack<T>();
    return stack;
}

int main(int argc, char const *argv[]){
    pr::Stack<int> *stack = createStack<int>("/stack");
    for (int i = 0; i<N; ++i) {
        if (!fork()) {
            prod(stack);
            goto clean_up;
        }
    }

    for (int i = 0; i<M; i++) {
        if (!fork()) {
            cons(stack);
            goto clean_up;
        }
    }

    for (int i = 0; i < M + N; i++) wait(NULL);
    delete stack;
    clean_up:
    munmap((void *) stack, sizeof(pr::Stack<int>));
    return 0;
}
