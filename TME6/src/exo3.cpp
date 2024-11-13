#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "rsleep.h"

int timeout = 0;

void handler(int sig) {
	timeout = 1;
}

int wait_till_pid(pid_t pid) {
	pid_t ret = 0;
	do {
		ret = wait(nullptr);
	}while(ret != pid || ret != -1);
	return ret;
}

int wait_till_pid(pid_t pid, int sec) {
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = handler;
	sigaction(SIGALRM, &act, nullptr);

	pid_t ret = 0;
	alarm(sec);
	do {
		ret = wait(nullptr);
	}while(ret != pid && ret != -1 && !timeout);
	alarm(0);
	timeout = 0;
	return ret;
}

int main() {

}
