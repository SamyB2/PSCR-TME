#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main2 () {
	const int N = 3;
	int nb_fils = 0;
	std::cout << "main pid=" << getpid() << std::endl;
	pid_t pid;
	for (int i=1, j=N; i<=N && j==N && (pid = fork())==0 ; i++ ) {
		nb_fils++;
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++) {
			if ( (pid = fork()) == 0) {
				j=0;
				std::cout << " k:j " << k << ":" << j << std::endl;
			}
			if(pid) nb_fils++;
			else nb_fils = 0;
		}
	}
	nb_fils++;

	for (int i = 0; i< nb_fils; ++i)
		wait(nullptr);
	pid = wait(nullptr);
	if (pid == -1)
		std::cout << "gud !" << std::endl;
	else
		std::cout << "not gud !" << std::endl;

	return 0;
}
