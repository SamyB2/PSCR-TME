#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "rsleep.h"

int pv = 3;

void handle_sig(int sig) {
	pv -= 1;
	if (!pv) {
		std::cout << "perdu le combat meurt..." << std::endl;
		exit(1);
	}
	std::cout << "reçu une attaque -1pv, reste : "
			<< pv << std::endl;
}

void handle_sig_luke(int sig) {
	std::cout << "attaque paréee"
			<< std::endl;
}

void attaque (pid_t adversaire) {
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = handle_sig;
	sigaction(SIGINT, &act, nullptr);

	pid_t ret = kill(adversaire, SIGINT);
	if (ret == -1) {
		std::cout << "adversqire defqit !!" << std::endl;
		exit(0);
	}
	randsleep();
}

void defense() {
	struct sigaction act = {0};
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = SIG_IGN;
	sigaction(SIGINT, &act, nullptr);

	randsleep();
}

void defense_luke() {
	struct sigaction act = {0};
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = handle_sig_luke;
	sigaction(SIGINT, &act, nullptr);

	sigset_t mask ;
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_BLOCK, &mask, nullptr);

	randsleep();

	sigset_t to_check ;
	sigfillset(&to_check);
	sigdelset(&to_check, SIGINT);
	sigsuspend(&to_check);
}

void combat(pid_t adv) {
	while(1) {
		attaque(adv);
		defense();
	}
}

void combat_luke(pid_t adv) {
	while(1) {
		attaque(adv);
		defense_luke();
	}
}


int main () {
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = SIG_IGN;
	sigaction(SIGINT, &act, nullptr);

	pid_t pid = fork();
	// if (!pid) combat_luke(getppid());
	// else combat(pid);

	if (!pid) combat(getppid());
	else combat(pid);

}
