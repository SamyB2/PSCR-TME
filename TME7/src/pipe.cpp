#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <wait.h>
// 0 1 2 3
void exec (int start, int end, const char * argv[]) {
    char **args = new char*[end - start + 1];
    for (int i = 0; i<end-start; i++) {
        args[i] = strdup(argv[start + i]);
    }
    args[end-start] = nullptr;
    if (execvp(argv[start], args) == -1) {
        perror("exec boo");
        exit(3);
    }
}

// cmd1 | cmd2

int main(int argc, char const *argv[]){
    for (int i = 1; i<argc; i++) {
        if(!strcmp(argv[i], "|")) {
            int newp[2];
            if (pipe(newp)) exit(1);
            pid_t pid = fork();
            if (!pid) { // exec cmd1
                dup2(newp[1], STDOUT_FILENO);
                close(newp[1]);
                close(newp[0]);
                exec(1, i, argv);
            }else { // exec cmd2
                dup2(newp[0], STDIN_FILENO);
                close(newp[0]);
                close(newp[1]);
                exec(i+1, argc, argv);
            }
        }
    }
    return 0;
}
