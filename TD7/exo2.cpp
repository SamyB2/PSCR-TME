#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>

void write (const std::string &str, int fd) {
    size_t sz = str.length();
    write(fd, &sz, sizeof(sz));
    write(fd, str.data(), sz);
}

int read(std::string &str, int fd) {
    size_t sz;
    int lu = read(fd, &sz, sizeof(sz));
    if (!lu) return 0;
    char * buff = new char[sz];
    read(fd, buff, sz);
    str = std::string(buff, sz);
    delete [] buff;
}

bool finished = false;

void handler(int sig) {
    finished = true;
}

void writer() {
    signal(SIGINT, handler);
    char * name = "boooo";
    mkfifo(name, 0666);
    int fdw = open(name, O_WRONLY);
    while (!finished) {
        std::string str;
        std::cin >> str;
        write(str, fdw);
    }
    unlink(name);
}

void reader(char * name) {
    int fdr = open(name, O_RDONLY);
    while(true) {
        std::string str;
        int sz = read(str, fdr);
        if (!sz) break;
        std::cout << str << std::endl;
    }
}

/*
    -> plusieurs lecteurs et ecrivains : FF 
        car entrelacement des read et write fait nimp
    -> plusieurs writer 1 reader : FF
        car entralacement des write
    -> plusieurs writer qui font 1 write et 1 reader : gud

    -> plusieurs str à taille variable : 
        => se send des paquets de taille fixe, une struct qui contient un buff fixe et une size 
*/