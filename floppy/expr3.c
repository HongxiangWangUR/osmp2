#include <signal.h>
#include "prinfo.h"
#include <unistd.h>

int main(int argc, char * const * argv)
{
	sigset_t s;
	struct prinfo p = { 0 };
	p.pid = getpid();

	if (sigemptyset(&s) < 0) {
		printf("sigemptyset");
		exit(0);
	}
	if (sigaddset(&s, SIGUSR2) < 0) {
		printf("sigaddset");
		exit(0);
	}
	if (sigprocmask(SIG_BLOCK, &s, NULL) < 0) { /* sigblock() is deprecated */
		printf("sigprocmask");
		exit(0);
	}

	if (kill(p.pid, SIGUSR2) < 0) {
		printf("first kill");
		exit(0);
	}
	if (kill(p.pid, SIGUSR2) < 0) {
		printf("second kill");
		exit(0);
	}

	if (syscall(181, &p) < 0) {
		printf("prinfo");
		exit(0);
	}
	
	show_result(&p);

	return 0;
}