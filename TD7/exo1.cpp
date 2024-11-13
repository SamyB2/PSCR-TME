#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

void affiche (pid_t p1, pid_t p2, pid_t p3) {
    std::cout << p1 << " " <<
                p2 << " " <<
                p3 << std::endl;
}

int main(int argc, char const *argv[]){
    pid_t pid = getpid();
    int t[2];
    if (pipe(t)) exit(-1);
    pid_t pid1 = fork();
    if (!pid1) {
        pid_t f1 = getpid();
        pid_t f2;
        read(t[0], &f2, sizeof(pid_t));
        close(t[0]);
        close(t[1]);
        affiche(pid1, f1, f2);
    }else {
        pid_t pid2 = fork();
        if (pid2) {
            pid_t f1 = getpid();
            affiche(pid, pid1, f1);
            write(t[1], &f1, sizeof(pid_t));
            close(t[0]);
            close(t[1]);
        }else{
            affiche(pid, pid1, pid2);
            close(t[0]);
            close(t[1]);
        }
    }
    return 0;
}
